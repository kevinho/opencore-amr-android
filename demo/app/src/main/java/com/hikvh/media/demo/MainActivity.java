package com.hikvh.media.demo;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.hikvh.media.KCacheUtils;
import com.hikvh.media.sound.SoundMan;

public class MainActivity extends Activity {

    Button recordButton;
    boolean isRecording;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        KCacheUtils.init(this);

        recordButton = (Button) findViewById(R.id.recordButton);
        recordButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                triggerButton();
            }
        });
    }

    private void triggerButton() {
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
}
