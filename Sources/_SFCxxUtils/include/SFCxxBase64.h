//===-- include/SFCxxBase64.h - Base64 Image Encoding -----------*- C++ -*-===//
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
/// \brief Provides functions to encode and decode Base64 images.
///
//===----------------------------------------------------------------------===//

#ifndef SFCxxBase64_h
#define SFCxxBase64_h

#include <string>
#include <vector>

/// \namespace sfcxx
/// \brief The `sfcxx` namespace contains all the core functionalities for the `SFFileCxxCore` and `_SFCxxUtils` libraries.
///
/// This namespace is the default namespace for the entire `SFFileCxxCore` and `_SFCxxUtils` libraries, 
/// providing utility functions, document handling and other internal functions.
namespace sfcxx {

/// Encodes binary data to a Base64 string.
///
/// This function takes a pointer to a binary data array and its length, and returns the Base64 encoded string.
///
/// \param bytes_to_encode Pointer to the binary data to encode.
/// \param len Length of the binary data.
/// \return Base64 encoded string.
std::string base64_encode(const unsigned char* bytes_to_encode, unsigned int len);

/// Decodes a Base64 string to binary data.
///
/// This function takes a Base64 encoded string and returns the decoded binary data as a vector of unsigned chars.
///
/// \param encoded_string The Base64 encoded string to decode.
/// \return Vector of decoded binary data.
std::vector<unsigned char> base64_decode(const std::string& encoded_string);

/// Reads the entire binary content of an image file into a vector.
///
/// This function takes the filename of an image file and returns its binary content as a vector of unsigned chars.
///
/// \param filename The name of the file to read.
/// \return Vector of binary data read from the file.
std::vector<unsigned char> read_image_file(const std::string& filename);

/// Writes binary data to an image file.
///
/// This function takes a filename and binary data, and writes the data to the file.
///
/// \param filename The name of the file to write.
/// \param data Vector of binary data to write to the file.
void write_image_file(const std::string& filename, const std::vector<unsigned char>& data);

}

#endif /* SFCxxBase64_h */
