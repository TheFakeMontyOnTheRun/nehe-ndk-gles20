package br.odb.nehe.lesson10;


import android.content.Context;

import com.google.vrtoolkit.cardboard.CardboardView;

class GL2JNIView extends CardboardView {

	public GL2JNIView(Context context) {
		super(context);
		setRenderer(new GL2JNIRenderer());
	}
}
