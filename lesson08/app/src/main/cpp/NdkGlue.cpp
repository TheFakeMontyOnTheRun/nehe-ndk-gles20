/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code
#include <cstdint>
#include <jni.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "GLES2Lesson.h"
#include "NdkGlue.h"

#include "android_asset_operations.h"

std::string gVertexShader;
std::string gFragmentShader;
odb::GLES2Lesson *gles2Lesson = nullptr;
int *pixels;

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
	gles2Lesson = new odb::GLES2Lesson();
	gles2Lesson->setTexture(pixels, 128, 128);
	pixels = nullptr; //now, it belongs to gles2Lesson.
	return gles2Lesson->init(w, h, gVertexShader, gFragmentShader);
}

void renderFrame() {
	if (gles2Lesson != nullptr) {
		gles2Lesson->render();
	}
}

void shutdown() {
	odb::GLES2Lesson *local = gles2Lesson;
	gles2Lesson = nullptr;
	local->shutdown();
	delete local;
}

void tick() {
	if (gles2Lesson != nullptr) {
		gles2Lesson->tick();
	}
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson08_GL2JNILib_onCreate(JNIEnv *env, jobject type,
																	jobject assetManager) {
	loadShaders(env, assetManager);
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson08_GL2JNILib_init(JNIEnv *env, jobject type,
																jint width, jint height) {
	setupGraphics(width, height);
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson08_GL2JNILib_step(JNIEnv *env, jobject type) {
	renderFrame();
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson08_GL2JNILib_tick(JNIEnv *env, jobject type) {
	tick();
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson08_GL2JNILib_onDestroy(JNIEnv *env, jobject type) {
	shutdown();
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_setTexture(JNIEnv *env, jobject type, jobject bitmap) {

	void *addr;
	AndroidBitmapInfo info;
	int errorCode;

	if ((errorCode = AndroidBitmap_lockPixels(env, bitmap, &addr)) != 0) {
		LOGI("error %d", errorCode);
	}

	if ((errorCode = AndroidBitmap_getInfo(env, bitmap, &info)) != 0) {
		LOGI("error %d", errorCode);
	}

	LOGI("bitmap info: %d wide, %d tall, %d ints per pixel", info.width, info.height, info.format);


	long size = info.width * info.height * info.format;
	pixels = new int[size];
	memcpy(pixels, addr, size * sizeof(int));

	if ((errorCode = AndroidBitmap_unlockPixels(env, bitmap)) != 0) {
		LOGI("error %d", errorCode);
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_toggleFiltering(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->toggleFiltering();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_toggleLightningMode(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->toggleLightning();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_speedUpYZRotation(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->speedUpYZ();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_speedDownYZRotation(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->speedDownYZ();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_speedUpXZRotation(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->speedUpXZ();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_speedDownXZRotation(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->speedDownXZ();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_reset(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->reset();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_dragTo(JNIEnv *env, jobject type, jfloat x, jfloat y) {

	if (gles2Lesson != nullptr) {
		gles2Lesson->setSpeeds(glm::normalize(glm::vec2(x, y)));
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson08_GL2JNILib_toggleBlending(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->toggleBlending();
	}
}