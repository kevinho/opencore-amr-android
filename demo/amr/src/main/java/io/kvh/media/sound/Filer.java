package io.kvh.media.sound;

import android.util.Log;

import io.kvh.media.KCacheUtils;

import java.io.BufferedOutputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.UUID;

/**
 * Created by changbinhe on 14/11/22.
 */
public class Filer implements Runnable, Supporter.AmrConsumer, Supporter.OnOffSwitcher {
    private static final String TAG = "Filer";
    private static final boolean DEBUG = false;
    final private static byte[] header = new byte[]{0x23, 0x21, 0x41, 0x4D, 0x52, 0x0A};

    private List<byte[]> amrFrames;

    private DataOutputStream sliceStream;
    private File sliceFile;

    private Supporter.FileConsumer fileConsumer;

    private boolean isRunning;
    private Thread runningThread;
    final private Object waitingObject;

    public Filer() {
        amrFrames = Collections.synchronizedList(new LinkedList<byte[]>());
        waitingObject = new Object();
    }

    @Override
    public void onAmrFeed(byte[] buffer, int length) {
        byte[] tempData = new byte[length];
        System.arraycopy(buffer, 0, tempData, 0, length);

        amrFrames.add(tempData);

        if (DEBUG)
            Log.i(TAG, "add one amr frame, try to notify");

        synchronized (waitingObject) {
            waitingObject.notify();
        }
    }

    public void nextSlice() {
        if (DEBUG)
            Log.i(TAG, "next slice");
        makeSlice();
    }

    synchronized private void makeSlice() {
        //flush
        if (sliceStream != null) {
            try {
                sliceStream.flush();
                //send to file consumer
                fileConsumer.onFileFeed(sliceFile);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        File folder = new File(KCacheUtils.getCacheDirectory() + "/record");
        if (!folder.exists()) {
            folder.mkdirs();
        }
        File file = new File(folder.getAbsolutePath(), UUID.randomUUID().toString() + ".amr");
        if (file.exists())
            file.delete();
        try {
            file.createNewFile();
            Log.i(TAG, "new slice file at:" + file.getAbsolutePath());

        } catch (IOException e) {
            e.printStackTrace();
        }

        DataOutputStream dos = null;
        try {
            OutputStream os = new FileOutputStream(file);
            BufferedOutputStream bos = new BufferedOutputStream(os);
            dos = new DataOutputStream(bos);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        if (file.exists() && dos != null) {
            sliceFile = file;
            sliceStream = dos;

            try {
                sliceStream.write(header);
            } catch (IOException e) {
                e.printStackTrace();
            }

            if (DEBUG)
                Log.i(TAG, "file create succeed, try to notify");
            synchronized (waitingObject) {
                waitingObject.notify();
            }
            return;
        }

        Log.i(TAG, "file create failed");
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

        if (sliceStream == null)
            makeSlice();

        //start
        runningThread = new Thread(this);
        runningThread.start();
    }

    @Override
    public void stop() {
        if (DEBUG)
            Log.i(TAG, "stop, clean up");
        if (!isRunning) {
            Log.i(TAG, "not running");
            return;
        }

        isRunning = false;
        runningThread.interrupt();
        runningThread = null;

        //finish all writing
        if (sliceStream != null) {
            while (amrFrames.size() > 0) {
                byte[] buffer = amrFrames.remove(0);
                try {
                    //
                    if (DEBUG)
                        Log.i(TAG, "clean up write");
                    sliceStream.write(buffer, 0, buffer.length);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            try {
                sliceStream.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        sliceStream = null;
        sliceFile = null;
    }

    @Override
    public void run() {
        while (isRunning) {
            synchronized (waitingObject) {
                if (amrFrames.size() == 0 || sliceStream == null) {
                    if (DEBUG)
                        Log.i(TAG, "waiting :" + amrFrames.size() + sliceStream);
                    try {
                        waitingObject.wait();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                } else {
                    //try to write
                    byte[] buffer = amrFrames.get(0);
                    try {
                        if (DEBUG)
                            Log.i(TAG, "writing :" + buffer.length);
                        sliceStream.write(buffer, 0, buffer.length);
                        amrFrames.remove(0);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }

    public void setFileConsumer(Supporter.FileConsumer fileConsumer) {
        this.fileConsumer = fileConsumer;
    }

}
