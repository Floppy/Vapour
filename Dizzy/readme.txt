                          Dizzy - Rotation Merger

                        General Information and Help

                             October 13, 1999


                               INTRODUCTION

Thanks for downloading Dizzy, from Vapour Technology. Dizzy is a rotation
merger that combines sequences of rotations into a single composite rotation
with the same result. It is intended for use with VRML, but feel free to use
it for other applications if you feel the need. It may be useful for OpenGL
and other graphics applications that use a right-handed coordinate system.
Dizzy was inspired by discussions on the newsgroup comp.lang.vrml about
rotation merging and combining multiple rotations. Some people posted pointers
to the quaternion maths necessary, and so we decided to write it all up in
one convenient Windows application. We hope you find it useful!

This new version has a number of new features, including the ability to float 
above other windows, variable precision output, and the ability to reorder 
rotations in the list.

                               INSTALLATION

To install Dizzy on you system, run the executable 'dizzy_install.exe'. Select
the directory you wish to install to, and the rest is done automatically.
Unfortunately, you have to add your own icon to the Start Menu, but such is
life.


                                  USAGE

Dizzy takes a sequence of rotations to be applied to an object and merges
them into a single rotation. In VRML, to have a sequence of rotations, it is
necessary to have multiple nested Transforms. Using Dizzy, you can reduce
these into a single Transform node.
To combine a sequence of rotations, type or paste them into the Input box at
the top of the window and press the 'Add' button (or hit Enter). The rotation
will then be added to the list in the box below. To combine a sequence of
rotations, add them to the list. The rotations are merged in DESCENDING order.
The topmost will be applied first, then the second, and so on.
To insert a rotation into the middle of the list, select the item below which
the new rotation should be added before adding it. The arrow buttons can also
be used to reorder the list.
After each rotation is added, the normalised resultant rotation is calculated 
and displayed at the bottom of the window, ready to be copied into your VRML 
file using the 'Copy' button. The number of decimal places in the output can
be altered by changing the 'Precision' setting.
To remove a value from the list, select the value to remove and press 'Delete'.
To empty the list, press the 'Clear' button.

The input data is entered as sets of four floating-point values, such
as:
          1.1 0.4 5.6 0.43
The first three values are the x, y and z components of rotation axis as
appropriate. The last is the value (in radians) to rotate about this axis.
Input can only be entered into the box in a valid format. If you enter data
into the input box and it does not appear, you are not entering a correctly
formatted number. Also, only four numeric values can be entered in the box.

The output is a VRML-style axis-angle orientation. The first three values
define an axis about which to rotate, and the last value is the amount (in
radians) to rotate about this axis. This is the correct format to be pasted
directly into a VRML file. The copy button copies the result directly onto
the clipboard ready to be pasted into your file.

                            RELATED SOFTWARE

Related applications available from Vapour Technology are Twister, which
constructs axis-angle rotations, and SpinDoctor, a 3D graphics calculator.

                           CONTACT INFORMATION

Vapour Technology can be reached on the web at http://www.vapourtech.com.

For bug reports and enquiries about this product, email support@vapourtech.com.
