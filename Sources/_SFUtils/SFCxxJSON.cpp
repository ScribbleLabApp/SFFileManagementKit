//===-- _SFCxxUtils/SFCxxJSON.cpp - JSON En- and Decoding -------*- C++ -*-===//
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

#include "include/SFCxxJSON.h"
#include "include/SFCJSON.h"

#include <sstream>
#include <iomanip>

namespace sfcxx {

std::string JSON::encode(const JSONVariant& value) {
    std::string result;
    encodeValue(value, result);
    return result;
}

JSONVariant JSON::decode(const std::string& json) {
    size_t pos = 0;
    return decodeValue(json, pos);
}

void JSON::encodeValue(const JSONVariant& value, std::string& out) {
    if (std::holds_alternative<std::nullptr_t>(value)) {
        out += "null";
    } else if (std::holds_alternative<bool>(value)) {
        out += std::get<bool>(value) ? "true" : "false";
    } else if (std::holds_alternative<double>(value)) {
        std::ostringstream oss;
        oss << std::setprecision(std::numeric_limits<double>::max_digits10) << std::get<double>(value);
        out += oss.str();
    } else if (std::holds_alternative<std::string>(value)) {
        out += '"';
        out += std::get<std::string>(value);
        out += '"';
    } else if (std::holds_alternative<std::vector<std::shared_ptr<JSONValue>>>(value)) {
        out += '[';
        const auto& vec = std::get<std::vector<std::shared_ptr<JSONValue>>>(value);
        for (auto it = vec.begin(); it != vec.end(); ++it) {
            if (it != vec.begin()) out += ',';
            encodeValue((*it)->value, out);
        }
        out += ']';
    } else if (std::holds_alternative<std::unordered_map<std::string, std::shared_ptr<JSONValue>>>(value)) {
        out += '{';
        const auto& map = std::get<std::unordered_map<std::string, std::shared_ptr<JSONValue>>>(value);
        for (auto it = map.begin(); it != map.end(); ++it) {
            if (it != map.begin()) out += ',';
            out += '"';
            out += it->first;
            out += "\":";
            encodeValue(it->second->value, out);
        }
        out += '}';
    }
}

JSONVariant JSON::decodeValue(const std::string& json, size_t& pos) {
    skipWhitespace(json, pos);
    char current = json[pos];

    if (current == '{') {
        return decodeObject(json, pos);
    } else if (current == '[') {
        return decodeArray(json, pos);
    } else if (current == '"') {
        return decodeString(json, pos);
    } else if (current == 't' || current == 'f') {
        return decodeBool(json, pos);
    } else if (current == 'n') {
        return decodeNull(json, pos);
    } else {
        return decodeNumber(json, pos);
    }
}

JSONVariant JSON::decodeObject(const std::string& json, size_t& pos) {
    std::unordered_map<std::string, std::shared_ptr<JSONValue>> result;
    ++pos; // Skip '{'
    skipWhitespace(json, pos);

    while (pos < json.size() && json[pos] != '}') {
        skipWhitespace(json, pos);
        std::string key = decodeString(json, pos);
        skipWhitespace(json, pos);
        ++pos;                                                              // Skip ':'
        skipWhitespace(json, pos);
        result[key] = std::make_shared<JSONValue>(decodeValue(json, pos));
        skipWhitespace(json, pos);

        if (json[pos] == ',') {
            ++pos;                                                          // Skip ','
            skipWhitespace(json, pos);
        }
    }
    ++pos;                                                                  // Skip '}'

    return result;
}

JSONVariant JSON::decodeArray(const std::string& json, size_t& pos) {
    std::vector<std::shared_ptr<JSONValue>> result;
    ++pos;                                                                  // Skip '['
    skipWhitespace(json, pos);

    while (pos < json.size() && json[pos] != ']') {
        result.push_back(std::make_shared<JSONValue>(decodeValue(json, pos)));
        skipWhitespace(json, pos);

        if (json[pos] == ',') {
            ++pos;                                                          // Skip ','
            skipWhitespace(json, pos);
        }
    }
    ++pos;                                                                  // Skip ']'

    return result;
}

void JSON::skipWhitespace(const std::string& json, size_t& pos) {
    while (pos < json.size() && std::isspace(json[pos])) {
        ++pos;
    }
}

std::string JSON::decodeString(const std::string& json, size_t& pos) {
    std::string result;
    ++pos;                                                                  // Skip opening quote
    while (pos < json.size()) {
        char current = json[pos++];
        if (current == '"') {
            return result;
        } else if (current == '\\') {
            if (pos < json.size()) {
                result += json[pos++];
            }
        } else {
            result += current;
        }
    }
    return result;
}

double JSON::decodeNumber(const std::string& json, size_t& pos) {
    size_t start = pos;
    while (pos < json.size() && (std::isdigit(json[pos]) || json[pos] == '.' || json[pos] == '-')) {
        ++pos;
    }
    std::istringstream iss(json.substr(start, pos - start));
    double result;
    iss >> result;
    return result;
}

bool JSON::decodeBool(const std::string& json, size_t& pos) {
    if (json[pos] == 't') {
        pos += 4; // true
        return true;
    } else {
        pos += 5; // false
        return false;
    }
}

std::nullptr_t JSON::decodeNull(const std::string& json, size_t& pos) {
    pos += 4; // null
    return nullptr;
}

} // namespace sfcxx

extern "C" {
const char* json_encode(JSONVariant value) {
    thread_local std::string encoded;
    encoded = sfcxx::JSON::encode(*reinterpret_cast<const sfcxx::JSONVariant*>(&value));
    
    return encoded.c_str();
}

JSONVariant json_decode(const char* json) {
    auto variant = sfcxx::JSON::decode(json);

    return new sfcxx::JSONValue(variant);
}

void free_json(JSONVariant value) {
    delete static_cast<sfcxx::JSONValue*>(value);
}
}
