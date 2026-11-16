# Canon EOS 350D add-on features

The EOS 350D (Digital Rebel XT, Kiss Digital N) is an ancient Canon DSLR,
released in 2005. It is over 20 years old.

This firmware add-on has features that Canon left out of the 350D (ISO 3200,
spot metering, longer bracketing, a timelapse, presets). It runs alongside
Canon's firmware - not replacing it.

This github repository is an archive of firmware tweaks from an old forum thread, [350d boot from cf card](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card), developed by CHDK forum users circa 2009-2014.

Not affiliated with Canon.

> **Warning:** this works only on 350D firmware **1.0.3**.  Installing
> alternative firmwares may void your camera’s warranty (though for a
> 2005 model, it’s likely expired). People have
> permanently bricked their cameras with this, mostly by running it on other
> firmware versions, but occasionally on 1.0.3 as well. Use it at your own
> risk.

There is no support available for this software.

## History

The 350D's firmware was written on a different OS (VxWorks) to that what modern
Canon DSLRs use.

This git reconstruction attempts to track the messy development history as
found on the forum. Every commit is a release posted between 2009 and 2014,
credited to the developer who posted it, dated to the post, and linked to
it.

```
     ┌────────────────────────────┐                    ┌─────────────────────┐
     │ 2008–2009  400D groundwork │                    │ 2009  Magic Lantern │
     │        (CHDK forum)        │                    │ (5D Mark II, DryOS) │
     └──────────────┬─────────────┘                    └──────────┬──────────┘
              ┌─────┴───────────────────────┐                     │
   maiohwmai  │                             │                     │
   ports it   ▼                             ▼                     │
╔═══════════════════════════╗   ┌──────────────────────┐  memspy  │
║         THIS REPO         ║   │ 2011  400plus (400D) │◄─────────┤
║ Canon EOS 350D, 2009–2014 ║   └──────────────────────┘  (2011)  │
╚═════════════╤═════════════╝               ▲                     │
              │ bundled                     │ screen drawing      │
              ▼                             │ (2012)              │
 ┌───────────────────────┬─╮     ┌──────────┴──────────┐  drawing │
 │ 2010  Max's blog post └─┤     │ 5dplus (5D Classic) │◄─────────┤
 │      chdkEOS350.zip     │     │        Coutts       │  code    │
 └~~~~~~~~~~~~~~~~~~~~~~~~~┘     └──────────┬──────────┘          │
              │ mirrored                    │                     ▼
              ▼                             └─────► Magic Lantern 5DC port
 ┌───────────────────────┬─╮                        (5DC.111, unmaintained)
 │ 2024  Christian Ege's └─┤
 │   Linux install guide   │
 │      + eos-bootable     │
 └~~~~~~~~~~~~~~~~~~~~~~~~~┘
```

The task of mapping out the timeline and turning it into a git repo was
assisted by AI.

## CHDK?
This code isn't CHDK. It sometimes gets called "CHDK for the 350D", but
that's wrong - this code is different.

The reason for the name, is the thread was simply hosted on the CHDK forum,
and the authors compiled using the ARM toolchain that CHDK used.

# Installation

There are two programs, one per folder:
- `bootflag`: enables booting from the CF card
- `350d`: the camera features

## Installing a Binary

These steps are a summary of the thread. Read the thread and the
[CHDK wiki's 350D page](https://chdk.fandom.com/wiki/350D) before you
start.

1. Check that the camera reports firmware 1.0.3.
2. With a fully charged battery, copy `bootflag/bootflg2.fir` to the root
   of a freshly formatted FAT16 card and run *Firmware update*. A
   successful run flashes the LEDs in this order:
   1. blue: firmware 1.0.3 is present and the boot flag area looks as
      expected
   2. blue: the boot flag has been read
   3. red and blue together: the new boot flag is being written
   4. solid blue: the write succeeded

   A solid red light at step 1 or 2 means a check failed and nothing was
   written. A solid red light, or no light, at step 4 means the write
   failed. When any solid light appears, turn the camera off and remove
   the battery. Each successful run toggles the flag, so running it again
   returns the camera to its factory state.
3. Format the card again in the camera, then make it bootable by writing
   two strings into its boot sector:
   - FAT16: `EOS_DEVELOP` at offset `0x2B` and `BOOTDISK` at `0x40`
   - FAT32: `EOS_DEVELOP` at `0x47` and `BOOTDISK` at `0x5C`

   On Linux or macOS, `make_bootable.sh` from Christian Ege's
   [eos-bootable](https://github.com/graugans/eos-bootable) writes these
   for both FAT16 and FAT32. Mount the card, then run the script as root:
   it finds the card by the `EOS_DIGITAL` label the camera gives it. You
   can also pass the card's partition, e.g.
   `sudo ./make_bootable.sh /dev/sdb1`. The script writes to the device
   with `dd`, so check the device name; a wrong one corrupts that disk. See
   its README for building the helper it uses.

   On Windows, CardTricks 1.45 can do this for FAT16 cards; FAT32 cards
   need it done by hand (see post #417).
4. Copy `AUTOEXEC.BIN` to the root of the card: either one you built, or
   a prebuilt one from `releases/`. The newest prebuilt one,
   `releases/2014-04-02-gezus-350D-14012014_GZS/`, also needs its
   `config/`, `presets/` and `settings` copied alongside it. Power on: the
   red and blue LEDs flash when it loads.

# The Code

## Build Chain

A build from a current toolchain hasn't been run on a camera yet. The
`350d` Makefile uses the GNU Arm Embedded toolchain (`arm-none-eabi`), and
builds cleanly with GCC 13.2. On Debian or Ubuntu:

```sh
sudo apt install --no-install-recommends gcc-arm-none-eabi make
```

No C library is needed: the code calls the camera firmware's own
routines.

The forum releases were built with an `arm-elf` cross-compiler, GCC 3.4.6
with binutils. In the forum thread it was distributed as a Windows
package, `gcc346.zip`, with each release unpacked into its `myproject/`
folder. That package hasn't survived; with a copy of it,
`make CROSS=arm-elf-` builds with it.

The two toolchains generate different code for calls into the firmware.
GCC 3.4.6 builds call addresses like `0xFF810000` with a direct `BL`,
which reaches them by wrapping around the 32-bit address space. Current
binutils doesn't wrap, so it routes each call through a two-word stub
that jumps to the same address, adding about 2.5 KB.

## `350d`:

`AUTOEXEC.BIN` is ARM code, built from the C and assembly sources in this
folder. Prebuilt copies from each release are in `releases/`. At power-on
the camera's Canon firmware loads it from the bootable card into memory
and runs it alongside Canon's own code. It adds features by calling
Canon's internal routines, which are listed by address in `funclist.txt`.
If you remove the card, or use one that isn't bootable, the camera
behaves as stock.

Most features are on the INFO screen: UP/DOWN chooses an item and
LEFT/RIGHT changes it. In the latest version they include:

- ISO 100–3200 in 1/3-stop steps, and colour-temperature white balance
- spot metering, and the custom functions without opening the menus
- extended AEB and EAEB, flash exposure bracketing, and flash exposure
  compensation up to ±6
- a timelapse, saved AF-point sets, review time and auto power-off time
- Set 3 picture parameters, image quality, B/W, and long-exposure noise
  reduction
- named presets stored on the card, and the shutter release count
- shortcuts from Canon's own menus (see `instructions and shortcuts.txt`)

You choose the features, menu order, language and shortcut signal at
build time in `user_settings.txt`.

### building

1. Edit `user_settings.txt`:
   - The active menu layout is the first block of `#if (1) … #else`;
     the second block is an alternative ("my settings") enabled by
     changing it to `#if (0)`.
   - Comment out a `#define POSITION_…` line to remove that item. Number
     the remaining items 1, 2, 3… in the order they should appear, with
     no gaps or repeats, and set `POSITION_LAST` to the highest number.
   - `INFOJUMPS` lists the items that repeated INFO presses jump to.
   - Above the layout: language (`ENGLISH`/`DEUTSCH`), shortcuts and
     their confirmation signal, and `USEPLAYFORSET`, which starts the
     timelapse and AF point sets with PLAY instead of SET (SET does
     nothing when custom function 1 is set to 0; see post #254).
2. Install the toolchain (see [Build Chain](#build-chain)) and run `make`
   to produce `AUTOEXEC.BIN`. It stops with an error, and deletes the
   file, if the binary is over the size limit below.
3. To keep a build, run `make release`. It copies `AUTOEXEC.BIN` into a
   new folder, `releases/<date>-<commit>/`, with `-dirty` added to the
   commit if there are uncommitted changes. If that folder already
   exists, it stops rather than overwrite it; commit, or wait a day, to
   release again.

The add-on reserves the top `ADDON_SIZE` bytes (32,000 by default) of
Canon's main heap, which Canon sets up from `0x200000` to `0x800000`. It
runs directly below `0x800000`, where the camera loads `AUTOEXEC.BIN`.
`init.c` copies it there at startup, and `entry.S` ends the heap where the
add-on begins, so the firmware never hands out its memory. A binary
larger than `ADDON_SIZE` would crash the camera (Err 99), so `make`
refuses to produce one. To make room, build with
`make ADDON_SIZE=<bytes>` (a multiple of 4). `make check` confirms that
the link address, the copy and the heap's end all agree. Older builds ran
at `0x7F0000`, inside the heap. The reserved heap hasn't been tested on a
camera yet.

#### prebuilt binaries

`releases/<date>-<author>-<release>/` holds the build outputs each
release shipped: `AUTOEXEC.BIN`, plus `.o` and `.elf` files and card files
(`config/`, `presets/`, `settings`) where the release included them. The
newest one, gezus's #477, was posted before the source in `350d/` (#482).
Every text string in that binary, including the 19 menu labels in the
order set by `user_settings.txt`, comes from the #482 source, so it was
very likely built from it. Without the original GCC 3.4.6 toolchain the
two can't be compared byte for byte, so rebuild from source if you need a
binary that is known to match the code.

Folders named `releases/<date>-<commit>/` are local builds from
`make release`, not forum releases.

## `bootflag`:

You run `bootflg2.fir` once from the camera's *Firmware update* menu. It
toggles a single boot flag in the camera's flash memory: after the first
run, the camera runs `AUTOEXEC.BIN` from a bootable CF card, and a second
run undoes the change. Before writing anything, it checks for firmware
1.0.3 and the flag's initial state. `ReadMe.txt` describes the LED signals
it uses to report each step. The tool adds no features of its own, and
it's the only part of the hack that writes to the camera's flash.

`autoexec.c`/`autoexec.bin` was a test program that only blinks the red and
blue LEDs, to confirm that the camera booted from the card

### building

Use the committed `bootflg2.fir`; don't rebuild it. This is the program
that writes to the camera's flash, and the committed file is the one
people have run on their cameras. Building it with modern toolchains is
untested.

The source, `bootflg2.c`, is kept for reference. Its Makefile is the
forum's original, written for the old `arm-elf` toolchain (GCC 3.4.6),
and `decrypt20D.exe`, a Windows tool, turned `bootflg2.bin` into
`bootflg2.fir`. On Windows, `ReadMe.txt` says to put the folder at
`gcc346\myproject\bootflg2\` and run `build.bat`. The Makefiles and
sources for the earlier `scanled` and `bootflag` versions are in their
own commits, for example
`git show maiohwmai/bootflag-toggle:bootflag/Makefile`.

## How the forum's history was captured into a Git repo

Each release is one commit on a branch named `<developer>/<release>`.
Each branch starts from `main` and is merged back with a merge commit.
The release commit and its merge are both authored by the developer and
dated to the forum post. The message describes the intent of the release,
gives the post number and URL, and says where the archive was found.
Developers' email addresses aren't public, so they are left empty.

Each commit changes the tree the way the thread describes the development.
A release builds on the code its author says it came from. When the thread
shows that one version superseded another, the new files replace the old
ones by rename: scanled became bootflag, which became bootflg2, so only
bootflg2 is left at the tip.

`350d/` holds only source, so its history shows code changes between
releases. Each release's build outputs and card files go to its own folder
in `releases/`, which keeps every shipped binary available. The binary-only
releases (cyrustam's #120 and gezus's #477) live entirely in their release
folders and leave `350d/` untouched, because their source is lost or was
posted later. gezus's source-only post (#482) updates `350d/` without a
release folder.

| Date | Developer | Post | Branch | Release |
|---|---|---|---|---|
| 2009-09-14 | maiohwmai | [#1](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/1) | `maiohwmai/scanled` | scanled.fir, first CF boot |
| 2009-09-20 | maiohwmai | [#3](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/3) | `maiohwmai/iso3200` | ISO 3200, release count |
| 2009-09-21 | maiohwmai | [#5](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/5) | `maiohwmai/bootflag-toggle` | bootflag.fir |
| 2009-09-27 | maiohwmai | [#11](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/11) | `maiohwmai/iso-info-screen` | ISO on the INFO screen |
| 2010-01-02 | cyrustam | [#120](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/120) | `cyrustam/all-in-info` | all features in the INFO menu (binary only) |
| 2010-08-14 | crazyklaus | [#223](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/223) | `crazyklaus/350D-20100814` | full source, build-time options |
| 2010-10-11 | crazyklaus | [#302](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/302) | `crazyklaus/au_courant` | one user's custom build (not merged) |
| 2010-10-11 | crazyklaus | [#303](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/303) | `crazyklaus/350D-20101011` | flash exposure bracketing |
| 2010-11-24 | peabody | [#341](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/341) | `peabody/bootflg2` | bootflg2.fir with firmware check |
| 2012-09-16 | zestoi | [#410](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/410) | `zestoi/350D-20120916` | presets, cyrustam's features ported |
| 2014-04-02 | gezus | [#477](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/477) | `gezus/350D-14012014_GZS` | B/W, noise reduction (binary only) |
| 2014-04-11 | gezus | [#482](https://chdk.setepontos.com/d/4202-350d-boot-from-cf-card/482) | `gezus/2014-source` | 2014 source |

### Where the archives came from

- **The forum:** the original attachments (2009 releases and bootflg2) were
  recovered from the Wayback Machine. gezus's attachments are still on the
  forum, but you need to log in to download them.
- **Other hosts:** crazyklaus's 20100814, au_courant and 20101011 zips are
  still on Mediafire. zestoi's zip came from djism.com through the
  Wayback Machine.
- **cyrustam's binary:** it comes from `chdkEOS350.zip`, a bundle Max put
  together and published on
  [his blog](https://max-fun.de/2010/02/23/chdk-firmware-hack-eos-350d/)
  on 2010-02-23. Its `AUTOEXEC.BIN` was built on 2010-01-08 and matches
  the INFO menu described in post #120. It isn't known whether it is
  `350d_all_in_info` or `350d_all_in_infov2`.

### Releases that are missing

The following releases are lost, so they have no commit. Most of them are
mentioned in the commit messages around the gap they leave.

- **cyrustam:** spot metering (#20), custom functions (#59), extended AEB
  (#102), colour temperature (#113), shortcuts (#127), fast custom WB
  (#146), the `gcc346.zip` source (#153), Set 3 parameters (#274) and
  flash bracketing (#275)
- **MilanC:** the timelapse and AEB work in #202 and #208
- **crazyklaus:** 350D-ck_1.0.1 (#184), 350D-20100821 (#228), and the
  builds in #244, #250 and #295

## Origins

The 350D code is informed by earlier reverse-engineering work on the Canon
EOS 400D (Digital Rebel XTi), done by others on the same forum in 2008–2009:

- **owerlord** found that the 400D boots `AUTOEXEC.BIN` from a CF card
  with `EOS_DEVELOP` at `0x2B` and `BOOTDISK` at `0x40`
  ([d/1618](https://chdk.setepontos.com/d/1618), June 2008). They also
  published the project template this code follows: C with an assembly
  start-up, a `funclist.txt` of firmware addresses generated from an IDA
  Pro disassembly, and a Makefile that builds `AUTOEXEC.BIN` and `.fir`
  files ([d/1631](https://chdk.setepontos.com/d/1631)).
- **DataGhost** explained that on FAT32 cards the first string belongs at
  `0x47` ([d/1618](https://chdk.setepontos.com/d/1618)), and **Seklth**
  posted the full FAT16 and FAT32 offsets that maiohwmai's first post
  repeats ([d/2001](https://chdk.setepontos.com/d/2001), July 2008).
- **Seklth** established that `AUTOEXEC.BIN` is loaded at `0x800000`, the
  address `init.c` copies from. They also developed the technique `entry.S`
  and `gui.S` use: looked at the start-up code and adding
  hooks to it ([d/1631](https://chdk.setepontos.com/d/1631)).
- **whim** modified CardTricks' `bootable.exe` to write the boot strings
  ([d/1618](https://chdk.setepontos.com/d/1618)).
- **thaibv**'s 400D hack, with ISO 3200, spot metering, shutter count and
  safety shift, is the direct ancestor. maiohwmai discussed adapting its
  `entry.S` for the 350D in the
  [400D thread](https://chdk.setepontos.com/d/3290) on the day of the
  350D bootflag release, which is why maiohwmai's `main.c` notes 400D
  addresses next to the 350D ones.

## Credits

- **maiohwmai:** the original boot-flag tools and firmware
- **cyrustam:** spot metering, custom functions, the INFO menu, EAEB, FEB
  and picture parameters
- **crazyklaus:** the configurable source tree and later releases
- **MilanC:** timelapse
- **peabody:** analysis of the boot-flag tools, and bootflg2
- **zestoi:** presets, and the ports of cyrustam's features
- **gezus:** the 2014 builds
- **Max** of max-fun.de: kept a copy of cyrustam's binary in his
  `chdkEOS350.zip` bundle
- **Christian Ege:** a 2024
  [guide to installing the hack from Linux](https://ch.ege.io/posts/2024/06/01/installing-chdk-firmware-on-a-canon-eos-350d-using-linux/),
  with his [eos-bootable](https://github.com/graugans/eos-bootable) tool
  for making cards bootable, and a mirror of `chdkEOS350.zip`
- **owerlord, Seklth, DataGhost, whim and thaibv:** the 400D groundwork
  described in [Origins](#origins)

## License

None of the authors stated a license, so copyright stays with each of
them.
