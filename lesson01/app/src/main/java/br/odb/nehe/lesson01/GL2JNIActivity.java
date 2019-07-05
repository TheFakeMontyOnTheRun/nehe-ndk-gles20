package br.odb.nehe.lesson01;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;


public class GL2JNIActivity extends Activity {

    GL2JNIView mView;
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
        mView.onPause();
    }

    @Override protected void onResume() {
        super.onResume();
        mView.onResume();
    }

    @Override
    protected void onDestroy() {

        mView = null;
        super.onDestroy();
    }
}
