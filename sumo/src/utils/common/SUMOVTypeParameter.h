/****************************************************************************/
/// @file    SUMOVTypeParameter.h
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @date    10.09.2009
/// @version $Id$
///
// Structure representing possible vehicle parameter
/****************************************************************************/
// SUMO, Simulation of Urban MObility; see http://sumo-sim.org/
// Copyright (C) 2001-2014 DLR (http://www.dlr.de/) and contributors
/****************************************************************************/
//
//   This file is part of SUMO.
//   SUMO is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
/****************************************************************************/
#ifndef SUMOVTypeParameter_h
#define SUMOVTypeParameter_h


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <string>
#include <map>
#include "SUMOVehicleClass.h"
#include "RGBColor.h"
#include "SUMOTime.h"
#include <utils/common/Parameterised.h>


// ===========================================================================
// class declarations
// ===========================================================================
class OutputDevice;
class OptionsCont;


// ===========================================================================
// value definitions
// ===========================================================================
const int VTYPEPARS_LENGTH_SET = 1;
const int VTYPEPARS_MINGAP_SET = 2;
const int VTYPEPARS_MAXSPEED_SET = 2 << 2;
const int VTYPEPARS_PROBABILITY_SET = 2 << 3;
const int VTYPEPARS_SPEEDFACTOR_SET = 2 << 4;
const int VTYPEPARS_SPEEDDEVIATION_SET = 2 << 5;
const int VTYPEPARS_EMISSIONCLASS_SET = 2 << 6;
const int VTYPEPARS_COLOR_SET = 2 << 7;
const int VTYPEPARS_VEHICLECLASS_SET = 2 << 8;
const int VTYPEPARS_WIDTH_SET = 2 << 9;
const int VTYPEPARS_HEIGHT_SET = 2 << 10;
const int VTYPEPARS_SHAPE_SET = 2 << 11;
const int VTYPEPARS_OSGFILE_SET = 2 << 12;
const int VTYPEPARS_IMGFILE_SET = 2 << 13;
const int VTYPEPARS_IMPATIENCE_SET = 2 << 14;
const int VTYPEPARS_LANE_CHANGE_MODEL_SET = 2 << 15;


// ===========================================================================
// struct definitions
// ===========================================================================
/**
 * @class SUMOVTypeParameter
 * @brief Structure representing possible vehicle parameter
 */
class SUMOVTypeParameter : public Parameterised {
public:
    /** @brief Constructor
     *
     * Initialises the structure with default values
     */
    SUMOVTypeParameter(const std::string& vtid, const SUMOVehicleClass vc = SVC_IGNORING);


    /** @brief Returns whether the given parameter was set
     * @param[in] what The parameter which one asks for
     * @return Whether the given parameter was set
     */
    bool wasSet(int what) const {
        return (setParameter & what) != 0;
    }


    /** @brief Writes the vtype
     *
     * @param[in, out] dev The device to write into
     * @exception IOError not yet implemented
     */
    void write(OutputDevice& dev) const;

    /** @brief Validates stored car-following parameter
     */
    void validateCFParameter() const;


    /** @brief Returns the named value from the map, or the default if it is not contained there
     * @param[in] attr The corresponding xml attribute
     * @param[in] defaultValue The value to return if the given map does not contain the named variable
     * @return The named value from the map or the default if it does not exist there
     */
    SUMOReal get(const SumoXMLAttr attr, const SUMOReal defaultValue) const;


    /// @brief The vehicle type's id
    std::string id;

    /// @brief The physical vehicle length
    SUMOReal length;
    /// @brief This class' free space in front of the vehicle itself
    SUMOReal minGap;
    /// @brief The vehicle type's maximum speed [m/s]
    SUMOReal maxSpeed;
    /// @brief The probability when being added to a distribution without an explicit probability
    SUMOReal defaultProbability;
    /// @brief The factor by which the maximum speed may deviate from the allowed max speed on the street
    SUMOReal speedFactor;
    /// @brief The standard deviation for speed variations
    SUMOReal speedDev;
    /// @brief The emission class of this vehicle
    SUMOEmissionClass emissionClass;
    /// @brief The color
    RGBColor color;
    /// @brief The vehicle's class
    SUMOVehicleClass vehicleClass;
    /// @brief The vehicle's impatience (willingness to obstruct others)
    SUMOReal impatience;
    /* /// @brief The vehicle's minimum headway time (tau) // will be moved here from the cfModel eventually
    SUMOReal headwayTime;
    /// @brief The vehicle's maximum acceleration
    SUMOReal maxAccel;
    /// @brief The vehicle's maximum deceleration
    SUMOReal maxDecel;
    /// @brief The driver's imperfection (dawdling factor in Krauss-like models)
    SUMOReal imperfection; */


    /// @name Values for drawing this class' vehicles
    /// @{

    /// @brief This class' width
    SUMOReal width;

    /// @brief This class' height
    SUMOReal height;

    /// @brief This class' shape
    SUMOVehicleShape shape;

    /// @brief 3D model file for this class
    std::string osgFile;

    /// @brief Image file for this class
    std::string imgFile;
    /// @}


    /// @brief The enum-representation of the car-following model to use
    SumoXMLTag cfModel;

    /// @brief Car-following parameter
    typedef std::map<SumoXMLAttr, SUMOReal> CFParams;
    CFParams cfParameter;

    /// @brief The lane-change model to use
    LaneChangeModel lcModel;

    /// @brief Information for the router which parameter were set
    int setParameter;


    /// @brief Information whether this type was already saved (needed by routers)
    mutable bool saved;

    /// @brief Information whether this is a type-stub, being only referenced but not defined (needed by routers)
    mutable bool onlyReferenced;

    static SUMOReal getDefaultMaxSpeed(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultAccel(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultDecel(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultSigma(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultLength(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultMinGap(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultTau(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultImpatience(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultSpeedFactor(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultSpeedDev(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultWidth(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultHeight(const SUMOVehicleClass vc = SVC_IGNORING);
    static SumoXMLTag getDefaultFollowModel(const SUMOVehicleClass vc = SVC_IGNORING);
    static LaneChangeModel getDefaultLaneChangeModel(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOVehicleShape getDefaultShape(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOEmissionClass getDefaultEmissionClass(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultTmp1(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultTmp2(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultTmp3(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultTmp4(const SUMOVehicleClass vc = SVC_IGNORING);
    static SUMOReal getDefaultTmp5(const SUMOVehicleClass vc = SVC_IGNORING);
        

};

#endif

/****************************************************************************/

