
BOOTFLG2.FIR for the Canon 350D:


At the beginning of the 350D thread, maiohwmai posted two versions of the .fir file that is used to change the camera's boot flag, which in turn allows the camera to be given extra features.  The first version was scanled.fir, which simply turns the boot flag ON.  The second was bootflag.fir, which toggles the boot flag to its opposite state each time it is run.

Neither of these programs checks for the presence of firmware v1.0.3, and scanled.fir in particular has several other deficiences that may pose a danger of bricking the camera.  Maiohwmai hasn't been seen in the forum for some time, so I decided to revise the bootflag.fir program so that it checks for v1.0.3, and does other tests as the process goes forward, in an effort to make sure everything is as expected before anything in the camera is modified.

However, even this revised version calls the same two firmware subroutines that maiohwmai called, but I don't know what they do, or how reliable they are, so the possibility of damage to the camera still exists.  A lot of people have successfully run even the original programs (so long as firmware v1.0.3 is present).  But there are a few people who have bricked their cameras even though they say they had v1.0.3 installed.  So, until we find out exactly what those subroutines do, some small degree of risk may remain - even if you do everything right.  All I can say for sure is that this works on my 350D.


Move bootflg2.fir to the root directory of a freshly-formatted (FAT16) CF card, and go into the camera's firmware update routine  - all as described in the Forum or the Wiki.  If successful, format the card again and make it bootable with Cardtricks 1.45, put the autoexec.bin file into the root directory, and see if the lights blink when you power up.

A successful run of bootflg2.fir will produce the following sequence of blinking LEDs:

1. First blue flash:  this indicates that firmware v1.0.3 is present, and the initial state of the boot flag area is as expected.

2. Second blue flash:  The first mystery subroutine has been called to read the boot flag bytes into a buffer, and it appears to have worked.

3. Red and blue flashed together:  The program has called the second mystery subroutine to write the new boot flag value.

4. Solid blue light:  The write process appears to have succeeded.  If instead you see a solid red LED at this point, or no LED at all, the write operation has failed.

A solid red LED in place of either 1 or 2 above means the relevant test has failed, and the program has been halted, with no write attempted.

So to summarize, what you want to see is:

blue
blue
both
solid blue

After any solid light appears, turn off the camera and remove the battery.

Each time you go through this process successfully, you toggle the boot flag to the opposite state.  This can be used to return the camera to its factory state.

To make changes to the software, install gcc346, and extract all the bootflg2.zip contents to folder c:\gcc346\myproject\bootflg2\.  Run build.bat to compile.


Peabody
