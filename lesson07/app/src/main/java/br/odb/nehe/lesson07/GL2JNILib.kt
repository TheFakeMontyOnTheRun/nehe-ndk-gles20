package br.odb.nehe.lesson07

import android.content.res.AssetManager
import android.graphics.Bitmap

internal object GL2JNILib {

	/**
	 * @param width  the current view width
	 * @param height the current view height
	 */
	@JvmStatic
	external fun init(width: Int, height: Int)

	@JvmStatic
	external fun step()
	external fun tick()
	external fun onDestroy()
	external fun onCreate(assetManager: AssetManager?)
	external fun setTexture(bitmap: Bitmap?)
	external fun toggleFiltering()
	external fun toggleLightningMode()
	external fun speedUpYZRotation()
	external fun speedDownYZRotation()
	external fun speedUpXZRotation()
	external fun speedDownXZRotation()
	external fun reset()
	external fun dragTo(x: Float, y: Float)

	init {
		System.loadLibrary("NdkGlue")
	}
}