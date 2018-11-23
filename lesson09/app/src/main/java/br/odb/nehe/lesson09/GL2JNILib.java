package br.odb.nehe.lesson09;

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

    public static native void setTexture(Bitmap bitmap, Bitmap detail);

    public static native void toggleTwinkling();

    public static native void zoomIn();

    public static native void speedUpTwist();

    public static native void zoomDown();

    public static native void speedDownTwist();

    public static native void reset();
}
