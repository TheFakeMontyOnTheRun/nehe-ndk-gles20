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

#include <GLES2/gl2.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glm/glm.hpp"
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "GLES2Lesson.h"
#include "NdkGlue.h"

#include "android_asset_operations.h"

static std::string gVertexShader;
static std::string gFragmentShader;

void loadShaders(JNIEnv *env, jobject &obj) {
    AAssetManager *asset_manager = AAssetManager_fromJava(env, obj);
    FILE *fd;
    fd = android_fopen("vertex.glsl", "r", asset_manager);
    gVertexShader = readShaderToString(fd);
    fclose(fd);
    fd = android_fopen("fragment.glsl", "r", asset_manager);
    gFragmentShader = readShaderToString(fd);
    fclose(fd);
}

bool setupGraphics(int w, int h) {
    return GLES2Lesson::init(w, h, gVertexShader.c_str(), gFragmentShader.c_str());
}

void renderFrame() {
    GLES2Lesson::render();
}

void shutdown() {
    GLES2Lesson::shutdown();
}

extern "C" {
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                                    jobject assetManager);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_onDestroy(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_step(JNIEnv *env, jobject obj);
};

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                                    jobject assetManager) {
    loadShaders(env, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_step(JNIEnv *env, jobject obj) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson03_GL2JNILib_onDestroy(JNIEnv *env, jobject obj) {
    shutdown();
}

