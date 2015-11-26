attribute vec4 aPosition;
//attribute vec4 aColour;
uniform mat4 uModel;
uniform mat4 uProjection;
//varying vec4 vColour;

void main() {
    gl_Position =  uProjection * uModel * aPosition;
//    vColour = aColour;
}
