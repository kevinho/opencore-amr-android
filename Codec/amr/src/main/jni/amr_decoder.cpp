#include <jni.h>
#include <interf_dec.h>
#include <string.h>

namespace amr_decode {

#ifndef _Included_com_hikvh_media_amr_AmrDecoder
#define _Included_com_hikvh_media_amr_AmrDecoder

#ifdef __cplusplus
    extern "C" {
#endif

    JNIEXPORT jlong JNICALL
    Java_io_kvh_media_amr_AmrDecoder_init(JNIEnv *env, jclass type) {
        return (jlong) Decoder_Interface_init();
    }

    JNIEXPORT void JNICALL
    Java_io_kvh_media_amr_AmrDecoder_exit(JNIEnv *env, jclass type, jlong state) {
        Decoder_Interface_exit((void *) state);
    }

    JNIEXPORT void JNICALL
    Java_io_kvh_media_amr_AmrDecoder_decode(JNIEnv *env, jclass, jlong state, jbyteArray in, jshortArray out) {

        jsize inLen = env->GetArrayLength(in);
        jbyte inBuf[inLen];
        env->GetByteArrayRegion(in, 0, inLen, inBuf);

        jsize outLen = env->GetArrayLength(out);
        short outBuf[outLen];

        Decoder_Interface_Decode((void *) state, (const unsigned char *) inBuf, (short *) outBuf);

        env->SetShortArrayRegion(out, 0, outLen, outBuf);
    }

#ifdef __cplusplus
    }
#endif
#endif

}
