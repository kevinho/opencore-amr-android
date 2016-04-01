package io.kvh.media.sound;

import android.util.Log;

import java.io.File;

/**
 * Created by changbinhe on 14/11/22.
 */
public class Uploader implements Supporter.FileConsumer, Supporter.OnOffSwitcher {
    private static final String TAG = "Uploader";

    @Override
    public void onFileFeed(File file) {
        Log.i(TAG, String.format("receive slice file:%s for %d seconds", file.getAbsolutePath(), Supporter.SLICE_SECOND));

        //send the file to your server in asyn way
    }

    @Override
    public void start() {

    }

    @Override
    public void stop() {

    }
}
