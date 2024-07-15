//
//  SFCFileOperations.h
//  
//
//  Created by Nevio Hirani on 15.07.24.
//

#ifndef SFCFileOperations_h
#define SFCFileOperations_h

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

char* encodeToJSON(void* jsonVariant);
void* decodeFromJSON(const char* jsonString);

#ifdef __cplusplus
}
#endif

int writeScribbleFile(const char* filePath, void* jsonVariant);
void* readScribbleFile(const char* filePath);
int deleteScribbleFile(const char* filePath);
int openScribbleFile(const char* filePath, int flags);

#endif /* SFCFileOperations_h */
