static int android_read(void *cookie, char *buf, int size) {
    return AAsset_read((AAsset *) cookie, buf, size);
}

static int android_write(void *cookie, const char *buf, int size) {
    return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void *cookie, fpos_t offset, int whence) {
    return AAsset_seek((AAsset *) cookie, offset, whence);
}

static int android_close(void *cookie) {
    AAsset_close((AAsset *) cookie);
    return 0;
}


FILE *android_fopen(const char *fname, const char *mode, AAssetManager *assetManager) {
    if (mode[0] == 'w') return NULL;

    AAsset *asset = AAssetManager_open(assetManager, fname, 0);
    if (!asset) return NULL;

    return funopen(asset, android_read, android_write, android_seek, android_close);
}

char* readToString(FILE *fileDescriptor) {
    fseek(fileDescriptor, 0, SEEK_END);
    size_t fileSize = ftell(fileDescriptor);
    char *buffer = (char*)calloc(1, fileSize + 1); //so we null terminate it.
    rewind(fileDescriptor);
    fread((void *) buffer, fileSize, 1 , fileDescriptor);

    return buffer;
}

