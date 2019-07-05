package br.odb.nehe.lesson09;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Toast;

import java.io.IOException;

public class GL2JNIActivity extends Activity {

    private GL2JNIView mView;
    private boolean running = false;
    private static AssetManager assets;

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        switch( keyCode ) {
            case KeyEvent.KEYCODE_T:
                GL2JNILib.toggleTwinkling();
                return true;

            case KeyEvent.KEYCODE_DPAD_UP:
                GL2JNILib.zoomIn();
                return true;

            case KeyEvent.KEYCODE_DPAD_RIGHT:
                GL2JNILib.speedUpTwist();
                return true;

            case KeyEvent.KEYCODE_DPAD_DOWN:
                GL2JNILib.zoomDown();
                return true;

            case KeyEvent.KEYCODE_DPAD_LEFT:
                GL2JNILib.speedDownTwist();
                return true;

            case KeyEvent.KEYCODE_R:
                GL2JNILib.reset();
                return true;
        }

        return super.onKeyDown(keyCode, event);
    }

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

        assets = getAssets();
        GL2JNILib.onCreate(assets);

        Bitmap bitmap;
        Bitmap detail;
        try {
            bitmap = BitmapFactory.decodeStream(assets.open("star.png"));
            detail = BitmapFactory.decodeStream(assets.open("twinkle.png"));
            GL2JNILib.setTexture(bitmap, detail);
        } catch (IOException ignored) {
        }

        mView = new GL2JNIView(getApplication());
        setContentView(mView);

        mView.onResume();

        mView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                GL2JNILib.toggleTwinkling();
                Toast.makeText(GL2JNIActivity.this, "Toggling twinkling", Toast.LENGTH_SHORT).show();
            }
        });

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
