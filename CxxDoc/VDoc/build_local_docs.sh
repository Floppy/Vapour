#!/bin/bash

# build_local_docs.sh

# script to set doc options, checkout the VDoc module and
# run the full arch doc generation script

# Add confidentiality notice

# 13/09/2001 - Warren Moore
# Copyright 2000-2001 Vapour Technology Ltd.

#===--- Edit these to get a local doc build working 
# VALET_CVSOPTIONS must contain all the options for co from the VALET cvs repos
VALET_CVSOPTIONS="-z9 -Q -d :ext:vap-warren@cvs.vapourtech.com:/cvs"
# VALET_DOCROOT is the local path for generated docs
VALET_DOCROOT=/www/secure/www/internal/docs/
export VALET_CVSOPTIONS VALET_DOCROOT

# remove the previous documentation
rm -rf $VALET_DOCROOT*

# checkout the doc module
cvs $VALET_CVSOPTIONS co VDoc

# if the create_docs.pl script was checked out ok, run that
if [ -x VDoc/create_docs.pl ]
	then
		# change into dir and run the script
		cd VDoc
		./create_docs.pl
		cd ..
	else
		# didn't find it so bomb out
		echo "example.sh error: Unable to find VDoc/create_docs.pl"
fi

# delete the module directory
rm -rf VDoc
