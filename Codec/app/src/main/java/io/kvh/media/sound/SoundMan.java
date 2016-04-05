package io.kvh.media.sound;

import android.os.Handler;
import android.util.Log;

import io.kvh.media.amr.AmrDecoder;

/**
 * Created by changbinhe on 14/11/22.
 */
public class SoundMan implements Supporter.OnOffSwitcher {
    private static final String TAG = "Filer";
    private static final boolean DEBUG = true;

    private static final class SingletonHolder {
        public static final SoundMan INSTANCE = new SoundMan();
    }

    public static SoundMan getInstance() {
        return SingletonHolder.INSTANCE;
    }

    private boolean isRunning;
    private boolean initialized;
    private Handler handler;

    private SoundMan() {
        handler = new Handler();
    }

    @Override
    public void start() {
        if (DEBUG)
            Log.i(TAG, "try to start");
        if (isRunning) {
            Log.i(TAG, "already started");
            return;
        }

        if (!initialized) {
            if (DEBUG)
                Log.i(TAG, "try init");
            init();
            initialized = true;
            if (DEBUG)
                Log.i(TAG, "init succeed");
        }
        isRunning = true;

        recorder.start();
        codec.start();
        filer.start();
        uploader.start();
        if (DEBUG)
            Log.i(TAG, "start succeed");

        handler.removeCallbacksAndMessages(null);
        handler.postDelayed(sliceRunnable, Supporter.SLICE_SECOND * 1000);
    }

    Runnable sliceRunnable = new Runnable() {
        @Override
        public void run() {
            filer.nextSlice();
            handler.postDelayed(sliceRunnable,Supporter.SLICE_SECOND * 1000);
        }
    };

    @Override
    public void stop() {
        if (!isRunning)
            return;
        isRunning = false;
        handler.removeCallbacksAndMessages(null);

        //new slice
        recorder.stop();
        codec.stop();
        filer.stop();
    }

    private void init() {
        AmrDecoder.init();

        recorder = new Recorder();
        codec = new Codec();
        filer = new Filer();
        uploader = new Uploader();

        recorder.setPcmConsumer(codec);
        codec.setAmrConsumer(filer);
        filer.setFileConsumer(uploader);
    }

    private Codec codec;
    private Filer filer;
    private Recorder recorder;
    private Uploader uploader;

}
