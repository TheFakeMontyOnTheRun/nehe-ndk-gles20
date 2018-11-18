package br.odb.nehe.lesson02;

import android.content.res.AssetManager;

public class GL2JNILib {

    public static AssetManager assetManager;

     static {
         System.loadLibrary("NdkGlue");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);
    public static native void step();
    public static native void onDestroy();
    public static native void onCreate( AssetManager assetManager );
}
