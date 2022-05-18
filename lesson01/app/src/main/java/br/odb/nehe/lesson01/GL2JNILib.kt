package br.odb.nehe.lesson01

import android.content.res.AssetManager

object GL2JNILib {

	/**
	 * @param width the current view width
	 * @param height the current view height
	 */
	@JvmStatic
	external fun init(width: Int, height: Int)

	@JvmStatic
	external fun step()

	external fun onCreate(assetManager: AssetManager?)

	init {
		System.loadLibrary("gl2jni")
	}
}