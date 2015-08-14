#include <jni.h>
#include <interf_dec.h>
#include <string.h>

namespace amr_decode{

#ifndef _Included_com_hikvh_media_amr_AmrDecoder
#define _Included_com_hikvh_media_amr_AmrDecoder

#ifdef __cplusplus
extern "C" {
#endif

int decode_init;
void* decode_state;


JNIEXPORT void JNICALL Java_com_hikvh_media_amr_AmrDecoder_init
  (JNIEnv *, jclass) {
  if(decode_init ++ != 0){
          return;
  }
  decode_state = Decoder_Interface_init();
}

JNIEXPORT void JNICALL Java_com_hikvh_media_amr_AmrDecoder_exit
  (JNIEnv *, jclass) {
  if(!decode_init){
          return;
   }
   decode_init = 0;
   Decoder_Interface_exit(decode_state);
}

JNIEXPORT jint JNICALL Java_com_hikvh_media_amr_AmrDecoder_decode
  (JNIEnv *env, jclass, jbyteArray in, jshortArray out){
  if(!decode_init){
          return 0;
  }

    jsize inLen = env->GetArrayLength(in);
    jbyte inBuf[inLen];
    env->GetByteArrayRegion(in, 0, inLen, inBuf);

    jsize outLen = env->GetArrayLength(out);
    short outBuf[outLen];

    Decoder_Interface_Decode(decode_state, (const unsigned char*) inBuf, (short*) outBuf);

    env->SetShortArrayRegion(out, 0, outLen, outBuf);
}
 
#ifdef __cplusplus
}
#endif
#endif

}