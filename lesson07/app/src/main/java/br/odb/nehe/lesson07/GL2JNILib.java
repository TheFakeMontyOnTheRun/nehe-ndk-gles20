package br.odb.nehe.lesson07;

import android.content.res.AssetManager;
import android.graphics.Bitmap;

class GL2JNILib {

    public static AssetManager assetManager;

    static {
        System.loadLibrary("NdkGlue");
    }

    /**
     * @param width  the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);

    public static native void step();

    public static native void tick();

    public static native void onDestroy();

    public static native void onCreate(AssetManager assetManager);

    public static native void setTexture(Bitmap bitmap);

    public static native void toggleFiltering();

    public static native void toggleLightningMode();

    public static native void speedUpYZRotation();

    public static native void speedDownYZRotation();

    public static native void speedUpXZRotation();

    public static native void speedDownXZRotation();

    public static native void reset();

    public static native void dragTo(float x, float y);
}
