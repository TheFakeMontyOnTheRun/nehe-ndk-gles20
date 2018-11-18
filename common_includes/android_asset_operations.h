static int android_read(void *asset, char *buf, int size) {
    return AAsset_read((AAsset *) asset, buf, size);
}

static int android_write(void *asset, const char *buf, int size) {
    return EACCES; // can't provide write access to the apk
}

static fpos_t android_seek(void *asset, fpos_t offset, int whence) {
    return AAsset_seek((AAsset *) asset, offset, whence);
}

static int android_close(void *asset) {
    AAsset_close((AAsset *) asset);
    return 0;
}


FILE *android_fopen(const char *fname, const char *mode, AAssetManager *assetManager) {
    if (mode[0] == 'w') return NULL;

    AAsset *asset = AAssetManager_open(assetManager, fname, 0);
    if (!asset) return NULL;

    return funopen(asset, android_read, android_write, android_seek, android_close);
}

//improvements, as promissed.
char* readToString(FILE *fileDescriptor) {
    fseek(fileDescriptor, 0, SEEK_END);
    size_t fileSize = ftell(fileDescriptor);
    char *buffer = (char*)calloc(1, fileSize + 1); //so we null terminate it.
    rewind(fileDescriptor);
    fread((void *) buffer, fileSize, 1 , fileDescriptor);

    return buffer;
}

