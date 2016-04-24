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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLES2/gl2.h"

#include <jni.h>
#include <android/bitmap.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <memory>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_set>

#include <map>
#include "NativeBitmap.h"
#include "Trig.h"
#include "WalkBouncer.h"
#include "TrigBatch.h"
#include "GLES2Lesson.h"
#include "NdkGlue.h"

#include "android_asset_operations.h"


std::string gVertexShader;
std::string gFragmentShader;
std::string worldData;
std::map< GLuint, std::vector<Trig> > batches;
odb::GLES2Lesson *gles2Lesson = nullptr;
std::vector< std::shared_ptr<NativeBitmap>> textures;

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
    gles2Lesson->setTextures(textures);

    for ( auto& pair : batches ) {
        gles2Lesson->addTrigsForTexture(pair.first, pair.second);
    }
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
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                                    jobject assetManager);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobjectArray bitmap);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_onDestroy(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_setPerspective(JNIEnv *env, jclass type,
                                                           jfloatArray perspective_);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height);
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_step(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_tick(JNIEnv *env, jobject obj);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_reset(JNIEnv *env, jclass type);


JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_moveForward(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_moveBackward(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_turnRight(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_turnLeft(JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_onTouchNormalized(JNIEnv *env, jclass type, jfloat x,
                                                              jfloat y);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_setLookAtMatrix(JNIEnv *env, jclass type, jfloatArray lookAt_);

JNIEXPORT void JNICALL
        Java_br_odb_nehe_lesson10_GL2JNILib_setXZAngle(JNIEnv *env, jclass type, jfloat xz);
}

float toFloat(const char *nptr)
{
    return (strtod(nptr, NULL));
}

glm::vec3 readVertex(std::vector<std::string>::iterator &it) {
    float x = toFloat(it->c_str());
    it++;

    float y = toFloat(it->c_str());
    it++;

    float z = toFloat(it->c_str());
    it++;

    return glm::vec3(x, y, z);
}

glm::vec2 readUv(std::vector<std::string>::iterator &it) {
    float x = toFloat(it->c_str());
    it++;

    float y = toFloat(it->c_str());
    it++;

    return glm::vec2(x, y);
}

std::string filterComments(std::string input) {
    bool reading = true;
    std::stringstream output;

    for (auto &character : input) {

        if (character == '/') {
            reading = false;
        } else if (character == '\n') {
            reading = true;
        }

        if (reading) {
            output << character;
        }
    }

    return output.str();
}

void readWorld(JNIEnv *env, void *reserved,
               jobject assetManager) {

    AAssetManager *asset_manager = AAssetManager_fromJava(env, assetManager);
    FILE *fd;
    fd = android_fopen("world.txt", "r", asset_manager);
    worldData = readToString(fd);
    fclose(fd);

    std::stringstream buffer;

    buffer << filterComments(worldData);

    std::vector<std::string> tokenList{std::istream_iterator<std::string>(buffer),
                                       std::istream_iterator<std::string>{}};
    auto it = tokenList.begin();
    auto end = tokenList.end() - 1;

    it++;

    int numPolies = atoi(it->c_str());

    it++;
    int currentMaterial = 0;
    int polyCount = 0;

    while (polyCount < numPolies && it != end) {
        Trig trig;

        if ( *it == "m" ) {
            ++it;
            currentMaterial = atoi( it->c_str() );
            ++it;
        }

        trig.p0 = readVertex(it);
        trig.t0 = readUv(it);
        trig.p1 = readVertex(it);
        trig.t1 = readUv(it);
        trig.p2 = readVertex(it);
        trig.t2 = readUv(it);

        batches[ currentMaterial ].push_back(trig);
        ++polyCount;
    }

    LOGI("done loading\n");
}

void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_onCreate(JNIEnv *env, void *reserved,
                                                          jobject assetManager) {
    loadShaders(env, assetManager);
    readWorld(env, reserved, assetManager);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_init(JNIEnv *env, jobject obj,
                                                                jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_step(JNIEnv *env, jobject obj) {
    renderFrame();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_tick(JNIEnv *env, jobject obj) {
    tick();
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_onDestroy(JNIEnv *env, jobject obj) {
    shutdown();
}

std::shared_ptr<NativeBitmap> makeNativeBitmapFromJObject(JNIEnv *env, jobject bitmap) {

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
    int *pixels = new int[size];
    memcpy(pixels, addr, size * sizeof(int));
    auto toReturn = std::make_shared<NativeBitmap>(128, 128, pixels);
    
    if ((errorCode = AndroidBitmap_unlockPixels(env, bitmap)) != 0) {
        LOGI("error %d", errorCode);
    }
    
    return toReturn;
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_reset(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->reset();
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_moveForward(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->moveForward(0.5f);
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_moveBackward(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->moveBackward(0.5f);
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_turnRight(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->turnRight(3.0f);
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_turnLeft(JNIEnv *env, jclass type) {
    if (gles2Lesson != nullptr) {
        gles2Lesson->turnLeft(3.0f);
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_onTouchNormalized(JNIEnv *env, jclass type, jfloat x,
                                                      jfloat y) {

    if (gles2Lesson != nullptr) {
        gles2Lesson->onNormalizedTouch((float) x, (float) y);
    }
}



JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_setTexture(JNIEnv *env, jclass type, jobjectArray bitmaps ) {

    int length = env->GetArrayLength( bitmaps );
    for ( int c = 0; c < length; ++c ) {
        textures.push_back( makeNativeBitmapFromJObject( env, env->GetObjectArrayElement( bitmaps, c ) ) );
    }
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_setLookAtMatrix(JNIEnv *env, jclass type, jfloatArray lookAt_) {
    jfloat *lookAt = env->GetFloatArrayElements(lookAt_, NULL);

    if (gles2Lesson != nullptr) {
        gles2Lesson->setLookAtMatrix(lookAt);
    }

    env->ReleaseFloatArrayElements(lookAt_, lookAt, 0);
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_setXZAngle(JNIEnv *env, jclass type, jfloat xz) {

	if (gles2Lesson != nullptr) {
		gles2Lesson->setXZAngleInDegrees(xz);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_setPerspective(JNIEnv *env, jclass type,
                                                   jfloatArray perspective_) {
    jfloat *perspective = env->GetFloatArrayElements(perspective_, NULL);

    if (gles2Lesson != nullptr) {
        gles2Lesson->setPerspectiveMatrix(perspective);
    }

    env->ReleaseFloatArrayElements(perspective_, perspective, 0);

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_strafeLeft(JNIEnv *env, jclass type) {

	if (gles2Lesson != nullptr) {
		gles2Lesson->strafeLeft(0.5f);
	}
}

JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_strafeRight(JNIEnv *env, jclass type) {

	if (gles2Lesson != nullptr) {
		gles2Lesson->strafeRight(0.5f);
	}
}