                    CCFix - Visual C++ Registry Fixer 

                          Important Information 

                              July 11, 2001
 

                               INTRODUCTION

CCFix does some dangerous stuff - please read this document completely before 
running the program!

Thanks for downloading CCFix, from Vapour Technology. CCFix is a utility that
performs a number of registry fixes that allow Visual C++ to use files with the
extensions .cc and .hh. The files will be associated with Visual C++, and will
be fully syntax highlighted in the editor window. It is based on information 
published in the Microsoft Knowledge Base at 
http://support.microsoft.com/support/kb/articles/Q181/5/06.ASP . CCFix simply 
automates the steps explained in this document. We hope you find it useful!


                                  USAGE

While we have tested CCFix as far as we can, it is a dangerous program. CCFix 
CHANGES YOUR REGISTRY! If you do not know your way around the registry, we suggest 
that you do not use this tool, in case anything goes wrong. The software is an 
internal company tool provided as a service to the community, and is provided 
without warranty or guarantee. If this worries you, do not use this tool. 

CCFix has only been tested with Visual C++ 6.0, but should work with versions 5 and
4 as well. Again, you take your life into your own hands if you use it to fix these 
versions. However, if you do use it and it works, please let us know at the address 
below so that we can update this document.

If you do decide to use CCFix, BACK UP YOUR ENTIRE REGISTRY FIRST. Again, if you don't 
know how to do this, just delete this tool now.

In order to use CCFix, first of all make sure that Visual Studio is closed. The tool
will not work if any part of Visual Studio is running. Unzip the CCFix.zip file and 
run CCFix.exe. You should only need to run the executable once, but if the program
doesn't have the effect you expect, run it again and it should be OK.

Once you've run CCFix, you should be sorted. Visual C++ will now be associated with 
.cc and .hh files, and will syntax highlight them correctly. However, there is one 
thing you need to do. The compiler does not understand what language the files are - 
C or C++. To fix this, for any project that uses .cc files you need to include the
compiler flag /TP on the compiler command line. This can be found in the Project 
Settings dialog.

Remember - CCFix is provided WITHOUT WARRANTY OR GUARANTEE. Vapour Technology are NOT
responsible for any damage done to your machine by running this program.

                           CONTACT INFORMATION

Vapour Technology can be reached on the web at http://www.vapourtech.com.

For bug reports and enquiries about this product, email support@vapourtech.com.
