PLATOTermJR
===========

A smaller version of PLATOTerm specifically for the PCjr and Tandy 1000, using its 320x200x16 color mode.

BUILD NOTES
-----------

Since this is a special version of PLATOTerm PC meant for exotic MS-DOS machines, These build flags affect how PLATOTermJR is built:

Machine Types:
==============

'''IMPORTANT: ONLY ONE OF THESE FLAGS MUST BE SET!'''

* OLIVETTI=1  - Build for Olivetti machines
* PCJR=1 - Build for PCjr or Tandy 1000 machines

OLIVETTI=1 should also have MONO=1 set, as the graphics mode is mono only.