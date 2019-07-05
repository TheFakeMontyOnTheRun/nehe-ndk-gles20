package br.odb.nehe.lesson10;

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

    public static native void setTexture(Bitmap[] bitmap);

    public static native void reset();

    public static native void moveForward();

    public static native void turnRight();

    public static native void moveBackward();

    public static native void turnLeft();

    public static native void onTouchNormalized(float x, float y);
}
