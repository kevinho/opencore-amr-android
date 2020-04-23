package io.kvh.media.amr;

/**
 * Created by kv.h on 14/11/21.
 */
public class AmrEncoder {


    /**
     * http://read.pudn.com/downloads466/doc/fileformat/1956393/AMR%20format.pdf
     CMR MODE FRAME SIZE( in bytes )
     0 AMR 4.75 13
     1 AMR 5.15 14
     2 AMR 5.9 16
     3 AMR 6.7 18
     4 AMR 7.4 20
     5 AMR 7.95 21
     6 AMR 10.2 27
     7 AMR 12.2 32
     */

    public enum Mode {
        MR475,/* 4.75 kbps */
        MR515,    /* 5.15 kbps */
        MR59,     /* 5.90 kbps */
        MR67,     /* 6.70 kbps */
        MR74,     /* 7.40 kbps */
        MR795,    /* 7.95 kbps */
        MR102,    /* 10.2 kbps */
        MR122,    /* 12.2 kbps */
        MRDTX,    /* DTX       */
        N_MODES   /* Not Used  */
    }

    public static native void init(int dtx);

    public static native int encode(int mode, short[] in, byte[] out);

    public static native void reset();

    public static native void exit();

    static {
		System.loadLibrary("amr-codec");
	}
}
