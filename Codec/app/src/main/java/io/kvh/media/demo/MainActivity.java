package io.kvh.media.demo;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.hikvh.media.demo.R;

import io.kvh.media.KCacheUtils;
import io.kvh.media.sound.SoundMan;

public class MainActivity extends Activity {
    boolean isRecording;
    boolean isDecoding;

    private AmrFileDecoder mAmrFileDecoder;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        KCacheUtils.init(this);
        setContentView(R.layout.activity_main);
    }

    public void onRecord(View view) {
        Button recordButton = (Button) view;
        if (isRecording) {
            SoundMan.getInstance().stop();
            recordButton.setText(R.string.record_start);
        } else {
            SoundMan.getInstance().start();
            recordButton.setText(R.string.record_stop);

            Toast.makeText(this, String.format("Check your file at: %s", KCacheUtils.getCacheDirectory() + "/record"), Toast.LENGTH_LONG).show();
        }

        isRecording = !isRecording;
    }

    public void onDecode(View view) {
        Button decodeButton = (Button) view;

        if (mAmrFileDecoder == null) {
            mAmrFileDecoder = new AmrFileDecoder();
        }

        if (!isDecoding) {
            mAmrFileDecoder.start(getResources().openRawResource(R.raw.demo));
            decodeButton.setText(R.string.decode_stop);
        } else {
            mAmrFileDecoder.stop();
            decodeButton.setText(R.string.decode_start);
        }

        isDecoding = !isDecoding;
    }
}
