package io.kvh.media.amr;

/**
 * Created by kv.h on 14/11/21.
 */
public class AmrDecoder {

	public static native long init();

	public static native void exit(long state);

	public static native void decode(long state, byte[] in, short[] out);

	static {
		System.loadLibrary("amr-codec");
	}
}
