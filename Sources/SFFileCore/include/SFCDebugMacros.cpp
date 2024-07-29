//===-- include/SFCDebugMacros.cpp - Debug Macros ---------------*- C++ -*-===//
//                                                                            //
// This source file is part of the Scribble Foundation open source project    //
//                                                                            //
// Copyright (c) 2024 ScribbleLabApp. and the ScribbleLab project authors     //
// Licensed under Apache License v2.0 with Runtime Library Exception          //
//                                                                            //
// Copyright (c) 2016 - 2024 Apple Inc. All Rights Reserved.                  //
// Some of the definitions were borrowed from Apple's CoreAudio Utility       //
// Classes.                                                                   //
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

#include "SFCDebugMacros.h"
#include <stdio.h>
#include <stdarg.h>
#if TARGET_API_MAC_OSX
    #include <syslog.h>
#endif

#if DEBUG
#include <stdio.h>

void DebugPrint(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}
#endif /* DEBUG */

#if TARGET_API_MAC_OSX
void LogError(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
#if DEBUG
	vprintf(fmt, args);
#endif
	vsyslog(LOG_ERR, fmt, args);
	va_end(args);
}

void LogWarning(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
#if DEBUG
	vprintf(fmt, args);
#endif
	vsyslog(LOG_WARNING, fmt, args);
	va_end(args);
}
#endif /* TARGET_API_MAC_OSX */