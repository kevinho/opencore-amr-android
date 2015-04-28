opencore-AMR-Android
====================

An opencore amr codec JNI wrapper with explanation

## QQ tribe for help: 453503476

## Background
- [opencore][1] is the multimedia framework of android, which is a originallly contributed by PacketVideo.
- [AMR][2] is abbreviation for Adaptive Multi-Rate audio codec, which is an audio compression format optimized for speech coding.
- [opencore-AMR][3] is extracted from opencore as an codec for amr<->pcm encode/decode

## What Is This
- opencore-AMR-Android is a wrapper for handy usage. You can usage wrapped api in Android Application without any troubles of writing c++ wrapper or ndk mk file.

## Quick Start Up
- Down load the package.
- Put jni/libs/src folder to your Android Studio or ADT application project.
- Call api like this:

``` java
    AmrEncoder.init(0);
    int mode = AmrEncoder.Mode.MR122.ordinal();
    short[] in;//short array read from AudioRecorder
    byte[] out = new byte[in.length];
    int byteEncoded = AmrEncoder.encode(mode, in, out);
    AmrEncoder.exit();
```
there you go.

  [1]: https://github.com/android/platform_external_opencore
  [2]: http://en.wikipedia.org/wiki/Adaptive_Multi-Rate_audio_codec
  [3]: http://opencore-amr.sourceforge.net/
