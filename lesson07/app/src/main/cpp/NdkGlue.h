//
// Created by monty on 23/11/15.
//

#ifndef LESSON07_NDKGLUE_H
#define LESSON07_NDKGLUE_H

#define  LOG_TAG    "NdkGlue"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

void printGLString(const char *name, GLenum s);

void checkGlError(const char *op);

#endif //LESSON07_NDKGLUE_H
