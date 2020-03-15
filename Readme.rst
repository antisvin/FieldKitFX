What is this
============

This is unofficial fork of FieldKitFX firmware originally released by Koma Elektronik. Use at your own risk.

Building
========

It's configured as platformio project, so you should be able to built it yourself. See https://platformio.org/ for details. Currently is's configured to use SWDIO headers that have to be soldered, but it should also be possible to load firmware by DFU over USB cable.

License
=======

The code in this repository is licensed under the GNU General Public License v3.0.

Release history
===============

0.1.0
-----

* Rewritten all audio processing code to use floating point values
* Increased MCU frequency

0.0.1
-----

Initial version, no new features added - just migrated to platformio project and resolved a few build issues to get everything to work.
