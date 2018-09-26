PLATOTerm for the Tandy 2000 - 2018-09-26 build
===============================================

THIS IS AN EXPERIMENTAL BUILD OF PLATOTERM!

I HAVE NO IDEA IF IT EVEN WORKS! SERIOUSLY, WHO THE HELL KNOWS?!

With that said, if I haven't scared you off, some things to note:

This is a diet build, this means:

* No preferences
* Must use FOSSIL driver or BIOS to set serial port parameters (e.g. mode)
* I Do not know where the OT2K FOSSIL driver is, if someone finds it, please let me know so I can bundle it with the build
* You must have a Graphics option in your Tandy 2000, this means either color or mono, as the graphics mode is 640x400. The switches to select are defined below.

Graphics mode switches
======================
/COLOR - Use Mode 0x08 - 640x400x16 (sorta)
/MONO - Use Mode 0x09 - 640x400x2

As FOSSIL and BIOS use the same calls for what we need, it should work with
or without a FOSSIL driver, but obviously much better with one.

Please let me know if this works at all, raise an issue!

