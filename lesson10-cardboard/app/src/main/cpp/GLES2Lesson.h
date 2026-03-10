//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_GLES2LESSON_H
#define LESSON02_GLES2LESSON_H

namespace odb {

	class GLES2Lesson {

		void fetchShaderLocations();

		void setPerspective();

		void prepareShaderProgram();

		void clearBuffers();

		void resetTransformMatrices();

		void printVerboseDriverInformation();

		GLint createProgram(const char *pVertexSource, const char *pFragmentSource);

		GLint loadShader(GLenum shaderType, const char *pSource);

		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

		GLint vertexAttributePosition;
		GLint modelMatrixAttributePosition;
		GLint viewMatrixAttributePosition;
		GLint samplerUniformPosition;
		GLint textureCoordinatesAttributePosition;
		GLint projectionMatrixAttributePosition;
		GLint gProgram;
		WalkBouncer mBouncer;
		GLuint textureId;
		std::map<GLuint, std::shared_ptr<TrigBatch> > mBatches;
		glm::vec3 camera;
		glm::vec3 cameraDirection;
		float angleXzInDegress;
		std::vector<std::shared_ptr<NativeBitmap> > mTextures;
		std::vector<GLint> mMaterials;

	public:
		explicit GLES2Lesson();

		~GLES2Lesson();

		bool init(float w, float h, const std::string &vertexShader,
		          const std::string &fragmentShader);

		void render();

		void shutdown();

		void tick();

		void reset();

		void moveForward(float d);

		void moveBackward(float d);

		void turnRight(float d);

		void turnLeft(float d);

		void updateDirectionVector();

		void onNormalizedTouch(float d, float d1);

		void addTrigsForTexture(GLuint textureId, const std::vector<Trig> &newTrigs);

		void setTextures(const std::vector<std::shared_ptr<NativeBitmap>> &vector);

		void setLookAtMatrix(float *pDouble);

		void setXZAngleInDegrees(float d);

		void setPerspectiveMatrix(float *pDouble);

		void strafeLeft(float d);

		void strafeRight(float d);
	};
}
#endif //LESSON02_GLES2LESSON_H
