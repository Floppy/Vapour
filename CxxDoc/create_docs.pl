#!/usr/bin/perl -w

# bring in the environment vars
use strict;
use Env qw(VALET_SRCROOT VALET_DOCROOT);

# make the eht directory
`mkdir eht`;
# move the eht files into the dir
my $find_results = "`find $VALET_SRCROOT -name '*.eht'`";
`cp $find_results eht`;
# run CxxDoc on the source
`CxxDoc -pn VALET -o $VALET_DOCROOT -p $VALET_SRCROOT -i $VALET_SRCROOT -eht eht -tc class -td docnode`;
