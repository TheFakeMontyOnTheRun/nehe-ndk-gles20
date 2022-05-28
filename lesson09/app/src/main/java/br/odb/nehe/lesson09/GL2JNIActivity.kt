package br.odb.nehe.lesson09

import android.app.Activity
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.view.KeyEvent
import android.widget.Toast
import java.io.IOException

class GL2JNIActivity : Activity() {
	private var mView: GL2JNIView? = null
	private var running = false
	override fun onKeyDown(keyCode: Int, event: KeyEvent): Boolean {
		when (keyCode) {
			KeyEvent.KEYCODE_T -> {
				GL2JNILib.toggleTwinkling()
				return true
			}
			KeyEvent.KEYCODE_DPAD_UP -> {
				GL2JNILib.zoomIn()
				return true
			}
			KeyEvent.KEYCODE_DPAD_RIGHT -> {
				GL2JNILib.speedUpTwist()
				return true
			}
			KeyEvent.KEYCODE_DPAD_DOWN -> {
				GL2JNILib.zoomDown()
				return true
			}
			KeyEvent.KEYCODE_DPAD_LEFT -> {
				GL2JNILib.speedDownTwist()
				return true
			}
			KeyEvent.KEYCODE_R -> {
				GL2JNILib.reset()
				return true
			}
		}
		return super.onKeyDown(keyCode, event)
	}

	override fun onPause() {
		super.onPause()
		running = false
		mView!!.onPause()
		GL2JNILib.onDestroy()
	}

	override fun onResume() {
		super.onResume()
		GL2JNILib.onCreate(assets)
		val bitmap: Bitmap
		val detail: Bitmap
		try {
			bitmap = BitmapFactory.decodeStream(assets.open("star.png"))
			detail = BitmapFactory.decodeStream(assets.open("twinkle.png"))
			GL2JNILib.setTexture(bitmap, detail)
		} catch (ignored: IOException) {
		}
		mView = GL2JNIView(application)
		setContentView(mView)
		mView!!.onResume()
		mView!!.setOnClickListener {
			GL2JNILib.toggleTwinkling()
			Toast.makeText(this@GL2JNIActivity, "Toggling twinkling", Toast.LENGTH_SHORT).show()
		}
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