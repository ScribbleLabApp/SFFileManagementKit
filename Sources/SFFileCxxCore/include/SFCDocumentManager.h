//===-- include/SFCDocumentManager.h - Constants ------------------*- C -*-===//
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
/// \brief Defines functions for managing documents in the SFFileManagementKit project.
///
/// This header file contains function prototypes and error codes related to
/// document management within the SFFileManagementKit project.
///
//===----------------------------------------------------------------------===//

#ifndef SFCDocumentManager_h
#define SFCDocumentManager_h

#include <stdio.h>
#include <stdbool.h>

#define SUCCESS 0                   ///< Indicates successful operation.
#define FILE_OPEN_ERROR -1          ///< Error encountered while opening a file.
#define FILE_WRITE_ERROR -2         ///< Error encountered while writing to a file.
#define FILE_READ_ERROR -3          ///< Error encountered while reading from a file.
#define FILE_CLOSE_ERROR -4         ///< Error encountered while closing a file.

#endif /* SFCDocumentManager_h */
