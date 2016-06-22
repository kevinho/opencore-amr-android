package io.kvh.media.demo;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;

import io.kvh.media.amr.AmrDecoder;

/**
 * Created by bugtags.com on 16/4/5.
 */
public class AmrFileDecoder implements Runnable {

    private Thread mDecodeThread;
    private AudioTrack mAudioTrack;

    private InputStream mInputStream;
    private long mDecoderState;

    byte[] readBuffer;
    byte[] readBufferWithoutCompress;

    int playerBufferSize = 0;
    // 8 k * 16bit * 1 = 8k shorts
    static final int SAMPLE_RATE = 8000;
    // 20 ms second
    // 0.02 x 8000 x 2 = 320;160 short
    static final int PCM_FRAME_SIZE = 160;
    static final int AMR_FRAME_SIZE = 32;

    boolean isRunning;

    public void start(InputStream inputStream) {
        if (isRunning) {
            return;
        }

        isRunning = true;

        mDecoderState = AmrDecoder.init();

        playerBufferSize = AudioTrack.getMinBufferSize(SAMPLE_RATE,
                AudioFormat.CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT);
        mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, SAMPLE_RATE,
                AudioFormat.CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT,
                playerBufferSize, AudioTrack.MODE_STREAM);
        readBuffer = new byte[AMR_FRAME_SIZE];
        readBufferWithoutCompress = new byte[PCM_FRAME_SIZE];

        mInputStream = inputStream;

        //amr file has 6 bytes header: "23 21 41 4D 52 0A" => "#!amr.", so skip here
        try {
            mInputStream.skip(6);
        } catch (IOException e) {
            e.printStackTrace();
        }
        mAudioTrack.play();
        mDecodeThread = new Thread(this);
        mDecodeThread.start();
    }

    public void stop() {
        if (!isRunning)
            return;

        mDecodeThread.interrupt();

        AmrDecoder.exit(mDecoderState);

        isRunning = false;

        mAudioTrack.stop();
    }

    @Override
    public void run() {
        try {
            while (isRunning && mInputStream.read(readBuffer) != -1) {
                // amr frame 32 bytes
                byte[] amrFrame = readBuffer.clone();
                // pcm frame 160 shorts
                short[] pcmFrame = new short[PCM_FRAME_SIZE];
                AmrDecoder.decode(mDecoderState, amrFrame, pcmFrame);

                // 播放
                mAudioTrack.write(pcmFrame, 0, pcmFrame.length);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
