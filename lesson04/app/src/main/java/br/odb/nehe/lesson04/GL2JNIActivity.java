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

package br.odb.nehe.lesson04;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

import java.io.File;


public class GL2JNIActivity extends Activity {

    GL2JNIView mView;
    boolean running = false;
    static AssetManager assets;
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
