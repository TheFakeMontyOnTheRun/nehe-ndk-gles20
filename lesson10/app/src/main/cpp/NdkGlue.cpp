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


char *gVertexShader;
char *gFragmentShader;
char *worldData;
std::map<GLuint, std::vector<Trig> > batches;
odb::GLES2Lesson *gles2Lesson = nullptr;
std::vector<std::shared_ptr<NativeBitmap>> textures;

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

	for (auto &pair: batches) {
		gles2Lesson->addTrigsForTexture(pair.first, pair.second);
	}
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
	free(worldData);
}

void tick() {
	if (gles2Lesson != nullptr) {
		gles2Lesson->tick();
	}
}

float toFloat(const char *nptr) {
	return (strtod(nptr, nullptr));
}

glm::vec3 readVertex(std::vector<std::string>::iterator &it) {
	float x = toFloat(it->c_str());
	it++;

	float y = toFloat(it->c_str());
	it++;

	float z = toFloat(it->c_str());
	it++;

	return {x, y, z};
}

glm::vec2 readUv(std::vector<std::string>::iterator &it) {
	float x = toFloat(it->c_str());
	it++;

	float y = toFloat(it->c_str());
	it++;

	return {x, y};
}

std::string filterComments(const std::string& input) {
	bool reading = true;
	std::stringstream output;

	for (auto &character: input) {

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

void readWorld(JNIEnv *env, jobject type,
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

		if (*it == "m") {
			++it;
			currentMaterial = atoi(it->c_str());
			++it;
		}

		trig.p0 = readVertex(it);
		trig.t0 = readUv(it);
		trig.p1 = readVertex(it);
		trig.t1 = readUv(it);
		trig.p2 = readVertex(it);
		trig.t2 = readUv(it);

		batches[currentMaterial].push_back(trig);
		++polyCount;
	}

	LOGI("done loading\n");
}

extern "C"
void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_onCreate(JNIEnv *env, jobject type,
														  jobject assetManager) {
	loadShaders(env, assetManager);
	readWorld(env, type, assetManager);
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_init(JNIEnv *env, jobject type,
																jint width, jint height) {
	setupGraphics(width, height);
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_step(JNIEnv *env, jobject type) {
	renderFrame();
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_tick(JNIEnv *env, jobject type) {
	tick();
}

extern "C"
JNIEXPORT void JNICALL Java_br_odb_nehe_lesson10_GL2JNILib_onDestroy(JNIEnv *env, jobject type) {
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

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_reset(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->reset();
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_moveForward(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->moveForward(0.25f);
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_moveBackward(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->moveBackward(0.25f);
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_turnRight(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->turnRight(3.0f);
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_turnLeft(JNIEnv *env, jobject type) {
	if (gles2Lesson != nullptr) {
		gles2Lesson->turnLeft(3.0f);
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_onTouchNormalized(JNIEnv *env, jobject type, jfloat x,
													  jfloat y) {

	if (gles2Lesson != nullptr) {
		gles2Lesson->onNormalizedTouch((float) x, (float) y);
	}
}

extern "C"
JNIEXPORT void JNICALL
Java_br_odb_nehe_lesson10_GL2JNILib_setTexture(JNIEnv *env, jobject type, jobjectArray bitmaps) {

	int length = env->GetArrayLength(bitmaps);
	for (int c = 0; c < length; ++c) {
		textures.push_back(
				makeNativeBitmapFromJObject(env, env->GetObjectArrayElement(bitmaps, c)));
	}
}