OpenCore-Amr-Android
====================

An opencore amr codec JNI wrapper with explanation,
And one solution for packaging amr audio files.

## QQ tribe for help: 453503476

## Background
- [opencore][1] is the multimedia framework of android, which is a originallly contributed by PacketVideo.
- [AMR][2] is abbreviation for Adaptive Multi-Rate audio codec, which is an audio compression format optimized for speech coding.
- [opencore-AMR][3] is extracted from opencore as an codec for amr<->pcm encode/decode

## What Is This
- opencore-AMR-Android is a wrapper for handy usage. You can usage wrapped api in Android Application without any troubles of writing c++ wrapper or ndk mk file.
- demo project offer a solution for packaging amr audio files, in elegant code structure.

## Quick Start Up
- Down load the library.
- Put jni/libs/src folder to your Android Studio or ADT application project.
- Call api like this:

``` java
    AmrEncoder.init();
    int mode = AmrEncoder.Mode.MR122.ordinal();
    short[] in;//short array read from AudioRecorder, recommend length 160
    byte[] out = new byte[in.length];
    int byteEncoded = AmrEncoder.encode(mode, in, out);
    AmrEncoder.exit();
```
there you go.

## Packaging amr audio to file system
> record->encode->package amr file->upload(not implemented)

Audio steam is packaged to file in slices, the slice last a few seconds(customizable). This policy is suitable for this scenario: client is under unstable mobile network, client records amr file, sends slice by slice, server re-assembles the slices.

If you are interested in this solution, please refer to [demo](demo/)


## FAQ
1. The second parameter of AmrEncoder.encode accept an array, it's recommended to 160 in short, or 320 in byte.

  [1]: https://github.com/android/platform_external_opencore
  [2]: http://en.wikipedia.org/wiki/Adaptive_Multi-Rate_audio_codec
  [3]: http://opencore-amr.sourceforge.net/
