//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H


class GLES2Lesson {
public:

	static float triangleVertices[9];
	static float squareVertices[18];
	static glm::mat4 triangleTransformMatrix;
	static glm::mat4 squareTransformMatrix;
	static glm::mat4 projectionMatrix;

	static GLint vertexAttributePosition;
	static GLint modelMatrixAttributePosition;
	static GLint projectionMatrixAttributePosition;
	static GLuint gProgram;

	static bool init(float w, float h, const char *vertexShader, const char *fragmentShader);

	static void render();

	static void shutdown();
};


#endif //LESSON02_GLES2LESSON_H
