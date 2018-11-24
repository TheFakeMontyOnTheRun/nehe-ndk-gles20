package br.odb.nehe.lesson07;

import android.app.Activity;
import android.content.res.AssetManager;
import android.graphics.BitmapFactory;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import java.io.IOException;


public class GL2JNIActivity extends Activity {

	private GL2JNIView mView;
	private boolean running = false;
	private static AssetManager assets;

	@Override
	protected void onPause() {
		super.onPause();

		running = false;
		mView.onPause();

		GL2JNILib.onDestroy();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {

		switch (keyCode) {
			case KeyEvent.KEYCODE_L:
				GL2JNILib.toggleLightningMode();
				return true;
			case KeyEvent.KEYCODE_F:
				GL2JNILib.toggleFiltering();
				return true;
			case KeyEvent.KEYCODE_DPAD_LEFT:
				GL2JNILib.speedDownXZRotation();
				return true;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
				GL2JNILib.speedUpXZRotation();
				return true;
			case KeyEvent.KEYCODE_DPAD_UP:
				GL2JNILib.speedDownYZRotation();
				return true;
			case KeyEvent.KEYCODE_DPAD_DOWN:
				GL2JNILib.speedUpYZRotation();
				return true;
			case KeyEvent.KEYCODE_R:
				GL2JNILib.reset();
				return true;
			default:
				return super.onKeyDown(keyCode, event);
		}
	}

	@Override
	protected void onResume() {
		super.onResume();


		assets = getAssets();
		GL2JNILib.onCreate(assets);

		try {
			GL2JNILib.setTextures(BitmapFactory.decodeStream(assets.open("metal.png")), BitmapFactory.decodeStream(assets.open("hexa.png")));
		} catch (IOException ignored) {
		}

		mView = new GL2JNIView(getApplication());
		setContentView(mView);

		mView.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				GL2JNILib.toggleFiltering();
				Toast.makeText(GL2JNIActivity.this, "Toggling filtering", Toast.LENGTH_SHORT).show();
			}
		});

		mView.setOnLongClickListener(new View.OnLongClickListener() {
			@Override
			public boolean onLongClick(View v) {
				GL2JNILib.toggleLightningMode();
				Toast.makeText(GL2JNIActivity.this, "Toggling lightning", Toast.LENGTH_SHORT).show();
				return true;
			}
		});

		mView.setOnTouchListener(new View.OnTouchListener() {
			@Override
			public boolean onTouch(View v, MotionEvent event) {

				float halfXRes = (v.getWidth() / 2.0f);
				float halfYRes = (v.getHeight() / 2.0f);
				float deltaX = halfXRes - event.getX();
				float deltaY = halfYRes - event.getY();

				if (Math.abs(deltaX) > halfXRes / 2.0f || Math.abs(deltaY) > halfYRes / 2.0f) {
					GL2JNILib.dragTo(deltaX, deltaY);
					return true;
				} else {
					return false;
				}
			}
		});

		Toast.makeText(GL2JNIActivity.this, "Besides the keyboard, you can also touch the cube to toggle filtering, long touch to toggle lightning and touch screen boarders to controls spinning", Toast.LENGTH_SHORT).show();

		mView.onResume();

		new Thread(new Runnable() {
			@Override
			public void run() {
				running = true;
				while (running) {
					try {
						Thread.sleep(20);
					} catch (InterruptedException e) {
						e.printStackTrace();
					}
					GL2JNILib.tick();
				}
			}
		}).start();
	}
}
