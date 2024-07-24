//===-- include/SFCxxJSON.h - JSON En- and Decoding -------------*- C++ -*-===//
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
//     __ _____ _____ _____                                                   //
//  __|  |   __|     |   | |  SFCJSON methods for Scribble Foundation         //
// |  |  |__   |  |  | | | |  Version 1.0                                     //
// |_____|_____|_____|_|___|  https://github.com/ScribbleLabApp/              //
//                                                                            //
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines classes and functions for JSON encoding and decoding.
///
/// This header file provides facilities for encoding JSON data into a string
/// format and decoding JSON data from a string format. It defines the `JSON`
/// class with static methods `encode` and `decode` for performing these operations.
///
/// JSON encoding supports various data types including null, boolean, numeric,
/// string, arrays, and objects. Decoding functions handle JSON objects and arrays
/// recursively, using variants and shared pointers for flexibility and efficiency.
///
///
/// \author ScribbleLabApp
/// \date July 2024
///
//===----------------------------------------------------------------------===//

#ifndef SFCJSON_h
#define SFCJSON_h

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Represents a JSON variant.
 *
 * This typedef defines a JSONVariant as a void pointer. JSONVariant is used to store
 * a variant value that can be of any JSON data type.
 */
typedef void* JSONVariant;

/**
 * @brief Encodes a JSON variant into a string format.
 *
 * This function takes a JSON variant as input and converts it into a string format
 * according to the JSON specification. The encoded string can then be used for further
 * processing or storage.
 *
 * @param value The JSON variant to encode. This can be of any JSON data type, including
 *              null, boolean, numeric, string, arrays, and objects.
 *
 * @return A null-terminated C-style string representing the encoded JSON data. The caller
 *         is responsible for freeing the allocated memory for the returned string.
 *
 * @note The returned string is allocated dynamically and must be deallocated using the
 *       `free` function to avoid memory leaks.
 *
 * @warning The caller is responsible for freeing the allocated memory for the returned
 *          string using the `free` function. Failure to do so may lead to memory leaks.
 */
const char* json_encode(JSONVariant value);

/**
 * @brief Decodes a JSON string into a JSON variant.
 *
 * This function decodes a JSON string into a JSON variant. The JSON string must be
 * a valid UTF-8 encoded string. The decoded JSON variant can be of any JSON data type,
 * including null, boolean, numeric, string, arrays, and objects.
 *
 * @param json The JSON string to decode. The input string must be null-terminated.
 *
 * @return A JSONVariant representing the decoded JSON data. The caller is responsible
 *         for freeing the memory allocated for the JSON variant using the `free_json`
 *         function.
 *
 * @note The `json_decode` function does not validate the input JSON string for correctness.
 *       It assumes that the input string is a valid JSON string. If the input string is
 *       not a valid JSON string, the behavior of the function is undefined.
 *
 * @warning The caller is responsible for freeing the memory allocated for the JSON variant
 *          using the `free_json` function. Failure to do so may lead to memory leaks.
 */
JSONVariant json_decode(const char* json);

/**
 * @brief Frees the memory allocated for a JSON variant.
 *
 * This function releases the memory allocated for a JSON variant. It is
 * essential to call this function after using a JSON variant to prevent memory leaks.
 *
 * @param value The JSON variant to free.
 *
 * @return void.
 *
 * @note This function does not deallocate the memory for the JSON string itself.
 *       It only frees the memory allocated for the JSON variant structure.
 *
 * @warning Do not call this function on a JSON variant that is part of a larger
 *          data structure or that has been copied. Deallocating memory for such
 *          variants may lead to undefined behavior.
 */
void free_json(JSONVariant value);

/**
 * @brief Creates a new JSON object variant.
 *
 * This function allocates memory for a new JSON object variant and initializes it.
 * The returned JSON object variant can be used to store key-value pairs representing
 * JSON objects.
 *
 * @return A JSONVariant representing a new JSON object. The caller is responsible for
 *         freeing the memory allocated for the JSON variant using the `free_json`
 *         function.
 *
 * @note The returned JSONVariant is initially empty. Additional key-value pairs can be
 *       added using the `json_set_string`, `json_set_boolean`, `json_set_object`,
 *       and `json_set_array` functions.
 *
 * @warning The caller is responsible for freeing the memory allocated for the JSON variant
 *          using the `free_json` function. Failure to do so may lead to memory leaks.
 */
JSONVariant json_create_object(void);

/**
 * @brief Creates a new JSON array variant.
 *
 * This function allocates memory for a new JSON array variant and initializes it.
 * The returned JSON array variant can be used to store a sequence of JSON values.
 *
 * @return A JSONVariant representing a new JSON array. The caller is responsible for
 *         freeing the memory allocated for the JSON variant using the `free_json`
 *         function.
 *
 * @note The returned JSONVariant is initially empty. Additional JSON values can be
 *       added using the `json_array_append_object` function.
 *
 * @warning The caller is responsible for freeing the memory allocated for the JSON variant
 *          using the `free_json` function. Failure to do so may lead to memory leaks.
 */
JSONVariant json_create_array(void);

/**
 * @brief Sets a string value for a given key in a JSON object variant.
 *
 * This function sets a string value for a given key in a JSON object variant.
 * If the key already exists in the object, its value will be updated. If the key
 * does not exist, it will be added to the object.
 *
 * @param obj The JSON object variant to modify.
 * @param key The key for which the value will be set. The key must be a null-terminated
 *            C-style string.
 * @param value The string value to set for the given key. The value must be a null-terminated
 *              C-style string.
 *
 * @return void.
 *
 * @note This function does not allocate memory for the key or value strings. It assumes
 *       that the memory for these strings is managed outside of the JSON library.
 *
 * @warning The caller is responsible for ensuring that the memory for the key and value
 *          strings remains valid until the JSON variant is freed using the `free_json`
 *          function. Failure to do so may lead to undefined behavior.
 */
void json_set_string(JSONVariant obj, const char* key, const char* value);

/**
 * @brief Sets a boolean value for a given key in a JSON object variant.
 *
 * This function sets a boolean value for a given key in a JSON object variant.
 * If the key already exists in the object, its value will be updated. If the key
 * does not exist, it will be added to the object.
 *
 * @param obj The JSON object variant to modify. This must be a valid JSON object variant
 *            created using the `json_create_object` function.
 * @param key The key for which the value will be set. The key must be a null-terminated
 *            C-style string. The memory for the key string must remain valid until the
 *            JSON variant is freed using the `free_json` function.
 * @param value The boolean value to set for the given key. A non-zero value represents
 *              `true`, while a zero value represents `false`.
 *
 * @return void.
 *
 * @note This function does not allocate memory for the key string. It assumes that the
 *       memory for the key string is managed outside of the JSON library.
 *
 * @warning The caller is responsible for ensuring that the memory for the key string remains
 *          valid until the JSON variant is freed using the `free_json` function. Failure to
 *          do so may lead to undefined behavior.
 */
void json_set_boolean(JSONVariant obj, const char* key, int value);

/**
 * @brief Sets a JSON object value for a given key in a JSON object variant.
 *
 * This function sets a JSON object value for a given key in a JSON object variant.
 * If the key already exists in the object, its value will be updated. If the key
 * does not exist, it will be added to the object.
 *
 * @param obj The JSON object variant to modify. This must be a valid JSON object variant
 *            created using the `json_create_object` function.
 * @param key The key for which the value will be set. The key must be a null-terminated
 *            C-style string. The memory for the key string must remain valid until the
 *            JSON variant is freed using the `free_json` function.
 * @param value The JSON object value to set for the given key. This must be a valid JSON
 *              object variant created using the `json_create_object` function. The memory
 *              for the value JSON variant must remain valid until the JSON variant is
 *              freed using the `free_json` function.
 *
 * @return void.
 *
 * @note This function does not allocate memory for the key or value JSON variants. It assumes
 *       that the memory for these variants is managed outside of the JSON library.
 *
 * @warning The caller is responsible for ensuring that the memory for the key and value
 *          JSON variants remains valid until the JSON variant is freed using the `free_json`
 *          function. Failure to do so may lead to undefined behavior.
 */
void json_set_object(JSONVariant obj, const char* key, JSONVariant value);

/**
 * @brief Sets a JSON array value for a given key in a JSON object variant.
 *
 * This function sets a JSON array value for a given key in a JSON object variant.
 * If the key already exists in the object, its value will be updated. If the key
 * does not exist, it will be added to the object.
 *
 * @param obj The JSON object variant to modify. This must be a valid JSON object variant
 *            created using the `json_create_object` function.
 * @param key The key for which the value will be set. The key must be a null-terminated
 *            C-style string. The memory for the key string must remain valid until the
 *            JSON variant is freed using the `free_json` function.
 * @param value The JSON array value to set for the given key. This must be a valid JSON
 *              array variant created using the `json_create_array` function. The memory
 *              for the value JSON variant must remain valid until the JSON variant is
 *              freed using the `free_json` function.
 *
 * @return void.
 *
 * @note This function does not allocate memory for the key or value JSON variants. It assumes
 *       that the memory for these variants is managed outside of the JSON library.
 *
 * @warning The caller is responsible for ensuring that the memory for the key and value
 *          JSON variants remains valid until the JSON variant is freed using the `free_json`
 *          function. Failure to do so may lead to undefined behavior.
 */
void json_set_array(JSONVariant obj, const char* key, JSONVariant value);

/**
 * @brief Appends a JSON object variant to a JSON array variant.
 *
 * This function appends a JSON object variant to a JSON array variant. The input JSON object
 * variant is added as a new element at the end of the array.
 *
 * @param array The JSON array variant to which the object will be appended. This must be a
 *              valid JSON array variant created using the `json_create_array` function. The
 *              memory for the array JSON variant must remain valid until the JSON variant is
 *              freed using the `free_json` function.
 * @param value The JSON object variant to append to the array. This must be a valid JSON
 *              object variant created using the `json_create_object` function. The memory
 *              for the value JSON variant must remain valid until the JSON variant is
 *              freed using the `free_json` function.
 *
 * @return void.
 *
 * @note This function does not allocate memory for the array or value JSON variants. It assumes
 *       that the memory for these variants is managed outside of the JSON library.
 *
 * @warning The caller is responsible for ensuring that the memory for the array and value
 *          JSON variants remains valid until the JSON variant is freed using the `free_json`
 *          function. Failure to do so may lead to undefined behavior.
 */
void json_array_append_object(JSONVariant array, JSONVariant value);

#ifdef __cplusplus
}
#endif

#endif /* SFCJSON_h */
