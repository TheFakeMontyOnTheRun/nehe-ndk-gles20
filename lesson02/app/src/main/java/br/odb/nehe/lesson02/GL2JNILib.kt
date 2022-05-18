package br.odb.nehe.lesson02

import android.content.res.AssetManager

internal object GL2JNILib {

	/**
	 * @param width the current view width
	 * @param height the current view height
	 */
	@JvmStatic
	external fun init(width: Int, height: Int)

	@JvmStatic
	external fun step()

	external fun onDestroy()
	external fun onCreate(assetManager: AssetManager?)

	init {
		System.loadLibrary("NdkGlue")
	}
}