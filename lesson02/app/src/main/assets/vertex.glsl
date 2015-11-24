attribute vec4 aPosition;
uniform mat4 uModel;
uniform mat4 uProjection;

void main() {
    gl_Position =  uProjection * uModel * aPosition;
}
