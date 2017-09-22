#!/usr/bin/env python
"""
@file    test.py
@author  Pablo Alvarez Lopez
@date    2016-11-25
@version $Id$

python script used by sikulix for testing netedit

SUMO, Simulation of Urban MObility; see http://sumo.dlr.de/
Copyright (C) 2009-2017 DLR (http://www.dlr.de/) and contributors

This file is part of SUMO.
SUMO is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
"""
# import common functions for netedit tests
import os
import sys

testRoot = os.path.join(os.environ.get('SUMO_HOME', '.'), 'tests')
neteditTestRoot = os.path.join(
    os.environ.get('TEXTTEST_HOME', testRoot), 'netedit')
sys.path.append(neteditTestRoot)
import neteditTestFunctions as netedit  # noqa

# Open netedit
neteditProcess, match = netedit.setupAndStart(neteditTestRoot)

# go to select mode
netedit.selectMode()

# select all using invert
netedit.selectionInvert()

# go to inspect mode
netedit.inspectMode()

# inspect E3s
netedit.leftClick(match, 150, 250)

# Set invalid freq
netedit.modifyAttribute(0, "-30")

# Set valid freq
netedit.modifyAttribute(0, "50")

# Set invalid time speed treshold
netedit.modifyAttribute(1, "-5")

# Set valid time speed treshold
netedit.modifyAttribute(1, "11.3")

# Set invalid speed speed treshold
netedit.modifyAttribute(2, "-3")

# Set valid speed speed treshold
netedit.modifyAttribute(2, "4")

# save additionals
netedit.saveAdditionals()

# save newtork
netedit.saveNetwork()

# quit netedit
netedit.quit(neteditProcess)
