package br.odb.nehe.lesson09

import android.content.res.AssetManager
import android.graphics.Bitmap

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
	external fun setTexture(bitmap: Bitmap?, detail: Bitmap?)
	external fun toggleTwinkling()
	external fun zoomIn()
	external fun speedUpTwist()
	external fun zoomDown()
	external fun speedDownTwist()
	external fun reset()

	init {
		System.loadLibrary("NdkGlue")
	}
}