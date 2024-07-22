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
//  __|  |   __|     |   | |  SFCxxJSON methods for Scribble Foundation       //
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
/// Example usage:
/// \code
///   std::string jsonString = "{\"key\": \"value\"}";
///   auto jsonVariant = JSON::decode(jsonString);
///   std::string encodedJson = JSON::encode(jsonVariant);
/// \endcode
///
/// \author ScribbleLabApp
/// \date July 2024
///
//===----------------------------------------------------------------------===//

#ifndef SFCxxJSON_h
#define SFCxxJSON_h

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>

namespace sfcxx {

/// \brief Represents a variant of JSON values.
///
/// This variant type can hold null, boolean, numeric, string, arrays of JSON values,
/// and objects (unordered maps) of JSON values.
using JSONVariant = std::variant<std::nullptr_t, bool, double, std::string,
                                 std::vector<std::shared_ptr<struct JSONValue> >,
                                 std::unordered_map<std::string, std::shared_ptr<struct JSONValue>>>;

/// \brief Represents a JSON value.
///
/// This struct serves as the base type for JSON values.
struct JSONValue {
    JSONVariant value;
    JSONValue(JSONVariant val) : value(val) {}
    ~JSONValue() = default;
};

/// \brief Provides methods for encoding and decoding JSON data.
///
/// The JSON class contains static methods to encode JSONVariant data into a string
/// format and to decode JSON data from a string into JSONVariant format.
class JSON {
public:
    /// \brief Encodes a JSONVariant into a JSON string.
    ///
    /// \param value The JSONVariant to encode.
    /// \return The encoded JSON string.
    static std::string encode(const JSONVariant& value);

    /// \brief Decodes a JSON string into a JSONVariant.
    ///
    /// \param json The JSON string to decode.
    /// \return The decoded JSONVariant.
    static JSONVariant decode(const std::string& json);

private:
    /// \brief Recursively encodes a JSONVariant into a JSON string.
    ///
    /// This method is used internally by the encode method.
    ///
    /// \param value The JSONVariant to encode.
    /// \param out The output string to store the encoded JSON.
    static void encodeValue(const JSONVariant& value, std::string& out);
    
    /// \brief Recursively decodes a JSON string into a JSONVariant.
    ///
    /// This method is used internally by the decode method.
    ///
    /// \param json The JSON string to decode.
    /// \param pos The current position in the JSON string.
    /// \return The decoded JSONVariant.
    static JSONVariant decodeValue(const std::string& json, size_t& pos);
    
    
    /// \brief Decodes a JSON object from a JSON string.
    ///
    /// \param json The JSON string to decode.
    /// \param pos The current position in the JSON string.
    /// \return The decoded JSONVariant representing the object.
    static JSONVariant decodeObject(const std::string& json, size_t& pos);
    
    /// \brief Decodes a JSON array from a JSON string.
    ///
    /// \param json The JSON string to decode.
    /// \param pos The current position in the JSON string.
    /// \return The decoded JSONVariant representing the array.
    static JSONVariant decodeArray(const std::string& json, size_t& pos);

    
    /// \brief Skips whitespace characters in a JSON string.
    ///
    /// \param json The JSON string to process.
    /// \param pos The current position in the JSON string.
    static void skipWhitespace(const std::string& json, size_t& pos);
    
    /// \brief Decodes a JSON string value from a JSON string.
    ///
    /// \param json The JSON string to decode.
    /// \param pos The current position in the JSON string.
    /// \return The decoded string value.
    static std::string decodeString(const std::string& json, size_t& pos);
    
    /// \brief Decodes a JSON numeric value from a JSON string.
    ///
    /// \param json The JSON string to decode.
    /// \param pos The current position in the JSON string.
    /// \return The decoded numeric value.
    static double decodeNumber(const std::string& json, size_t& pos);
    
    /// \brief Decodes a JSON boolean value from a JSON string.
    ///
    /// \param json The JSON string to decode.
    /// \param pos The current position in the JSON string.
    /// \return The decoded boolean value.
    static bool decodeBool(const std::string& json, size_t& pos);
    
    /// \brief Decodes a JSON null value from a JSON string.
    ///
    /// \param json The JSON string to decode.
    /// \param pos The current position in the JSON string.
    /// \return The decoded null value.
    static std::nullptr_t decodeNull(const std::string& json, size_t& pos);
};

}

#endif /* SFCxxJSON_h */
