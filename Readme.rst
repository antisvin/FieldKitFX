What is this
============

This is unofficial fork of FieldKitFX firmware originally released by Koma Elektronik. Use at your own risk.

Firmware builds will be uploaded once new features will be added.

Building
========

It's configured as platformio project, so you should be able to built it yourself. See https://platformio.org/ for details. Currently is's configured to use SWDIO headers that have to be soldered, but it should also be possible to load firmware by DFU over USB cable.

License
=======

The code in this repository is licensed under the GNU General Public License v3.0.

UI changes
==========

This firmware decouples FX selection from looper control, so upper switch toggles looper control and FX selection modes. We also have more effects now.

Barberpole phaser
-----------------

This effect is a variation of phaser. It imitates "infinite" notch movemement going up or down.

    Led color
        Blue

    Control 1
        Effect amount (bipolar, so you can change rising and falling movement)

    Control 2
        Feedback amount

Thru-zero flanger
-----------------

Original analog effect was implemented by two identical tape reels, one of them was getting slowed down and sped up relative to the other.

    Led color
        Pink
    Control 1
        Modulation depth
    Control 2:
        Modulation rate

Phaser
------

A common effect based on two copies of signal with different phase offsets. Summing them gives us a moving distribution of notches due to parts of signals canceling each other.

    Led color
        Green
    Control 1
        Modulation depth before 12 o'clock, modulation rate above 12 o'clock
    Control 2
        Allpass filter coefficients selection - this changes phaser character considerably

Chorus
------

This effect is based on mixing of signal with its one or more delayed copies.

    Led color
        Orange
    Control 1
        When it's turned to less than 12 o'clock, a single sine LFO is used and this control changes modulation depth.
        After 12 o'clock it adds a second delay line with triangular LFO running at half frequency.
    Control 2
        Base rate for LFOs

Decimator
---------

This effect was present in original firmare and is mostly unchanged.

    Led color
        Red
    Control 1
        Samplerate reduction amount
    Control 2
        Bit depth reduction amount


Release history
===============

0.3.1
-----

* Replaced frequency shifter with barberpole phaser effect.
* Added more DSP effects

0.3.0
-----

* DSP effects are independent from looper and can be switch
* "Shift" menu will be used for settings, currenly you can switch effect type here with loop button or disable effect processing
* DSP effect is applied on signal before looper, which means that it would be recored by looper. Second effect slot would be added for wet looper signal later.
* Looper will always run and won't be reset by toggling the other menu

0.2.0
-----

* Rewrote most of the code from C to C++
* Deleted some unused files (ring buffer, debug)
* Fixed DC blocker - previously it wasn't removing DC

0.1.0
-----

* Rewritten all audio processing code to use floating point values
* Increased MCU frequency

0.0.1
-----

Initial version, no new features added - just migrated to platformio project and resolved a few build issues to get everything to work.
