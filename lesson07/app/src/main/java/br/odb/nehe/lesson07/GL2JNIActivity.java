/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package br.odb.nehe.lesson07;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.WindowManager;

import java.io.File;
import java.io.IOException;


public class GL2JNIActivity extends Activity {

    GL2JNIView mView;
    boolean running = false;
    static AssetManager assets;

    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);

        assets = getAssets();
        GL2JNILib.onCreate(assets);

        Bitmap bitmap = null;
        try {
            bitmap = BitmapFactory.decodeStream(assets.open("crate0.png"));
            GL2JNILib.setTexture(bitmap);
        } catch (IOException e) {
        }

        mView = new GL2JNIView(getApplication());
        setContentView(mView);

        mView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                GL2JNILib.toggleLightningMode();
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();

        running = false;
        mView.onPause();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {

        switch ( keyCode ) {
            case KeyEvent.KEYCODE_L:
                GL2JNILib.toggleLightningMode();
                return true;
            case KeyEvent.KEYCODE_F:
                GL2JNILib.toggleFiltering();
                return true;
            case KeyEvent.KEYCODE_DPAD_LEFT:
                GL2JNILib.speedDownXZRotation();
                return true;
            case KeyEvent.KEYCODE_DPAD_RIGHT:
                GL2JNILib.speedUpXZRotation();
                return true;
            case KeyEvent.KEYCODE_DPAD_UP:
                GL2JNILib.speedDownYZRotation();
                return true;
            case KeyEvent.KEYCODE_DPAD_DOWN:
                GL2JNILib.speedUpYZRotation();
                return true;
            case KeyEvent.KEYCODE_R:
                GL2JNILib.reset();
                return true;
            default:
                return super.onKeyDown(keyCode, event);
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
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

    @Override
    protected void onDestroy() {

        GL2JNILib.onDestroy();

        super.onDestroy();
    }
}
