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
GLES2Lesson *gles2Lesson = nullptr;
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
    gles2Lesson = new GLES2Lesson();
    gles2Lesson->setTexture(pixels, 128, 128, 1);
    pixels = nullptr; //now, it belongs to gles2Lesson.
    return gles2Lesson->init(w, h, gVertexShader.c_str(), gFragmentShader.c_str());
}

void renderFrame() {
    if (gles2Lesson != nullptr) {
        gles2Lesson->render();
    }
}

void shutdown() {
    GLES2Lesson *local = gles2Lesson;
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
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                                    jobject assetManager);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson06_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobject bitmap);


JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_onDestroy(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_step(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_tick(JNIEnv *env, jobject obj);
};

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                                    jobject assetManager) {
    loadShaders(env, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_step(JNIEnv *env, jobject obj) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_tick(JNIEnv *env, jobject obj) {
    tick();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson06_GL2JNILib_onDestroy(JNIEnv *env, jobject obj) {
    shutdown();
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson06_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobject bitmap) {

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