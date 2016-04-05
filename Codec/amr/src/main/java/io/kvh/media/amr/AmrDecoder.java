package io.kvh.media.amr;

/**
 * Created by kv.h on 14/11/21.
 */
public class AmrDecoder {

	public static native int init();

	public static native void exit(int state);

	public static native void decode(int state, byte[] in, short[] out);

	static {
		System.loadLibrary("amr-codec");
	}
}
