package br.odb.nehe.lesson05

import android.app.Activity
import android.os.Bundle

class GL2JNIActivity : Activity() {

	private var mView: GL2JNIView? = null

	private var running = false

	override fun onCreate(icicle: Bundle?) {
		super.onCreate(icicle)
		GL2JNILib.onCreate(assets)
		mView = GL2JNIView(application)
		setContentView(mView)
	}

	override fun onPause() {
		super.onPause()
		running = false
		mView!!.onPause()
	}

	override fun onResume() {
		super.onResume()
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

	override fun onDestroy() {
		GL2JNILib.onDestroy()
		super.onDestroy()
	}
}