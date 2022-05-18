package br.odb.nehe.lesson02

import android.app.Activity
import android.os.Bundle

class GL2JNIActivity : Activity() {
	
	private var mView: GL2JNIView? = null

	override fun onCreate(icicle: Bundle?) {
		super.onCreate(icicle)
		GL2JNILib.onCreate(assets)
		mView = GL2JNIView(application)
		setContentView(mView)
	}

	override fun onPause() {
		super.onPause()
		mView!!.onPause()
	}

	override fun onResume() {
		super.onResume()
		mView!!.onResume()
	}

	override fun onDestroy() {
		GL2JNILib.onDestroy()
		super.onDestroy()
	}
}