//
// Created by monty on 23/11/15.
//

#ifndef LESSON04_GLES2LESSON_H
#define LESSON04_GLES2LESSON_H

class GLES2Lesson {

	void fetchShaderLocations();

	void setPerspective();

	void prepareShaderProgram() const;

	static void clearBuffers();

	void resetTransformMatrices();

	static void printVerboseDriverInformation();

	void createVBOs();

	void deleteVBOs();

	void drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
					  const glm::mat4 &transform) const;

	GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

	static GLuint loadShader(GLenum shaderType, const char *pSource);

	const static float triangleVertices[18];
	const static unsigned short triangleIndices[3];

	const static float squareVertices[24];
	const static unsigned short squareIndices[4];

	glm::mat4 triangleTransformMatrix{};
	glm::mat4 squareTransformMatrix{};
	glm::mat4 projectionMatrix{};

	GLint vertexAttributePosition;
	GLint colourAttributePosition;
	GLint modelMatrixAttributePosition;
	GLint projectionMatrixAttributePosition;
	GLuint gProgram;

	//VBO stuff
	GLuint vboTriangleVertexDataIndex{};
	GLuint vboSquareVertexDataIndex{};
	GLuint vboTriangleVertexIndicesIndex{};
	GLuint vboSquareVertexIndicesIndex{};

	float triangleRotationAngle;
	float squareRotationAngle;
public:
	GLES2Lesson();

	~GLES2Lesson();

	bool init(float w, float h, const std::string &vertexShader, const std::string &fragmentShader);

	void render();

	static void shutdown();

	void tick();
};

#endif //LESSON04_GLES2LESSON_H
