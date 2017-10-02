/****************************************************************************/
/// @file    TraCI_TrafficLight.cpp
/// @author  Daniel Krajzewicz
/// @author  Mario Krumnow
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @date    30.05.2012
/// @version $Id$
///
// C++ TraCI client API implementation
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
// Copyright (C) 2017-2017 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <microsim/MSLane.h>
#include <microsim/MSNet.h>
#include <microsim/traffic_lights/MSTLLogicControl.h>
#include <microsim/traffic_lights/MSSimpleTrafficLightLogic.h>
#include "TraCI_TrafficLight.h"


// ===========================================================================
// member definitions
// ===========================================================================
std::vector<std::string>
TraCI_TrafficLight::getIDList() {
    return MSNet::getInstance()->getTLSControl().getAllTLIds();
}


int
TraCI_TrafficLight::getIDCount() {
    return (int)getIDList().size();
}


std::string
TraCI_TrafficLight::getRedYellowGreenState(const std::string& tlsID) {
    return getTLS(tlsID).getActive()->getCurrentPhaseDef().getState();
}


std::vector<TraCILogic>
TraCI_TrafficLight::getCompleteRedYellowGreenDefinition(const std::string& tlsID) {
    std::vector<TraCILogic> result;
    const std::vector<MSTrafficLightLogic*> logics = getTLS(tlsID).getAllLogics();
    for (MSTrafficLightLogic* logic : logics) {
        TraCILogic l(logic->getProgramID(), 0, logic->getCurrentPhaseIndex());
        l.subParameter = logic->getMap();
        for (int j = 0; j < logic->getPhaseNumber(); ++j) {
            MSPhaseDefinition phase = logic->getPhase(j);
            l.phases.emplace_back(TraCIPhase(phase.duration, phase.minDuration, phase.maxDuration, phase.getState()));
        }
        result.emplace_back(l);
    }
    return result;
}


std::vector<std::string>
TraCI_TrafficLight::getControlledJunctions(const std::string& tlsID) {
    std::set<std::string> junctionIDs;
    const MSTrafficLightLogic::LinkVectorVector& links = getTLS(tlsID).getActive()->getLinks();
    for (const MSTrafficLightLogic::LinkVector& llinks : links) {
        for (const MSLink* l : llinks) {
            junctionIDs.insert(l->getJunction()->getID());
        }
    }
    return std::vector<std::string>(junctionIDs.begin(), junctionIDs.end());
}


std::vector<std::string>
TraCI_TrafficLight::getControlledLanes(const std::string& tlsID) {
    std::vector<std::string> laneIDs;
    const MSTrafficLightLogic::LaneVectorVector& lanes = getTLS(tlsID).getActive()->getLaneVectors();
    for (const MSTrafficLightLogic::LaneVector& llanes : lanes) {
        for (const MSLane* l : llanes) {
            laneIDs.push_back(l->getID());
        }
    }
    return laneIDs;
}


std::vector<std::vector<TraCILink> >
TraCI_TrafficLight::getControlledLinks(const std::string& tlsID) {
    std::vector<std::vector<TraCILink> > result;
    const MSTrafficLightLogic::LaneVectorVector& lanes = getTLS(tlsID).getActive()->getLaneVectors();
    const MSTrafficLightLogic::LinkVectorVector& links = getTLS(tlsID).getActive()->getLinks();
    for (int i = 0; i < (int)lanes.size(); ++i) {
        std::vector<TraCILink> subList;
        const MSTrafficLightLogic::LaneVector& llanes = lanes[i];
        const MSTrafficLightLogic::LinkVector& llinks = links[i];
        // number of links controlled by this signal (signal i)
        for (int j = 0; j < (int)llanes.size(); ++j) {
            MSLink* link = llinks[j];
            // approached non-internal lane (if any)
            const std::string to = link->getLane() != 0 ? link->getLane()->getID() : "";
            // approached "via", internal lane (if any)
            const std::string via = link->getViaLane() != 0 ? link->getViaLane()->getID() : "";
            subList.emplace_back(TraCILink(llanes[j]->getID(), via, to));
        }
        result.emplace_back(subList);
    }
    return result;
}


std::string
TraCI_TrafficLight::getProgram(const std::string& tlsID) {
    return getTLS(tlsID).getActive()->getProgramID();
}


int
TraCI_TrafficLight::getPhase(const std::string& tlsID) {
    return getTLS(tlsID).getActive()->getCurrentPhaseIndex();
}


SUMOTime
TraCI_TrafficLight::getPhaseDuration(const std::string& tlsID) {
    return getTLS(tlsID).getActive()->getCurrentPhaseDef().duration;
}


SUMOTime
TraCI_TrafficLight::getNextSwitch(const std::string& tlsID) {
    return getTLS(tlsID).getActive()->getNextSwitchTime();
}


std::string
TraCI_TrafficLight::getParameter(const std::string& tlsID, const std::string& paramName) {
    return getTLS(tlsID).getActive()->getParameter(paramName, "");
}


void
TraCI_TrafficLight::setRedYellowGreenState(const std::string& tlsID, const std::string& state) {
    getTLS(tlsID).setStateInstantiatingOnline(MSNet::getInstance()->getTLSControl(), state);
}


void
TraCI_TrafficLight::setPhase(const std::string& tlsID, const int index) {
    MSTrafficLightLogic* const active = getTLS(tlsID).getActive();
    if (index < 0 || active->getPhaseNumber() <= index) {
        throw TraCIException("The phase index " + toString(index) + " is not in the allowed range [0,"
            + toString(active->getPhaseNumber() - 1) + "].");
    }
    const SUMOTime cTime = MSNet::getInstance()->getCurrentTimeStep();
    const SUMOTime duration = active->getPhase(index).duration;
    active->changeStepAndDuration(MSNet::getInstance()->getTLSControl(), cTime, index, duration);
}


void
TraCI_TrafficLight::setProgram(const std::string& tlsID, const std::string& programID) {
    try {
        getTLS(tlsID).switchTo(MSNet::getInstance()->getTLSControl(), programID);
    } catch (ProcessError& e) {
        throw TraCIException(e.what());
    }
}


void
TraCI_TrafficLight::setPhaseDuration(const std::string& tlsID, const SUMOTime phaseDuration) {
    MSTrafficLightLogic* const active = getTLS(tlsID).getActive();
    const SUMOTime cTime = MSNet::getInstance()->getCurrentTimeStep();
    const int index = active->getCurrentPhaseIndex();
    active->changeStepAndDuration(MSNet::getInstance()->getTLSControl(), cTime, index, phaseDuration);
}


void
TraCI_TrafficLight::setCompleteRedYellowGreenDefinition(const std::string& tlsID, const TraCILogic& logic) {
    MSTLLogicControl::TLSLogicVariants& vars = getTLS(tlsID);
    // make sure index and phaseNo are consistent
    if (logic.currentPhaseIndex >= (int)logic.phases.size()) {
        throw TraCIException("set program: parameter index must be less than parameter phase number.");
    }
    std::vector<MSPhaseDefinition*> phases;
    for (TraCIPhase phase : logic.phases) {
        phases.push_back(new MSPhaseDefinition(phase.duration, phase.duration1, phase.duration2, phase.phase));
    }
    if (vars.getLogic(logic.subID) == 0) {
        MSTrafficLightLogic* mslogic = new MSSimpleTrafficLightLogic(MSNet::getInstance()->getTLSControl(), tlsID, logic.subID, phases, logic.currentPhaseIndex, 0, logic.subParameter);
        vars.addLogic(logic.subID, mslogic, true, true);
    } else {
        static_cast<MSSimpleTrafficLightLogic*>(vars.getLogic(logic.subID))->setPhases(phases, logic.currentPhaseIndex);
    }
}


void
TraCI_TrafficLight::setParameter(const std::string& tlsID, const std::string& paramName, const std::string& value) {
    return getTLS(tlsID).getActive()->setParameter(paramName, value);
}


MSTLLogicControl::TLSLogicVariants&
TraCI_TrafficLight::getTLS(const std::string& id) {
    if (!MSNet::getInstance()->getTLSControl().knows(id)) {
        throw TraCIException("Traffic light '" + id + "' is not known");
    }
    return MSNet::getInstance()->getTLSControl().get(id);
}


/****************************************************************************/