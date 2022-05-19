package br.odb.nehe.lesson04

import android.content.res.AssetManager

// Wrapper for native library
internal object GL2JNILib {

	/**
	 * @param width  the current view width
	 * @param height the current view height
	 */
	external fun init(width: Int, height: Int)
	external fun step()
	external fun tick()
	external fun onDestroy()
	external fun onCreate(assetManager: AssetManager?)

	init {
		System.loadLibrary("NdkGlue")
	}
}