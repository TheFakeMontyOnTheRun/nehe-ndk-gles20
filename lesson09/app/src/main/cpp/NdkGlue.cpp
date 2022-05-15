#include <stdint.h>
#include <jni.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>



#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Star.h"
#include "GLES2Lesson.h"
#include "NdkGlue.h"

#include "android_asset_operations.h"

char* gVertexShader;
char* gFragmentShader;
odb::GLES2Lesson *gles2Lesson = nullptr;
int *pixels;
int *details;

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
    gles2Lesson->setTexture(pixels, details, 128, 128);
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
    free(gVertexShader);
    free(gFragmentShader);
}

void tick() {
    if (gles2Lesson != nullptr) {
        gles2Lesson->tick();
    }
}

extern "C" {
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_onCreate(JNIEnv *env, jclass type,
                                                                    jobject assetManager);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson09_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobject bitmap,
                                                       jobject detail);


JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_onDestroy(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_init(JNIEnv *env, jclass type,
                                                                jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_step(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_tick(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson09_GL2JNILib_reset(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson09_GL2JNILib_toggleTwinkling(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson09_GL2JNILib_zoomIn(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson09_GL2JNILib_speedUpTwist(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson09_GL2JNILib_zoomDown(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson09_GL2JNILib_speedDownTwist(JNIEnv *env, jclass type);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_onCreate(JNIEnv *env, jclass type,
                                                                    jobject assetManager) {
    loadShaders(env, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_init(JNIEnv *env, jclass type,
                                                                jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_step(JNIEnv *env, jclass type) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_tick(JNIEnv *env, jclass type) {
    tick();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson09_GL2JNILib_onDestroy(JNIEnv *env, jclass type) {
    shutdown();
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson09_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobject bitmap,
                                               jobject detail) {

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


    if ((errorCode = AndroidBitmap_lockPixels(env, detail, &addr)) != 0) {
        LOGI("error %d", errorCode);
    }

    if ((errorCode = AndroidBitmap_getInfo(env, detail, &info)) != 0) {
        LOGI("error %d", errorCode);
    }

    LOGI("detail info: %d wide, %d tall, %d ints per pixel", info.width, info.height, info.format);


    size = info.width * info.height * info.format;
    details = new int[size];
    memcpy(details, addr, size * sizeof(int));

    if ((errorCode = AndroidBitmap_unlockPixels(env, detail)) != 0) {
        LOGI("error %d", errorCode);
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson09_GL2JNILib_toggleTwinkling(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->toggleTwinkling();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson09_GL2JNILib_zoomIn(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->zoomIn();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson09_GL2JNILib_speedUpTwist(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->speedUpTwist();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson09_GL2JNILib_zoomDown(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->zoomOut();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson09_GL2JNILib_speedDownTwist(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->speedDownTwist();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson09_GL2JNILib_reset(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->reset();
    }
}