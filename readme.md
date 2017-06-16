wav2png
=======

a simple converter that can turn wav files into pngs and then back to wave files.

building
--------

this project uses cmake. the easiest way to build it is to install cmake and then run

```bash
cmake .
make
```

usage
-----

to convert my_audio.wav to a .png:

```bash
wav2png bw my_audio.wav
```

(will create output.png in the working directory)

to convert my_image.png to a .wav:

```bash
png2wav bw my_image.png
```

(will create output.wav in the working directory)

by default png2wav will assume a sample rate of 44100Hz. to change this, supply the sample rate as a second argument:

```bash
png2wav bw my_image.png 96000
```

libraries
---------

wav2png/png2wav depends upon the following public domain libraries:

- [stb_image.h & stb_image_writer.h](https://github.com/nothings/stb)
- [dr_wav.h](https://github.com/mackron/dr_libs)

license
-------

wav2png/png2wav is available under a permissive zlib/libpng license.
