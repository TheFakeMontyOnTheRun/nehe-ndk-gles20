attribute vec4 aPosition;
uniform mat4 uMVP;

void main() {
    gl_Position = uMVP * aPosition;
}
