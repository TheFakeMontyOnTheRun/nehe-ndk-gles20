package br.odb.nehe.lesson04;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;


public class GL2JNIActivity extends Activity {

    private GL2JNIView mView;
    private boolean running = false;
    private static AssetManager assets;
    @Override protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        assets = getAssets();
        GL2JNILib.onCreate( assets );
        mView = new GL2JNIView(getApplication());
        setContentView(mView);
    }

    @Override protected void onPause() {
        super.onPause();

        running = false;
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();

        new Thread(new Runnable() {
            @Override
            public void run() {
                running = true;
                while( running ) {
                    try {
                        Thread.sleep( 20 );
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    GL2JNILib.tick();
                }
            }
        }).start();
    }

    @Override
    protected void onDestroy() {

        GL2JNILib.onDestroy();

        super.onDestroy();
    }
}
