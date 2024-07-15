//
//  SFCFileOperations.c
//  
//
//  Created by Nevio Hirani on 15.07.24.
//

#include "SFCFileOperations.h"

int writeScribbleFile(const char* filePath, void* jsonVariant) {
    int fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return -1;
    }
    
    char* jsonString = encodeToJSON(jsonVariant);
    if (!jsonString) {
        return -1;
    }
    
    size_t jsonStringLength = strlen(jsonString);
    ssize_t written = write(fd, jsonString, jsonStringLength);
    free(jsonString);
    close(fd);
    
    if (written == -1 || (size_t)written != jsonStringLength) {
        return -1;
    }
    
    return 0;
}

void* readScribbleFile(const char* filePath) {
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        return NULL;
    }
    
    off_t fileSize = lseek(fd, 0, SEEK_END);
    if (fileSize == -1) {
        close(fd);
        return NULL;
    }
    
    lseek(fd, 0, SEEK_SET);
    
    char* jsonString = (char*)malloc(fileSize + 1);
    if (!jsonString) {
        close(fd);
        return NULL;
    }
    
    size_t readSize = read(fd, jsonString, fileSize);
    close(fd);
    
    if (readSize == -1) {
        free(jsonString);
        return NULL;
    }
    
    jsonString[readSize] = '\0';
    void* jsonVariant = decodeFromJSON(jsonString);
    free(jsonString);
    
    return jsonVariant;
}

int deleteScribbleFile(const char* filePath) {
    return unlink(filePath);
}

int openScribbleFile(const char* filePath, int flags) {
    int fd = open(filePath, flags);
    if (fd == -1) {
        return -1;
    }
    
    return fd;
}
