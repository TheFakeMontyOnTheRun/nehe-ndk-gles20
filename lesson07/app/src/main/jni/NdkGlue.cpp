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
#include <stdint.h>
#include <jni.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "GLES2Lesson.h"
#include "NdkGlue.h"

#include "android_asset_operations.h"

std::string gVertexShader;
std::string gFragmentShader;
odb::GLES2Lesson *gles2Lesson = nullptr;
int *texturePixels;
int *normals;

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
    gles2Lesson->setTexture( texturePixels, normals, 128, 128, 1);
	texturePixels = nullptr; //now, it belongs to gles2Lesson.
	normals = nullptr;
    return gles2Lesson->init(w, h, gVertexShader.c_str(), gFragmentShader.c_str());
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

extern "C" {
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                                    jobject assetManager);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_setTextures(JNIEnv *env, jclass type, jobject texture,
                                                        jobject normalMap);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onDestroy(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_step(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_tick(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_toggleFiltering(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_toggleLightningMode(JNIEnv *env, jclass type);


JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_speedUpYZRotation(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_speedDownYZRotation(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_speedUpXZRotation(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_speedDownXZRotation(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_reset(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_dragTo(JNIEnv *env, jclass type, jfloat x, jfloat y);

}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                                    jobject assetManager) {
    loadShaders(env, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_step(JNIEnv *env, jobject obj) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_tick(JNIEnv *env, jobject obj) {
    tick();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onDestroy(JNIEnv *env, jobject obj) {
    shutdown();
}

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_setTextures(JNIEnv *env, jclass type, jobject texture,
                                                        jobject normalMap) {

    void *addr;
    AndroidBitmapInfo info;
    int errorCode;
	long size;

    if ((errorCode = AndroidBitmap_lockPixels(env, texture, &addr)) != 0) {
        LOGI("error %d", errorCode);
    }

    if ((errorCode = AndroidBitmap_getInfo(env, texture, &info)) != 0) {
        LOGI("error %d", errorCode);
    }

    LOGI("bitmap info: %d wide, %d tall, %d ints per pixel", info.width, info.height, info.format);


    size = info.width * info.height * info.format;
    texturePixels = new int[size];
    memcpy(texturePixels, addr, size * sizeof(int));

    if ((errorCode = AndroidBitmap_unlockPixels(env, texture)) != 0) {
        LOGI("error %d", errorCode);
    }
	////////

	if ((errorCode = AndroidBitmap_lockPixels(env, normalMap, &addr)) != 0) {
		LOGI("error %d", errorCode);
	}

	if ((errorCode = AndroidBitmap_getInfo(env, normalMap, &info)) != 0) {
		LOGI("error %d", errorCode);
	}

	LOGI("bitmap info: %d wide, %d tall, %d ints per pixel", info.width, info.height, info.format);


	size = info.width * info.height * info.format;
	normals = new int[size];
	memcpy(normals, addr, size * sizeof(int));

	if ((errorCode = AndroidBitmap_unlockPixels(env, normalMap)) != 0) {
		LOGI("error %d", errorCode);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_toggleFiltering(JNIEnv *env, jclass type) {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->toggleFiltering();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_toggleLightningMode(JNIEnv *env, jclass type) {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->toggleLightning();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_speedUpYZRotation(JNIEnv *env, jclass type) {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->speedUpYZ();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_speedDownYZRotation(JNIEnv *env, jclass type) {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->speedDownYZ();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_speedUpXZRotation(JNIEnv *env, jclass type) {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->speedUpXZ();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_speedDownXZRotation(JNIEnv *env, jclass type) {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->speedDownXZ();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_reset(JNIEnv *env, jclass type) {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->reset();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson07_GL2JNILib_dragTo(JNIEnv *env, jclass type, jfloat x, jfloat y) {

    if ( gles2Lesson != nullptr ) {
        gles2Lesson->setSpeeds( glm::normalize( glm::vec2( x, y ) ) );
    }
}