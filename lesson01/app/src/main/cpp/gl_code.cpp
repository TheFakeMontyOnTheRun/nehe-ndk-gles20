#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cerrno>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static std::string gVertexShader;
static std::string gFragmentShader;

static int android_read(void* asset, char* buf, int size) {
    return AAsset_read((AAsset*)asset, buf, size);
}

static int android_write(void* asset, const char* buf, int size) {
    return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void* asset, fpos_t offset, int whence) {
    return AAsset_seek((AAsset*)asset, offset, whence);
}

static int android_close(void* asset) {
    AAsset_close((AAsset*)asset);
    return 0;
}


FILE* android_fopen(const char* fname, const char* mode, AAssetManager *assetManager) {
    if(mode[0] == 'w') return NULL;

    AAsset* asset = AAssetManager_open( assetManager, fname, 0);
    if(!asset) return NULL;

    return funopen(asset, android_read, android_write, android_seek, android_close);
}

//We will eventually improve this, but let's settle with a familar structure, for now.
std::string readShaderToString( FILE* fileDescriptor ) {
    const unsigned N=1024;
    std::string total;
    while (true) {
        char buffer[ N ];
        size_t read = fread((void *)&buffer[0], 1, N, fileDescriptor);
        if (read) {
            for ( int c = 0; c <  read; ++c ) {
                total.push_back( buffer[ c ] );
            }
        }
        if (read < N) {
            break;
        }
    }

    return total;
}

void loadShaders( JNIEnv* env, jobject& obj ) {
    AAssetManager *asset_manager = AAssetManager_fromJava( env, obj );
    FILE* fd;
    fd = android_fopen( "vertex.glsl", "r", asset_manager );
    gVertexShader = readShaderToString( fd );
    fclose( fd );
    fd = android_fopen( "fragment.glsl", "r", asset_manager );
    gFragmentShader = readShaderToString( fd );
    fclose( fd );
}

GLuint loadShader(GLenum shaderType, const char* pSource) {
    auto shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    auto vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    auto pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    auto program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;

bool setupGraphics(int w, int h) {

    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader.c_str(), gFragmentShader.c_str());
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}

void renderFrame() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");
}

extern "C" {
    JNIEXPORT void JNICALL Java_br_odb_nehe_lesson01_GL2JNILib_onCreate(JNIEnv *env, jclass type, jobject assetManager);
    JNIEXPORT void JNICALL Java_br_odb_nehe_lesson01_GL2JNILib_init(JNIEnv *env, jclass type, jint width, jint height);
    JNIEXPORT void JNICALL Java_br_odb_nehe_lesson01_GL2JNILib_step(JNIEnv *env, jclass type);
};

extern "C" JNIEXPORT void JNICALL Java_br_odb_nehe_lesson01_GL2JNILib_onCreate(JNIEnv *env, jclass type, jobject assetManager) {
    loadShaders( env, assetManager );
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson01_GL2JNILib_init(JNIEnv *env, jclass type, jint width, jint height) {
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_br_odb_nehe_lesson01_GL2JNILib_step(JNIEnv *env, jclass type) {
    renderFrame();
}