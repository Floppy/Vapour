                        Twister - Rotation Converter

                        General Information and Help

                             January 26, 1999


                               INTRODUCTION

Thanks for downloading Twister, from Vapour Technology. Twister is a rotation
calculator that converts an Euler rotation (ie components about the three x y
and z axes) into an axis-angle representation. It is intended for use with
VRML, but feel free to use it for other applications if you feel the need. It
may be useful for OpenGL and other graphics applications that use a right-
handed coordinate system.
Twister was inspired by a couple of enquiries by content developers to myself
and others. We hope you find it useful!


                               INSTALLATION

To install Twister on you system, run the executable 'twister_install.exe'.
Select the directory you wish to install to, and the rest is done
automatically. Unfortunately, you have to add your own icon to the Start Menu,
but such is life.


                                  USAGE

Twister converts a three-axis style Euler rotation into a axis-angle style
rotation. An Euler rotation is defined by three values, each one a rotation
about the X Y or Z axis. Twister assumes that the three values rotate around
the X Y and Z axes respectively. Also, the rotation calculations are done in
this order.

The input is three floating-point values representing the rotations around
each axis. These are entered into the appropriate boxes at the top of the
window. If an invalid value is entered, the box is reset to zero on update.
To update the result, press the Update button or hit Enter. The result at
the bottom of the window will be recalculated and displayed.

The input can be entered in either radians or degrees by setting the
appropriate choice on the radio buttons. If you enter a number, and switch
to the other angle measurement, the numbers already entered will be converted
to the current measure. So, if you enter 180 degrees and press the 'radians'
button, the number will be converted to 3.14 radians.

The output is a VRML-style axis-angle orientation. The first three values
define an axis about which to rotate, and the last value is the amount (in
radians) to rotate about this axis. This is the correct format to be pasted
directly into a VRML file. This can be done using the copy button to copy the
result directly onto the clipboard.


                           CONTACT INFORMATION

Vapour Technology can be reached on the web at http://www.vapourtech.com.

For bug reports and enquiries about this product, email support@vapourtech.com.
