#!/bin/bash

#=========================================---
# VT Structure Visualisation Documentation
#-----------------------------------------
# Documentation system
# Copyright 2002 Vapour Technology Ltd.
#
# 02/04/2002 - Warren Moore
#
# Uses CxxDoc from http://ravl.sf.net
#

# pull in the eht's
echo "VTStrucVis Documentation"
echo "Copyright 2002 Vapour Technology Ltd."
mkdir eht 2> /dev/null
cp `find ../Control -name '*.eht'` eht
CxxDoc -pn VTStrucVis -o ../Doc/ -p ../Control -i ../Control -eht eht -tc class -td docnode
rm -rf eht
