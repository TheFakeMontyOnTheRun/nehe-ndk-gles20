package br.odb.nehe.lesson10;

import com.google.vrtoolkit.cardboard.CardboardView;
import com.google.vrtoolkit.cardboard.Eye;
import com.google.vrtoolkit.cardboard.HeadTransform;
import com.google.vrtoolkit.cardboard.Viewport;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by monty on 17/03/16.
 */
public class GL2JNIRenderer implements CardboardView.StereoRenderer {

	float[] forwardVector = new float[ 3 ];

	@Override
	public void onNewFrame(HeadTransform headTransform) {
		headTransform.getEulerAngles(forwardVector, 0);
		float xz = extractAngleXZFromHeadtransform(headTransform);
		GL2JNILib.setXZAngle( xz );
	}

	@Override
	public void onDrawEye(Eye eye) {
		float[] lookAt = eye.getEyeView();
		GL2JNILib.setPerspective( eye.getPerspective( 0.1f, 1000.0f ) );
		GL2JNILib.setLookAtMatrix( lookAt );
		GL2JNILib.step();
	}

	@Override
	public void onFinishFrame(Viewport viewport) {

	}

	@Override
	public void onSurfaceChanged(int width, int height) {
		GL2JNILib.init(width, height);
	}

	@Override
	public void onSurfaceCreated(EGLConfig eglConfig) {

	}

	@Override
	public void onRendererShutdown() {


	}

	private float extractAngleXYFromHeadtransform(HeadTransform headTransform) {
		return  360.0f - ((float)( forwardVector[ 2 ] * ( 180 / Math.PI ) ));
	}

	private float extractAngleYZFromHeadtransform(HeadTransform headTransform) {
		return  360.0f - ((float)( forwardVector[ 0 ] * ( 180 / Math.PI ) ));
	}

	private float extractAngleXZFromHeadtransform(HeadTransform headTransform) {
		return   ((float)( forwardVector[ 1 ] * ( 180 / Math.PI ) ));
	}
}
