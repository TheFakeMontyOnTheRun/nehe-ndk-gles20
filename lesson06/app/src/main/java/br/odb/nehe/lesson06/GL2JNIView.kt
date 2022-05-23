package br.odb.nehe.lesson06

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.Log
import br.odb.nehe.lesson06.GL2JNILib.init
import br.odb.nehe.lesson06.GL2JNILib.step
import javax.microedition.khronos.egl.EGL10
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.egl.EGLContext
import javax.microedition.khronos.egl.EGLDisplay
import javax.microedition.khronos.opengles.GL10

internal class GL2JNIView(context: Context?) : GLSurfaceView(context) {
	private fun init(depth: Int) {
		setEGLContextFactory(ContextFactory())
		setEGLConfigChooser(ConfigChooser(5, 6, 5, depth))
		setRenderer(Renderer())
	}

	private class ContextFactory : EGLContextFactory {
		override fun createContext(
			egl: EGL10,
			display: EGLDisplay,
			eglConfig: EGLConfig
		): EGLContext {
			Log.w(TAG, "creating OpenGL ES 2.0 context")
			checkEglError("Before eglCreateContext", egl)
			val attribList = intArrayOf(EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE)
			val context =
				egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attribList)
			checkEglError("After eglCreateContext", egl)
			return context
		}

		override fun destroyContext(egl: EGL10, display: EGLDisplay, context: EGLContext) {
			egl.eglDestroyContext(display, context)
		}

		companion object {
			private const val EGL_CONTEXT_CLIENT_VERSION = 0x3098
		}
	}

	private class ConfigChooser(
		val mRedSize: Int,
		val mGreenSize: Int,
		val mBlueSize: Int,
		val mDepthSize: Int
	) : EGLConfigChooser {
		override fun chooseConfig(egl: EGL10, display: EGLDisplay): EGLConfig {
			val numConfig = IntArray(1)
			egl.eglChooseConfig(display, s_configAttribs2, null, 0, numConfig)
			val numConfigs = numConfig[0]
			require(numConfigs > 0) { "No configs match configSpec" }
			val configs = arrayOfNulls<EGLConfig>(numConfigs)
			egl.eglChooseConfig(display, s_configAttribs2, configs, numConfigs, numConfig)
			return chooseConfig(egl, display, configs)!!
		}

		fun chooseConfig(
			egl: EGL10, display: EGLDisplay,
			configs: Array<EGLConfig?>
		): EGLConfig? {
			for (config in configs) {
				val d = findConfigAttrib(
					egl, display, config,
					EGL10.EGL_DEPTH_SIZE, 0
				)
				if (d < mDepthSize) continue
				val r = findConfigAttrib(
					egl, display, config,
					EGL10.EGL_RED_SIZE, 0
				)
				val g = findConfigAttrib(
					egl, display, config,
					EGL10.EGL_GREEN_SIZE, 0
				)
				val b = findConfigAttrib(
					egl, display, config,
					EGL10.EGL_BLUE_SIZE, 0
				)
				if (r == mRedSize && g == mGreenSize && b == mBlueSize) return config
			}
			return null
		}

		private fun findConfigAttrib(
			egl: EGL10, display: EGLDisplay,
			config: EGLConfig?, attribute: Int, defaultValue: Int
		): Int {
			return if (egl.eglGetConfigAttrib(display, config, attribute, mValue)) {
				mValue[0]
			} else defaultValue
		}

		private val mValue = IntArray(1)

		companion object {
			private const val EGL_OPENGL_ES2_BIT = 4
			private val s_configAttribs2 = intArrayOf(
				EGL10.EGL_RED_SIZE, 4,
				EGL10.EGL_GREEN_SIZE, 4,
				EGL10.EGL_BLUE_SIZE, 4,
				EGL10.EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL10.EGL_NONE
			)
		}
	}

	private class Renderer : GLSurfaceView.Renderer {
		override fun onDrawFrame(gl: GL10) {
			step()
		}

		override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
			init(width, height)
		}

		override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
			// Do nothing.
		}
	}

	companion object {
		private const val TAG = "Lesson06 NeHe View"
		private fun checkEglError(prompt: String, egl: EGL10) {
			var error: Int
			while (egl.eglGetError().also { error = it } != EGL10.EGL_SUCCESS) {
				Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error))
			}
		}
	}

	init {
		init(16)
	}
}