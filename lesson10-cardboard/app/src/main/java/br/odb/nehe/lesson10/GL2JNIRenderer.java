package br.odb.nehe.lesson10;

import android.opengl.GLES20;

import com.google.vrtoolkit.cardboard.CardboardView;
import com.google.vrtoolkit.cardboard.Eye;
import com.google.vrtoolkit.cardboard.HeadTransform;
import com.google.vrtoolkit.cardboard.Viewport;

import javax.microedition.khronos.egl.EGLConfig;

class GL2JNIRenderer implements CardboardView.StereoRenderer {

	private final float[] forwardVector = new float[3];

	@Override
	public void onNewFrame(HeadTransform headTransform) {
		headTransform.getEulerAngles(forwardVector, 0);
		float xz = extractAngleXZFromHeadtransform();
		GL2JNILib.setXZAngle(xz);
	}

	@Override
	public void onDrawEye(Eye eye) {
		GLES20.glEnable(GLES20.GL_DEPTH_TEST);
		float[] lookAt = eye.getEyeView();
		GL2JNILib.setPerspective(eye.getPerspective(0.1f, 1000.0f));
		GL2JNILib.setLookAtMatrix(lookAt);
		GL2JNILib.step();
	}

	@Override
	public void onFinishFrame(Viewport viewport) {}

	@Override
	public void onSurfaceChanged(int width, int height) {
		GL2JNILib.init(width, height);
	}

	@Override
	public void onSurfaceCreated(EGLConfig eglConfig) {}

	@Override
	public void onRendererShutdown() {}

	private float extractAngleXZFromHeadtransform() {
		return ((float) (forwardVector[1] * (180 / Math.PI)));
	}
}
