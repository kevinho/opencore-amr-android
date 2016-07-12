opencore-amr-android
====================

一个 Android 平台下的 amr 编解码库，附带了将 amr 组装成文件的解决方案。

## 帮助QQ群：453503476

## 背景资料
- [opencore][1] 是一个Android 的多媒体框架，最早是由 PacketVideo 贡献的。
- [amr][2] 是 Adaptive Multi-Rate 编解码器的缩写，提供了较高的压缩率，主要用户声音传送编解码。
- [opencore-amr][3] 是从 opencore 中抽取出来，用于 amr 与 pcm 的互相转换。

## 这是什么
- opencoe-amr-android 是一个拿来即用的工具，免去了手动写c++ wrapper和ndk mk文件的繁琐。
- app 项目提供了一个把pcm->amr->file的方案。

## 使用
### Android Studio

* 在project的build.gradle中添加:  

  ```
  allprojects {
        repositories {
            jcenter()
        }
      }
  ```
* 在module的build.gradle中添加:3. 
  
  ```
  dependencies {
        compile fileTree(dir: 'libs', include: ['*.jar'])
        compile 'io.kvh:amr:1.1.1'//这个地方使用依赖
  }
  ```

  或者: 拷贝如下文件夹的内容 [Code/amr/](Code/amr/) 到你的module，如下图:

  ![Integration](arts/android_studio_integration.png)
> 也可以参考 Code/amr/ 工程

- ADT(Eclipse)： 参照上图，拷贝对应文件即可。

## 调用API
* encode

``` 
AmrEncoder.init(0);

int mode = AmrEncoder.Mode.MR122.ordinal();
short[] in = new short[160];//short array read from AudioRecorder, length 160
byte[] out = new byte[32];//output amr frame, length 32
int byteEncoded = AmrEncoder.encode(mode, in, out);

AmrEncoder.exit();        
```
[Arm-2-File](https://github.com/kevinho/opencore-amr-android/blob/master/Codec/app/src/main/java/io/kvh/media/sound/Filer.java)

* decode

```
long state = AmrDecoder.init();

byte[] amrframe = new byte[32];//amr frame 32 bytes
short[] pcmframs = new short[160];//pcm frame 160 shorts
AmrDecoder.decode(state, amrframe, pcmframs);

AmrDecoder.exit(state);
```
[AmrFileDecoder.java](https://github.com/kevinho/opencore-amr-android/blob/master/Codec/app/src/main/java/io/kvh/media/demo/AmrFileDecoder.java)
## 基础知识
* Amr 文件有6字节的文件头："23 21 41 4D 52 0A" => "#!amr.", 在 decode frame 之前，记得先移除文件头。谢谢[AAccount 的讲解](https://github.com/kevinho/opencore-amr-android/issues/7).

## 将 amr 打包成文件
> record->encode->package amr file->upload(not implemented)
将音频流打包成文件切片，每个切片若干秒。
这个策略在如下场景实用：
在较为不好的网络环境下，需要录制较长的音频，上传到服务器，在服务器进行组装。

如果感兴趣，可以参照 [Codec/app](Codec/app)

## Changelog

#### v1.1.1 2016-04-06
- 修改 32bit 平台下 decode 的错误

### v1.1.0 2016-04-02
- 增加全平台的 prebuild
- 修改代码包名
- 整理代码

## FAQ
编辑FAQ文档，写上您使用上遇到的问题和解决方案，帮助后人，谢谢！赠人玫瑰，手留余香！

[FAQ_CN](FAQ_CN.md)

[FAQ](FAQ.md)

  [1]: https://github.com/android/platform_external_opencore
  [2]: http://en.wikipedia.org/wiki/Adaptive_Multi-Rate_audio_codec
  [3]: http://opencore-amr.sourceforge.net/
