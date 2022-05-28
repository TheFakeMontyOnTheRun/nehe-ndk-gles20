package br.odb.nehe.lesson08

import android.app.Activity
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.view.KeyEvent
import android.widget.Toast
import java.io.IOException
import kotlin.math.abs

class GL2JNIActivity : Activity() {
	private var mView: GL2JNIView? = null
	private var running = false
	override fun onPause() {
		super.onPause()
		running = false
		mView!!.onPause()
		GL2JNILib.onDestroy()
	}

	override fun onKeyDown(keyCode: Int, event: KeyEvent): Boolean {
		return when (keyCode) {
			KeyEvent.KEYCODE_L -> {
				GL2JNILib.toggleLightningMode()
				true
			}
			KeyEvent.KEYCODE_F -> {
				GL2JNILib.toggleFiltering()
				true
			}
			KeyEvent.KEYCODE_DPAD_LEFT -> {
				GL2JNILib.speedDownXZRotation()
				true
			}
			KeyEvent.KEYCODE_DPAD_RIGHT -> {
				GL2JNILib.speedUpXZRotation()
				true
			}
			KeyEvent.KEYCODE_DPAD_UP -> {
				GL2JNILib.speedDownYZRotation()
				true
			}
			KeyEvent.KEYCODE_DPAD_DOWN -> {
				GL2JNILib.speedUpYZRotation()
				true
			}
			KeyEvent.KEYCODE_R -> {
				GL2JNILib.reset()
				true
			}
			KeyEvent.KEYCODE_B -> {
				GL2JNILib.toggleBlending()
				true
			}
			else -> super.onKeyDown(keyCode, event)
		}
	}

	override fun onResume() {
		super.onResume()
		GL2JNILib.onCreate(assets)
		val bitmap: Bitmap
		try {
			bitmap = BitmapFactory.decodeStream(assets.open("celestial.png"))
			GL2JNILib.setTexture(bitmap)
		} catch (ignored: IOException) {
		}
		mView = GL2JNIView(application)
		setContentView(mView)
		mView!!.setOnClickListener {
			GL2JNILib.toggleBlending()
			Toast.makeText(this@GL2JNIActivity, "Toggling blending", Toast.LENGTH_SHORT).show()
		}
		mView!!.setOnLongClickListener {
			GL2JNILib.toggleLightningMode()
			Toast.makeText(this@GL2JNIActivity, "Toggling lightning", Toast.LENGTH_SHORT).show()
			true
		}
		mView!!.setOnTouchListener { v, event ->
			val halfXRes = v.width / 2.0f
			val halfYRes = v.height / 2.0f
			val deltaX = halfXRes - event.x
			val deltaY = halfYRes - event.y
			if (abs(deltaX) > halfXRes / 2.0f || abs(deltaY) > halfYRes / 2.0f) {
				GL2JNILib.dragTo(deltaX, deltaY)
				true
			} else {
				false
			}
		}
		Toast.makeText(
			this@GL2JNIActivity,
			"Besides the keyboard, you can also touch the cube to toggle filtering, long touch to toggle lightning and touch screen boarders to controls spinning",
			Toast.LENGTH_SHORT
		).show()
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