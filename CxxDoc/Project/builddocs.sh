#!/bin/bash

#======================---
# PROJECT Documentation
#----------------------
# Documentation system
# Copyright 2002 Vapour Technology Ltd.
#
# 02/04/2002 - Warren Moore
#
# Uses CxxDoc from http://ravl.sf.net
#

# pull in the eht's
echo "PROJECT Documentation"
echo "Copyright 2002 Vapour Technology Ltd."

# Get the eht's
mkdir eht 2> /dev/null
cp -f `find .. -name '*.eht' -printf "%h/*.eht "` eht 2> /dev/null

# Generate the documentation
rm -rf ../Doc/*
CxxDoc -pn PROJECT -o ../Doc/ -p .. -i .. -eht eht -tc class -td docnode

# fix the namespace problems
cp -f ../Doc/user/class/..html ../Doc/user/class/std.html 2> /dev/null
cp -f ../Doc/developer/class/..html ../Doc/developer/class/std.html

# Remove the temp dir
rm -rf eht
