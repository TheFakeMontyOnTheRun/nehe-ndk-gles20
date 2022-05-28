package br.odb.nehe.lesson10

import android.app.Activity
import android.graphics.BitmapFactory
import android.view.KeyEvent
import android.view.MotionEvent
import java.io.IOException

class GL2JNIActivity : Activity() {

	private var mView: GL2JNIView? = null
	private var running = false
	private var fingerX = 0f
	private var fingerY = 0f
	private var touching = false

	override fun onKeyDown(keyCode: Int, event: KeyEvent): Boolean {
		when (keyCode) {
			KeyEvent.KEYCODE_T -> return true
			KeyEvent.KEYCODE_DPAD_UP -> {
				GL2JNILib.moveForward()
				return true
			}
			KeyEvent.KEYCODE_DPAD_RIGHT -> {
				GL2JNILib.turnRight()
				return true
			}
			KeyEvent.KEYCODE_DPAD_DOWN -> {
				GL2JNILib.moveBackward()
				return true
			}
			KeyEvent.KEYCODE_DPAD_LEFT -> {
				GL2JNILib.turnLeft()
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
		try {
			GL2JNILib.setTexture(
				arrayOf(
					BitmapFactory.decodeStream(assets.open("mud.png")),
					BitmapFactory.decodeStream(assets.open("bricks.png")),
					BitmapFactory.decodeStream(assets.open("grass.png"))
				)
			)
		} catch (ignored: IOException) {
		}
		mView = GL2JNIView(application)
		setContentView(mView)
		mView!!.setOnTouchListener { v, event ->
			when (event.action) {
				MotionEvent.ACTION_UP -> {
					touching = false
					true
				}
				MotionEvent.ACTION_DOWN -> {
					touching = true
					fingerX = event.x / v.width
					fingerY = event.y / v.height
					true
				}
				MotionEvent.ACTION_MOVE -> {
					fingerX = event.x / v.width
					fingerY = event.y / v.height
					true
				}
				else -> false
			}
		}
		mView!!.onResume()
		Thread {
			running = true
			while (running) {
				try {
					Thread.sleep(20)
					if (touching) {
						GL2JNILib.onTouchNormalized(fingerX, fingerY)
					}
				} catch (e: InterruptedException) {
					e.printStackTrace()
				}
				GL2JNILib.tick()
			}
		}.start()
	}
}