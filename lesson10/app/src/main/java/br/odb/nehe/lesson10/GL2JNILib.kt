package br.odb.nehe.lesson10

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
	external fun setTexture(bitmap: Array<Bitmap?>?)
	external fun reset()
	external fun moveForward()
	external fun turnRight()
	external fun moveBackward()
	external fun turnLeft()
	external fun onTouchNormalized(x: Float, y: Float)

	init {
		System.loadLibrary("NdkGlue")
	}
}