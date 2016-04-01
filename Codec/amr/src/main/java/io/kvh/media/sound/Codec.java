package io.kvh.media.sound;

import android.util.Log;

import io.kvh.media.amr.AmrEncoder;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by changbinhe on 14/11/22.
 */
public class Codec implements Runnable, Supporter.PcmConsumer, Supporter.OnOffSwitcher {
    private static final String TAG = "Codec";
    private static final boolean DEBUG = false;

    private List<short[]> pcmFrames;
    private boolean isRunning;

    private Supporter.AmrConsumer amrConsumer;
    private Thread runningThread;
    final private Object waitingObject;

    public Codec() {
        pcmFrames = Collections.synchronizedList(new LinkedList<short[]>());
        waitingObject = new Object();
    }

    @Override
    public void onPcmFeed(short[] buffer, int length) {
        //would crash if not 160
        if (length != 160)
            return;
        short[] tempArray = new short[length];
        System.arraycopy(buffer, 0, tempArray, 0, length);

        if (DEBUG)
            Log.i(TAG, "onPcmFeed :" + length);
        pcmFrames.add(tempArray);

        if (DEBUG)
            Log.i(TAG, "onPcmFeed pcmFrames :" + pcmFrames.size());
        synchronized (waitingObject) {
            waitingObject.notify();
        }
    }

    public void setAmrConsumer(Supporter.AmrConsumer amrConsumer) {
        this.amrConsumer = amrConsumer;
    }

    @Override
    public void start() {
        if (DEBUG)
            Log.i(TAG, "try to start");
        if (isRunning) {

            Log.i(TAG, "already started");
            return;
        }
        if (DEBUG)
            Log.i(TAG, "start succeed");
        isRunning = true;

        AmrEncoder.init(0);

        //start
        runningThread = new Thread(this);
        runningThread.start();
    }

    @Override
    public void stop() {
        if (DEBUG)
            Log.i(TAG, "stop clean up");
        if (!isRunning) {
            Log.i(TAG, "not running");
            return;
        }

        isRunning = false;

        //todo need sync?
        //finish all
        while (pcmFrames.size() > 0) {
            short[] buffer = pcmFrames.remove(0);
            byte[] encodedData = new byte[buffer.length];
            int encodedLength = AmrEncoder.encode(AmrEncoder.Mode.MR122.ordinal(), buffer, encodedData);

            if (DEBUG)
                Log.i(TAG, "clean up encode: length" + encodedLength);
            if (encodedLength > 0) {
                amrConsumer.onAmrFeed(encodedData, encodedLength);
            }
        }
    }

    @Override
    public void run() {
        while (isRunning) {
            synchronized (waitingObject) {
                if (pcmFrames.size() == 0) {
                    try {
                        if (DEBUG)
                            Log.i(TAG, "wait: " + pcmFrames.size());
                        waitingObject.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                } else {
                    short[] buffer = pcmFrames.remove(0);
                    byte[] encodedData = new byte[buffer.length];
                    int encodedLength = AmrEncoder.encode(AmrEncoder.Mode.MR122.ordinal(), buffer, encodedData);
                    //Log.i(TAG, "encode: length" + encodedLength);
                    if (encodedLength > 0) {
                        amrConsumer.onAmrFeed(encodedData, encodedLength);
                    }
                }
            }
        }


        AmrEncoder.exit();
    }
}
