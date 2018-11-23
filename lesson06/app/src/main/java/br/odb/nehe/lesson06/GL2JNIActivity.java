package br.odb.nehe.lesson06;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import java.io.IOException;

public class GL2JNIActivity extends Activity {

    private GL2JNIView mView;
    private boolean running = false;
    private static AssetManager assets;


    @Override
    protected void onPause() {
        super.onPause();

        running = false;
        mView.onPause();
        GL2JNILib.onDestroy();
    }

    @Override
    protected void onResume() {
        super.onResume();

        mView = new GL2JNIView(getApplication());
        setContentView(mView);

        assets = getAssets();
        GL2JNILib.onCreate(assets);


        Bitmap bitmap;
        try {
            bitmap = BitmapFactory.decodeStream(assets.open("texture.png"));
            GL2JNILib.setTexture(bitmap);
        } catch (IOException ignored) {
        }

        mView.onResume();

        new Thread(new Runnable() {
            @Override
            public void run() {
                running = true;
                while (running) {
                    try {
                        Thread.sleep(20);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    GL2JNILib.tick();
                }
            }
        }).start();
    }
}
