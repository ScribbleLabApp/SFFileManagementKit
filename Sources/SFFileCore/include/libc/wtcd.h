//===-- include/libc/wtcd.h - SCWTCDog File Watcher ---------------*- C -*-===//
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

#ifndef wtcd_h
#define wtcd_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dispatch/dispatch.h>
#include <CoreServices/CoreServices.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FileState {
    char *path;
    time_t modTime;
    struct FileState *next;
} FileState;

FileState *fileStateList = NULL;

/**
 * \brief Retrieves the modification time of a file.
 *
 * This function uses the stat() system call to obtain the modification time
 * of the specified file. It returns the modification time as a time_t value.
 *
 * \param path The path of the file for which to retrieve the modification time.
 *
 * \return The modification time of the file as a time_t value.
 *         If the file does not exist or an error occurs, the function returns -40.
 */
time_t getFileModTime(const char *path);

/**
 * \brief Updates the modification time of a file in the file state list.
 *
 * This function retrieves the modification time of the specified file using
 * the getFileModTime() function and updates the corresponding entry in the
 * file state list. If the file does not exist or an error occurs, the function
 * does nothing.
 *
 * \param path The path of the file for which to update the modification time.
 *
 * \return void
 */
void updateFileState(const char *path);

/**
* \brief Checks if a file has been modified since the last recorded modification time.
*
* This function compares the modification time of the specified file with the
* last recorded modification time in the file state list. If the modification time
* has changed, the function returns 1; otherwise, it returns 0.
*
* \param path The path of the file to check for modifications.
*
* \return 1 if the file has been modified since the last recorded modification time.
*         0 if the file has not been modified since the last recorded modification time.
*         -1 if the file does not exist or an error occurs
*/
int hasFileChanged(const char *path);


/**
 * \brief Handles file system events detected by the FSEventStream.
 *
 * This function is called whenever the FSEventStream detects a change in the
 * monitored directory. It prints the path of the changed file and provides
 * the necessary context for further processing.
 *
 * \param streamRef The FSEventStreamRef that triggered the callback.
 * \param clientCallBackInfo A pointer to client-specific data that was passed
 *                          when the stream was created.
 * \param numEvents The number of events in the eventPaths array.
 * \param eventPaths A C array of paths to the files that changed.
 * \param eventFlags An array of FSEventStreamEventFlags for each event.
 * \param eventIds An array of FSEventStreamEventIds for each event.
 */
void handleFileEvents(ConstFSEventStreamRef streamRef,
                      void *clientCallBackInfo,
                      size_t numEvents,
                      void *eventPaths,
                      const FSEventStreamEventFlags eventFlags[],
                      const FSEventStreamEventId eventIds[]) {
    char **paths = (char **)eventPaths;
    for (size_t i = 0; i < numEvents; i++) {
        printf("File system event detected at path: %s\n", paths[i]);

        if (hasFileChanged(paths[i])) {
            printf("File has changed: %s\n", paths[i]);
            updateFileState(paths[i]);

            // Implement the logic here to handle the file change
            // -    Update content.scstate file
            // -    Update .scconfig file
        } else {
            printf("No change detected for file: %s\n", paths[i]);
        }
    }
}

/**
 * \brief Starts a file watcher that monitors a specified directory for changes.
 *
 * This function creates an FSEventStream to detect file system events in the
 * specified directory. It uses a serial dispatch queue to handle the events and
 * calls the handleFileEvents function whenever an event is detected.
 *
 * \param path The path of the directory to monitor.
 * \param bundleID The id of the queue to create.
 *
 * \return void
 *
 * \note This function does not return until the program is terminated.
 */
void startFileWatcher(const char *path, const char *bundleID) {
    FSEventStreamContext context = {0, NULL, NULL, NULL, NULL};
    FSEventStreamRef stream;
    CFStringRef cfPath = CFStringCreateWithCString(NULL, path, kCFStringEncodingUTF8);
    CFArrayRef cfPaths = CFArrayCreate(NULL, (const void **)&cfPath, 1, &kCFTypeArrayCallBacks);

    stream = FSEventStreamCreate(NULL,
                                 &handleFileEvents,
                                 &context,
                                 cfPaths,
                                 kFSEventStreamEventIdSinceNow,
                                 1.0,
                                 kFSEventStreamCreateFlagNone);
    if (stream == NULL) {
        fprintf(stderr, "Failed to create FSEventStream.\n");
        CFRelease(cfPaths);
        CFRelease(cfPath);
        return;
    }

    // bundleID ~> "com.example.ScribbleLab.scwtchdog"
    dispatch_queue_t queue = dispatch_queue_create(bundleID, DISPATCH_QUEUE_SERIAL);
    FSEventStreamSetDispatchQueue(stream, queue);

    printf("Monitoring directory: %s\n", path);

    dispatch_main();

    FSEventStreamStop(stream);
    FSEventStreamInvalidate(stream);
    FSEventStreamRelease(stream);
    CFRelease(cfPaths);
    CFRelease(cfPath);

    FileState *current = fileStateList;
    while (current != NULL) {
        FileState *next = current->next;
        free(current->path);
        free(current);
        current = next;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* wtcd_h */
