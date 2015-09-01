opencore-amr-android
====================

一个Android平台下的amr编解码库，附带了将amr组装成文件的解决方案。

## 帮助QQ群：453503476

## 背景资料
- [opencore][1] 是一个Android的多媒体框架，最早是由PacketVideo贡献的。
- [AMR][2] 是Adaptive Multi-Rate编解码器的缩写，提供了较高的压缩率，主要用户声音传送编解码。
- [opencore-AMR][3] 是从opencore中抽取出来，用于amr与pcm的互相转换。

## 这是什么
- Opencoe-amr-android 是一个拿来即用的工具，免去了手动写c++ wrapper和ndk mk文件的繁琐。
- Demo 项目提供了一个把pcm->amr->file的方案。

## 使用
- Android Studio

  1. 在project的build.gradle中添加:
  ```gradle
  allprojects {
        repositories {
            jcenter()//0.8.0版之后的Android Studio默认为jcenter
        }
      }
  ```
  2. 在module的build.gradle中添加:
  ```gradle
  dependencies {
        compile fileTree(dir: 'libs', include: ['*.jar'])
        compile 'com.hikvh:opencore-amr-android:1.0.0'//这个地方使用依赖
  }
  ```

  或者: 拷贝如下文件夹的内容 [library](library/) 到你的module，如下图:

  ![Integration](screenshot/android_studio_integration.png)
> 也可以参考demo工程

- ADT(Eclipse)： 参照上图，拷贝对应文件即可。

## 调用API

``` java
    AmrEncoder.init(0);
    int mode = AmrEncoder.Mode.MR122.ordinal();
    short[] in;//长度160
    byte[] out = new byte[in.length];
    int byteEncoded = AmrEncoder.encode(mode, in, out);
    AmrEncoder.exit();
```
there you go.

## 将amr打包成文件
> record->encode->package amr file->upload(not implemented)
将音频流打包成文件切片，每个切片若干秒。
这个策略在如下场景实用：
在较为不好的网络环境下，需要录制较长的音频，上传到服务器，在服务器进行组装。

如果感兴趣，可以参照 [demo](demo/)

## FAQ
1. AmrEncoder.encode的第二份参数，接受一个数组，推荐为160个short，或者320个byte

  [1]: https://github.com/android/platform_external_opencore
  [2]: http://en.wikipedia.org/wiki/Adaptive_Multi-Rate_audio_codec
  [3]: http://opencore-amr.sourceforge.net/
