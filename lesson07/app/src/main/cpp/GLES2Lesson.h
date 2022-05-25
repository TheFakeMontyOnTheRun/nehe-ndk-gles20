//
// Created by monty on 23/11/15.
//

#ifndef LESSON07_GLES2LESSON_H
#define LESSON07_GLES2LESSON_H

namespace odb {

	class GLES2Lesson {

		void fetchShaderLocations();

		void setPerspective();

		void prepareShaderProgram();

		static void clearBuffers();

		void resetTransformMatrices();

		static void printVerboseDriverInformation();

		void createVBOs();

		void deleteVBOs();

		void drawGeometry(const int vertexVbo, const int indexVbo, int vertexCount,
						  const glm::mat4 &transform) const;

		static GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);

		static GLuint loadShader(GLenum shaderType, const char *pSource);

		const static float cubeVertices[6 * 4 * 9];
		const static unsigned short cubeIndices[6 * 6];

		const static glm::vec4 ambientLightFullColor;
		const static glm::vec4 ambientLightOffColor;

		glm::mat4 cubeTransformMatrix{};
		glm::mat4 projectionMatrix{};

		GLint vertexAttributePosition;
		GLint modelMatrixAttributePosition;
		GLint samplerUniformPosition{};
		GLint textureCoordinatesAttributePosition{};
		GLint projectionMatrixAttributePosition;
		GLint normalAttributePosition{};

		GLuint gProgram;

		GLuint textureId{};

		//VBO stuff
		GLuint vboCubeVertexDataIndex{};
		GLuint vboCubeVertexIndicesIndex{};

		int *textureData;
		int textureWidth{};
		int textureHeight{};
		GLint currentFilter;
		float cubeRotationAngleYZ{};
		float cubeRotationAngleXZ{};

		glm::vec4 diffuseLightDirection{};
		glm::vec4 diffuseLightColor{};
		bool mFullLight = false;

		GLint diffuseLightDirectionShaderLocation{};
		GLint diffuseLightColorShaderLocation{};
		GLint ambientLightColorShaderLocation{};

		float rotationXZSpeed{};
		float rotationYZSpeed{};
	public:
		GLES2Lesson();

		~GLES2Lesson();

		bool init(float w, float h, const std::string &vertexShader,
				  const std::string &fragmentShader);

		void setTexture(int *bitmapData, int width, int height);

		void render();

		void shutdown();

		void tick();

		void toggleFiltering();

		void toggleLightning();

		void speedUpXZ();

		void speedDownXZ();

		void speedUpYZ();

		void speedDownYZ();

		void reset();

		void setSpeeds(const glm::vec2 &param);
	};
}
#endif //LESSON07_GLES2LESSON_H
