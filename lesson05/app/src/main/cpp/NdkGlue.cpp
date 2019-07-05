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

char* gVertexShader;
char* gFragmentShader;
GLES2Lesson* gles2Lesson = nullptr;

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
    return gles2Lesson->init(w, h, gVertexShader, gFragmentShader);
}

void renderFrame() {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->render();
    }
}

void shutdown() {
    GLES2Lesson *local = gles2Lesson;
    gles2Lesson = nullptr;
    local->shutdown();
    delete local;
    free(gVertexShader);
    free(gFragmentShader);
}

void tick() {
    if ( gles2Lesson != nullptr ) {
        gles2Lesson->tick();
    }
}

extern "C" {
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_onCreate(JNIEnv *env, jclass type,
                                                                    jobject assetManager);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_onDestroy(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_init(JNIEnv *env, jclass type,
                                                                jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_step(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_tick(JNIEnv *env, jclass type);
};

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_onCreate(JNIEnv *env, jclass type,
                                                                    jobject assetManager) {
    loadShaders(env, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_init(JNIEnv *env, jclass type,
                                                                jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_step(JNIEnv *env, jclass type) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_tick(JNIEnv *env, jclass type) {
    tick();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson05_GL2JNILib_onDestroy(JNIEnv *env, jclass type) {
    shutdown();
}

