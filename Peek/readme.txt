                        Peek - Orientation Calculator

                        General Information and Help

                              January 26, 1999


                               INTRODUCTION

Thanks for downloading Peek, from Vapour Technology. Peek is an orientation
calculator that converts a position/target/up camera model into an axis-angle
camera model. While this is useful for calculating camera orientations, you
can also use it to orient any object. Peek is aimed at VRML applications,
though you may find it useful in OpenGL or any graphics system with a right-
handed coordinate system.
Peek was inspired by a similar Win32 console app called 'Orient' by Steve
Chenney. We decided to take the concept and rewrite it into a user-friendly
application. We hope you find it useful!

                               INSTALLATION

To install Peek on you system, run the executable 'peek_install.exe'. Select
the directory you wish to install to, and the rest is done automatically.
Unfortunately, you have to add you own icon to the Start Menu, but such is
life.


                                  USAGE

Peek converts a position/target/up camera model into an axis-angle style
orientation for the camera. To do this, enter the position of the camera,
the target point and the up direction into the appropriate boxes and press
the 'Update' button (or Enter). The normalised orientation for the camera
will then be calculated and displayed at the bottom of the window, ready to
be copied into your VRML file.

The input data is entered as three sets of three floating-point values, such
as:
          1.1 0.4 5.6
The three values are the x, y and z components of the points/vectors as
appropriate.

The output is a complete VRML Viewpoint node. Using the copy button, you can
copy this to the clipboard and paste it directly into your VRML files. If you
want the data for other applications, such as OpenGL, the orientation field
in the output is an axis/angle rotation value. The first three fields define
an axis, and the last is a rotation in radians about this axis.

                           CONTACT INFORMATION

Vapour Technology can be reached on the web at http://www.vapourtech.com.

For bug reports and enquiries about this product, email support@vapourtech.com.
