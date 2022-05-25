package br.odb.nehe.lesson06

import android.app.Activity
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import java.io.IOException

class GL2JNIActivity : Activity() {
	private var mView: GL2JNIView? = null
	private var running = false
	override fun onPause() {
		super.onPause()
		running = false
		mView!!.onPause()
		GL2JNILib.onDestroy()
	}

	override fun onResume() {
		super.onResume()
		mView = GL2JNIView(application)
		setContentView(mView)
		GL2JNILib.onCreate(assets)
		val bitmap: Bitmap
		try {
			bitmap = BitmapFactory.decodeStream(assets.open("texture.png"))
			GL2JNILib.setTexture(bitmap)
		} catch (ignored: IOException) {
		}
		mView!!.onResume()
		Thread {
			running = true
			while (running) {
				try {
					Thread.sleep(20)
				} catch (e: InterruptedException) {
					e.printStackTrace()
				}
				GL2JNILib.tick()
			}
		}.start()
	}
}