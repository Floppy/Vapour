#!/usr/bin/perl -w

# VMake.pl

# script to emulate intended VMake functionality

# 17/09/2001 - Warren Moore
# $Id: VMake.pl,v 1.10 2001/09/26 01:36:20 vap-warren Exp $
# Copyright 2000-2001 Vapour Technology Ltd.

use strict;

#=== global vars
# get a nice version number from the rcs id
my $rcs_id = '$Id: VMake.pl,v 1.10 2001/09/26 01:36:20 vap-warren Exp $';
$rcs_id =~ s/\$//g;
my $version = ($rcs_id =~ /^Id: [^\s]+ ([0-9\.]+)/) ? $1 : "(unknown)";
my $list_name = "arch.list";		# default arch.list location
my $def_build = "noarch";			# default build target
my $silent = "";						# default verbose output
my $doc = "";							# default build, not document
my $banner = <<BANNER;				# app banner
VMake.pl v$version
17/01/2001 - Warren Moore
Copyright 2001 Vapour Technology Ltd.
BANNER

#=== text function
sub output {
	if ($silent) {
		return;
	}
	my $text = shift(@_);
	print "$text";
}

#=== error function
sub error {
	my $text = shift(@_);
	print "VMake.pl error: $text\n";
	exit -1;
}

#=== main start
# get the build architecture
my $buf = "";
while (@ARGV) {
	$buf = shift(@ARGV);
	# get options
	if ($buf =~ /-[\w\d]/) {
		# silent running
		if ($buf eq "-0") {
			$silent = "true";
		}
		# arch file location
		if ($buf eq "-a") {
			$list_name = shift(@ARGV);
		}
		# cxxdoc format
		if ($buf eq "-doc") {
			$doc = "true";
		}
		$buf = "";
	}
}
my $arch_name = $buf ? $buf : $def_build;

# Print the prog banner
if (not $silent) {
	output($banner);
}

# check we have an arch
if (not $arch_name) {
	error("Usage - VMake.pl <arch>");
}

# read in the arch list
if (not open(ARCH_LIST, "<$list_name")) {
	error("Unable to open '$list_name' (set with -a)");
}
my @arch_list;
my @arch_desc;
ARCH_LIST_SKIP: while (<ARCH_LIST>) {
	# skip comments and blank lines
	next ARCH_LIST_SKIP if /^\s*\#/;
	next ARCH_LIST_SKIP if /^\s*$/;
	# remove any trailing newlines
	chomp;
	# if found, add the arch data
	if (/^([\w\d]+)\s+(.+)/) {
		push @arch_list, $1;
		push @arch_desc, $2;
	}
}
close(ARCH_LIST);

# check to make sure that the arch is valid
my $found = "";
my $count = 0;
ARCH_CHECK: foreach my $valid_arch(@arch_list) {
	if ($arch_name eq $valid_arch) {
		$found = "true";
		last ARCH_CHECK;
	}
	$count++;
}
if ($found) {
	output("Building architecture: $arch_desc[$count]\n");
}
else {
	error("Invalid architecture: $arch_name");
}

# remove any non-arch files
my @file_list = split /\n/, `find . -name '*.*'`;
my @rm_list;
$count = 0;
while ($count < scalar(@file_list)) {
	my $file = $file_list[$count];
	# search for files matching *.*.*
	if ($file =~ /(.+\/)([^\.\/]+)\.([^\.]+)\.([^\.~]+)$/) {
		my $search = "$1$2.$arch_name.$4";
		my $arch = $3;
		# forget about non-arch files
		if (not grep { /$arch/ } @arch_list) {
			splice(@file_list, $count, 1);
		}
		# forget or remove unused arch files
		elsif ($arch ne "noarch" and $arch ne $arch_name) {
			push @rm_list, splice(@file_list, $count, 1);
		}
		# forget or remove noarch files if an overiding arch file exists
		elsif ($arch_name ne "noarch" and $arch eq "noarch" and (grep { /$search/ } @file_list)) {
			push @rm_list, splice(@file_list, $count, 1);
		}
		# step through the list
		else {
			$count++;
		}
	}
	# ignore's symbolic links
	elsif (readlink $file) {
		splice(@file_list, $count, 1);
	}
	# ignore non-arch files
	else {
		splice(@file_list, $count, 1);
	}
}

# remove unused files
foreach my $file (@rm_list) {
	# remove unused files for doc build
	if ($doc) {
		output("Removing: $file\n");
		unlink $file;
	}
}

# Create links for remaining arch files
foreach my $file (@file_list) {
	# generate the link filename
	my $link = "";
	my $target = "";
	my $file_arch = "";
	if ($file =~ /(.+\/)([^\.\/]+)\.([^\.]+)\.([^\.]+)$/) {
		$link = $1 . $2 . "." . $4;
		$target = $2 . "." . $3 . "." . $4;
		$file_arch = $3;
	}
	else {
		error("Unable to generate target location for for '$file'");
	}

	# move them for doc build
	if ($doc) {
		output("Moving: $file -> $link\n");
		rename $file, $link or error("Unable to move '$file' to '$link'");
	}
	# generate the link
	else {
		output("Linking: $file -> $link\n");
		unlink $link if -r $link;
		symlink $target, $link or error("Unable to create symlink from '$link' to '$target'");
	}
}

