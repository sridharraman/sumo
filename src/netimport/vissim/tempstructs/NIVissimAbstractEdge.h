#ifndef NIVissimAbstractEdge_h
#define NIVissimAbstractEdge_h
//---------------------------------------------------------------------------//
//                        NIVissimAbstractEdge.h -  ccc
//                           -------------------
//  project              : SUMO - Simulation of Urban MObility
//  begin                : Sept 2002
//  copyright            : (C) 2002 by Daniel Krajzewicz
//  organisation         : IVF/DLR http://ivf.dlr.de
//  email                : Daniel.Krajzewicz@dlr.de
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//
// $Log$
// Revision 1.5  2003/06/05 11:46:55  dkrajzew
// class templates applied; documentation added
//
//


/* =========================================================================
 * included modules
 * ======================================================================= */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H


#include <map>
#include <utils/common/IntVector.h>
#include <utils/geom/Position2DVector.h>

/* =========================================================================
 * class definitions
 * ======================================================================= */
/**
 *
 */
class NIVissimAbstractEdge {
public:
    NIVissimAbstractEdge(int id, const Position2DVector &geom);
    virtual ~NIVissimAbstractEdge();
    Position2D getGeomPosition(double pos) const;
    Position2D getBeginPosition() const;
    Position2D getEndPosition() const;
    void splitAssigning();
    bool crossesEdge(NIVissimAbstractEdge *c) const;
    Position2D crossesEdgeAtPoint(NIVissimAbstractEdge *c) const;
    bool overlapsWith(const AbstractPoly &p, double offset=0.0) const;
    virtual void setNodeCluster(int nodeid) = 0;
    bool hasNodeCluster() const;
    double crossesAtPoint(const Position2D &p1,
        const Position2D &p2) const;

    bool hasGeom() const;
    virtual void buildGeom() = 0;
    int getID() const;
    const Position2DVector &getGeometry() const;

    void addDisturbance(int disturbance);

    const IntVector &getDisturbances() const
    { return myDisturbances; }

public:
    static bool dictionary(int id, NIVissimAbstractEdge *e);
    static NIVissimAbstractEdge *dictionary(int id);
    static void splitAndAssignToNodes();
    static IntVector getWithin(const AbstractPoly &p, double offset=0.0);
    static void clearDict();


protected:
    int myID;
    Position2DVector myGeom;
    IntVector myDisturbances;
    int myNode;

private:
    typedef std::map<int, NIVissimAbstractEdge*> DictType;
    static DictType myDict;
};




/**************** DO NOT DEFINE ANYTHING AFTER THE INCLUDE *****************/
//#ifndef DISABLE_INLINE
//#include "NIVissimAbstractEdge.icc"
//#endif

#endif

// Local Variables:
// mode:C++
// End:

