#!/usr/bin/perl -w

# VMake.pl

# script to emulate intended VMake functionality

# 17/09/2001 - Warren Moore
# $Id: VMake.pl,v 1.3 2001/09/19 12:31:26 vap-warren Exp $
# Copyright 2000-2001 Vapour Technology Ltd.

use strict;

#=== global vars
my $list_name = "arch.list";		# default arch.list location
my $def_build = "noarch";			# default build target
my $silent = "";						# default verbose output
my $doc = "";							# default build, not document
my $banner = <<BANNER;				# app banner
VMake.pl
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
my @file_list = split /\n/, `find . -name '*.*.*'`;
my @rm_list;
$count = 0;
while ($count < scalar(@file_list)) {
	my $file = $file_list[$count];
	if ($file =~ /(.+\/)([^\.\/]+)\.([^\.]+)\.([^\.]+)$/) {
		my $search = "$1$2.$arch_name.$4";
		my $arch = $3;
		# remove it if it's not an arch dependent file
		if (not grep { /$arch/ } @arch_list) {
			splice(@file_list, $count, 1);
		}
		# remove unused arch files
		elsif ($arch ne "noarch" and $arch ne $arch_name) {
			print "Unused: $file\n";
			push @rm_list, splice(@file_list, $count, 1);
		}
		# remove unnecessary noarch's
		elsif ($arch_name ne "noarch" and $arch eq "noarch" and (grep { /$search/ } @file_list)) {
			print "Superfluous: $file\n";
			push @rm_list, splice(@file_list, $count, 1);
		}
		# remove noarch files if doc build
		elsif ($doc and $arch eq "noarch") {
			print "Doc: $file\n";
			push @rm_list, splice(@file_list, $count, 1);
		}
		# step through the list
		else {
			$count++;
		}
	}
}

# Create links for remaining arch files
foreach my $file (@file_list) {
	# generate the link filename
	my $link = "";
	my $target = "";
	if ($file =~ /(.+\/)([^\.\/]+)(\.[^\.]+)(\.[^\.]+)$/) {
		$link = $1 . $2 . $4;
		$target = $2 . $3 . $4;
	}
	else {
		error("Unable to generate target location for for '$file'");
	}
	# generate the link
	if (not $doc) {
		output("Linking: $file -> $link\n");
		unlink $link if -r $link;
		symlink $target, $link or error("Unable to create symlink from '$link' to '$target'");
	}
	# move them for doc build
	else {
		output("Moving: $file -> $link\n");
		rename $file, $link or error("Unable to move '$file' to '$link'");
	}
}

# show or remove unused files
foreach my $file (@rm_list) {
	# show unused files
	if (not $doc) {
		output("Unused: $file\n");
	}
	# remove them for doc build
	else {
		output("Removing unused: $file\n");
		unlink $file;
	}
}

