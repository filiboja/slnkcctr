# slnkcctr

slnkcctr [slinky conductor] is a music toy that helps slinky jugglers to extend their performance with music.

In the current version, slnkcctr uses a camera connected to a computer to recognize position of slinky
and play simple sounds to the rhythm of slinky juggling steps.

Demonstration video: https://www.youtube.com/watch?v=QcneJYjU32M

## Setup

To build Release or Debug configuration of slnkcctr from source code, you need the following:

* Microsoft Visual C++ 2010
* [Boost](http://www.boost.org/)
    * Version: 1.55.0
    * Define `BOOST_ROOT` environment variable, e.g. "C:\local\boost_1_55_0"
	* Tutorial: [Getting Started on Windows](http://www.boost.org/doc/libs/1_55_0/more/getting_started/windows.html)
* [OpenCV](http://opencv.org/)
    * Version: 2.4.9
    * Define `OPENCV_DIR` environment variable, e.g. "C:\opencv\build\x86\vc10"
	* Add `%OPENCV_DIR%\bin` to system `PATH` variable
	* Tutorial: [Installation in Windows](http://docs.opencv.org/doc/tutorials/introduction/windows_install/windows_install.html#set-the-opencv-enviroment-variable-and-add-it-to-the-systems-path)
* SFML
    * Define `SFML` environment variable, e.g. "C:\local\SFML-2.1"
	* Add `%SFML%\bin` to system `PATH` variable

To build Profile configuration (only useful for profiling), you further need:

* [MicroProfiler](https://code.google.com/p/micro-profiler/)
    * Version: 1.1.567
    * Define `MICROPROFILER_DIR` environment variable, e.g. "C:\Program Files\gevorkyan.org\MicroProfiler"
    * Add `%MICROPROFILER_DIR%` to system `PATH` variable

After setting up all necessary libraries, proceed to build the application in Microsoft Visual C++.

Main Visual Studio project file: "slnkcctr.vcxproj"

## Usage

Once you have built "slnkcctr.exe", you can proceed to use it.

Run "slnkcctr.exe --help" for an overview of the available command line options.

Note that all of these options can also be used in configuration files.

You can find detailed overviews of command line options and configuration files
in the sections "Command line options", "General configuration files"
and "Detector configuration files".

### Calibration

To calibrate the slinky end detectors, run "slnkcctr.exe".
For each of the two slinky end detectors (called "0" and "1" by default),
you are presented two windows: "limits" and "video".

By default, there are the following four windows:

* For slinky end 0:
    * "0.limits"
    * "0.video"
* For slinky end 1:
    * "1.limits"
	* "1.video"

For each of the slinky ends, tweak the parameters in "limits" window
until the "video" window consistently shows a good estimate of the slinky end position.

Recommended process:

1. Set "Hue min" and "Hue max".
   Try to filter out as much of background as possible while keeping the slinky end in the foreground.
   Observe the corresponding "video" window to see the foreground and background division.
   Background is shown black and foreground is shown in color.
2. Do the same for "Sat min" and "Sat max".
3. Do the same for "Val min" and "Val max".
4. Set "Open size" to filter out noise in background
   (small patches of foreground enclosed by background).

In some conditions, it may be useful to also use "Crop" parameters to filter out borders of the image.

"Close size" parameter is present for completeness but hasn't been found useful in slnkcctr.
It may be used to filter out noise in foreground.

If you want to preserve the parameters for future usage,
record them in configuration files.
See details in section "Detector configuration files".

### Command line options

```
--config.filename <filename>
--config.filename "config.ini"
  Load the specified configuration file.

--source.mode device
--source.mode file
  Choose source of video stream. Choose "device" to use a camera.

--source.device <id>
--source.device 0
  Only works in "device" source mode.
  If more than one camera (or other capture device) is connected,
  use this option to choose the one you want to use.

--source.filename <filename>
--source.filename "recording.wmv"
  Only works in "file" source mode.
  For debugging purposes, you may want to use a video recording instead of a camera.
  Use this option to specify the filename of the recording.

--source.width <width>
--source.height <height>
--source.fps <fps>
--source.width 640
--source.height 480
--source.fps 15
  Use these options to attempt to override source frame width, height (in pixels) and rate (in frames per second).

--source.show <show?>
--source.show 1
--source.show 0
  Set to 1 to show the source video stream.

--detector.width <width>
--detector.height <height>
--detector.width 320
--detector.height 240
  Shrinking the video frames for the slinky end detectors may greatly improve performance.
  Use these options to set frame size for the detectors.

--detector.config.0 <filename>
--detector.config.1 <filename>
--detector.config.0 "magic\red.ini"
--detector.config.1 "magic\green.ini"
  Slinky end detectors may be configured using special detector configuration files (see details below).
  Use these options to set these configuration files.
  Note that only two files are supported (one for each of the two ends of the slinky).
```

### General configuration files

Unless another configuration file is specified using the command line option "config.filename",
slnkcctr tries to load the default configuration file "slnkcctr.ini".

General configuration files follow the INI configuration file convention,
i.e. you can set variable <variable> to value <value> by adding the following line to the configuration file:

```
<variable>=<value>
```

Configuration variable names match the command line option names without the leading "--".

An example configuration file is attached in "slnkcctr.ini".

### Detector configuration files

Detector configuration files also follow INI convention,
but have different variables available for configuration.
These variables parametrize the behavior of the detector.

```
name=<name>
name=slinky.red
  Name of the detector used to differentiate the two detectors in GUI.

window.video.show=<show?>
window.video.show=1
  Set to 1 to show the intermediate output of the detector.
  This is necessary for detector calibration.

window.limits.show=<show?>
window.limits.show=1
  Set to 1 to show the GUI for controlling the detector parameters during run-time.
  This is necessary for detector calibration.

color.hue.min=<hue min>
color.hue.max=<hue max>
color.sat.min=<saturation min>
color.sat.max=<saturation max>
color.val.min=<value min>
color.val.max=<value max>
color.hue.min=0
color.hue.max=179
color.sat.min=0
color.sat.max=255
color.val.min=0
color.val.max=255
  Color component limits that the detector uses to differentiate the slinky end from background.

morphological.open.size=<open size>
morphological.close.size=<close size>
morphological.open.size=1
morphological.close.size=1
  Parameters of the morphological operations used to clean up the area filtered by color filter.
```
