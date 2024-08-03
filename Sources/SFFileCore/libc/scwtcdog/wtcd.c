//===-- SFFileCore/libc/wtcd.c - SCWTCDog File Watcher ------------*- C -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// You may not use this file except in compliance with the License.           //
// You may obtain a copy of the License at                                    //
//                                                                            //
//      http://www.apache.org/licenses/LICENSE-2.0                            //
//                                                                            //
// Unless required by applicable law or agreed to in writing, software        //
// distributed under the License is distributed on an "AS IS" BASIS,          //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   //
// See the License for the specific language governing permissions and        //
// limitations under the License.                                             //
//                                                                            //
//===----------------------------------------------------------------------===//
///
/// \file
///
//===----------------------------------------------------------------------===//

#include "wtcd.h"

time_t getFileModTime(const char *path) {
    struct stat fileStat;
    if (stat(path, &fileStat) == -1) {
        perror("Failed to fetch file stat.\n");
        return -1;
    }

    return fileStat.st_mtime;
}

void updateFileState(const char *path) {
    FileState *current = fileStateList;
    FileState *prev    = NULL;

    while (current != NULL) {
        if (strcmp(current->path, path) == 0) {
            current->modTime = getFileModTime(path);
            return;
        }

        prev = current;
        current = current->next;
    }

    FileState *newState = malloc(sizeof(FileState));
    if (!newState) {
        perror("Allocation (malloc) failed.\n");
        return;
    }
    newState->path     = strdup(path);
    newState->modTime  = getFileModTime(path);
    newState->next     = NULL;

    if (!newState->path) {
        free(newState);
        perror("Allocation (strdup) failed.\n");
        return;
    }

    if (prev != NULL) {
        prev->next     = newState;
    } else {
        fileStateList  = newState;
    }
}

int hasFileChanged(const char *path) {
    time_t currentTime = getFileModTime(path);
    FileState *current = fileStateList;

    while (current != NULL) {
        if (strcmp(current->path, path) == 0) {
            return current->modTime != currentTime;
        }

        current        = current-> next;
    }
    return 1;          // ~> New or not tracked file
}