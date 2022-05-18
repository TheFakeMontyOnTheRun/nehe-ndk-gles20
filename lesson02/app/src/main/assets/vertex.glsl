#ifdef GL_ES
precision highp float;
#endif

#if __VERSION__ >= 140
	in vec4 aPosition;
	in mat4 uModel;
	in mat4 uProjection;
#else
	attribute vec4 aPosition;
	uniform mat4 uModel;
	uniform mat4 uProjection;
#endif

void main() {
	gl_Position =  uProjection * uModel * aPosition;
}
