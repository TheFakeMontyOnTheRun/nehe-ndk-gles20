package br.odb.nehe.lesson10;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.view.KeyEvent;

import com.google.vrtoolkit.cardboard.CardboardActivity;

import java.io.IOException;

public class GL2JNIActivity extends CardboardActivity {

	private GL2JNIView mView;
	private boolean running = false;
	private static AssetManager assets;

	@Override
	public void onCardboardTrigger() {
		GL2JNILib.moveForward();
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {

		switch (keyCode) {
			case KeyEvent.KEYCODE_T:
				return true;

			case KeyEvent.KEYCODE_DPAD_UP:
				GL2JNILib.moveForward();
				return true;

			case KeyEvent.KEYCODE_DPAD_RIGHT:
				GL2JNILib.strafeRight();
				return true;

			case KeyEvent.KEYCODE_DPAD_DOWN:
				GL2JNILib.moveBackward();
				return true;

			case KeyEvent.KEYCODE_DPAD_LEFT:
				GL2JNILib.strafeLeft();
				return true;

			case KeyEvent.KEYCODE_BUTTON_L1:
				GL2JNILib.strafeLeft();
				break;

			case KeyEvent.KEYCODE_BUTTON_R1:
				GL2JNILib.strafeRight();
				break;


			case KeyEvent.KEYCODE_R:
				GL2JNILib.reset();
				return true;
		}

		return super.onKeyDown(keyCode, event);
	}

	@Override
	protected void onPause() {
		super.onPause();

		running = false;
		mView.onPause();

		GL2JNILib.onDestroy();
	}

	@Override
	protected void onResume() {
		super.onResume();

		assets = getAssets();
		GL2JNILib.onCreate(assets);

		try {
			GL2JNILib.setTexture(new Bitmap[]{
					BitmapFactory.decodeStream(assets.open("mud.png")),
					BitmapFactory.decodeStream(assets.open("bricks.png")),
					BitmapFactory.decodeStream(assets.open("grass.png"))
			});

		} catch (IOException ignored) {
		}

		mView = new GL2JNIView(this);
		setContentView(mView);

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
