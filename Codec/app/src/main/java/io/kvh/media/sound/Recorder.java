package io.kvh.media.sound;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.util.Log;

/**
 * Created by changbinhe on 14/11/22.
 */
public class Recorder implements Runnable, Supporter.OnOffSwitcher {

    private static String TAG = "AudioRecorder";

    //8 k * 16bit * 1 = 8k shorts
    static final int SAMPLE_RATE = 8000;
    //20 ms second
    //0.02 x 8000 x 2 = 320;160 short
    static final int FRAME_SIZE = 160;

    private AudioRecord audioRecord;

    private short[] audioBuffer;

    private Thread runningThread;

    boolean isRecording;

    Supporter.PcmConsumer pcmConsumer;

    public void setPcmConsumer(Supporter.PcmConsumer pcmConsumer) {
        this.pcmConsumer = pcmConsumer;
    }

    @Override
    public void start() {
        if (isRecording) {
            Log.e(TAG, "is recoding");
            return;
        }
        int bufferSizeInByte = AudioRecord.getMinBufferSize(SAMPLE_RATE,
                AudioFormat.CHANNEL_CONFIGURATION_MONO,
                AudioFormat.ENCODING_PCM_16BIT);
        audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC,
                SAMPLE_RATE,
                AudioFormat.CHANNEL_CONFIGURATION_MONO,
                AudioFormat.ENCODING_PCM_16BIT,
                bufferSizeInByte);

        audioBuffer = new short[bufferSizeInByte / 2];

        audioRecord.startRecording();

        isRecording = true;

        //start
        runningThread = new Thread(this);
        runningThread.start();
    }

    @Override
    public void stop() {
        if (!isRecording)
            return;

        isRecording = false;
        runningThread.interrupt();
        runningThread = null;
        audioRecord.release();
    }

    @Override
    public void run() {
        while (isRecording) {
            int read = audioRecord.read(audioBuffer, 0, FRAME_SIZE);
            if (read == AudioRecord.ERROR_INVALID_OPERATION || read == AudioRecord.ERROR_BAD_VALUE) {
                Log.i(TAG, "error:" + read);
                continue;
            }

            pcmConsumer.onPcmFeed(audioBuffer, read);
        }
    }
}
