//
// Created by monty on 23/11/15.
//

#ifndef LESSON02_NDKGLUE_H
#define LESSON02_NDKGLUE_H

#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


#define  LOG_TAG    "NdkGlue"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void printGLString(const char *name, GLenum s);
void checkGlError(const char *op);

#endif //LESSON02_NDKGLUE_H
