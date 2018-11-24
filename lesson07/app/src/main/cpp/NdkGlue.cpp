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

char* gVertexShader;
char* gFragmentShader;
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
    gles2Lesson->setTexture( texturePixels, normals, 128, 128);
	texturePixels = nullptr; //now, it belongs to gles2Lesson.
	normals = nullptr;
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
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onCreate(JNIEnv *env, jclass type,
                                                                    jobject assetManager);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson07_GL2JNILib_setTextures(JNIEnv *env, jclass type, jobject texture,
                                                        jobject normalMap);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onDestroy(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_init(JNIEnv *env, jclass type,
                                                                jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_step(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_tick(JNIEnv *env, jclass type);

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

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onCreate(JNIEnv *env, jclass type,
                                                                    jobject assetManager) {
    loadShaders(env, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_init(JNIEnv *env, jclass type,
                                                                jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_step(JNIEnv *env, jclass type) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_tick(JNIEnv *env, jclass type) {
    tick();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson07_GL2JNILib_onDestroy(JNIEnv *env, jclass type) {
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