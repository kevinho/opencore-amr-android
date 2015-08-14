package com.hikvh.media.amr;

/**
 * Created by kv.h on 14/11/21.
 */
public class AmrDecoder {

    public static native void init();

    public static native void exit();

    public static native int decode(byte[] in, short[] out);

    static {
        System.loadLibrary("amr-codec");
    }
}
