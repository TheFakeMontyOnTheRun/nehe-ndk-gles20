#include <GLES2/gl2.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glm/glm.hpp"

#include "GLES2Lesson.h"
#include "NdkGlue.h"
#include <cerrno>

#include "android_asset_operations.h"

char *gVertexShader = nullptr;
char *gFragmentShader = nullptr;

void loadShaders(JNIEnv *env, jobject &obj) {
	AAssetManager *asset_manager = AAssetManager_fromJava(env, obj);
	FILE *fd;
	fd = android_fopen("vertex.glsl", "r", asset_manager);
	gVertexShader = readToString(fd);
	fclose(fd);
	fd = android_fopen("fragment.glsl", "r", asset_manager);
	gFragmentShader = readToString(fd);
	fclose(fd);
}

bool setupGraphics(int w, int h) {
	return GLES2Lesson::init(w, h, gVertexShader, gFragmentShader);
}

void renderFrame() {
	GLES2Lesson::render();
}

void shutdown() {
	GLES2Lesson::shutdown();
	free(gVertexShader);
	free(gFragmentShader);

	gVertexShader = nullptr;
	gFragmentShader = nullptr;
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson02_GL2JNILib_onCreate(JNIEnv *env, jobject clazz, jobject assetManager) {
	loadShaders(env, assetManager);
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson02_GL2JNILib_init(JNIEnv *env, jclass clazz, jint width, jint height) {
	setupGraphics(width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson02_GL2JNILib_step(JNIEnv *env, jclass clazz) {
	renderFrame();
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson02_GL2JNILib_onDestroy(JNIEnv *env, jobject clazz) {
	shutdown();
}