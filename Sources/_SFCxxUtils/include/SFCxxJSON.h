//
//  SFCxxJSON.hpp
//
//
//  Created by Nevio Hirani on 12.07.24.
//

#ifndef SFCxxJSON_h
#define SFCxxJSON_h

#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>

namespace sfcxx {

struct JSONValue {
    JSONValue() = default;
    ~JSONValue() = default;
};

using JSONValuePtr = std::shared_ptr<JSONValue>;

using JSONVariant = std::variant<std::nullptr_t, bool, double, std::string,
                                 std::vector<JSONValuePtr>,
                                 std::unordered_map<std::string, JSONValuePtr>>;

class JSON {
public:
    static std::string encode(const JSONVariant& value);

    static JSONVariant decode(const std::string& json);

private:
    static void encodeValue(const JSONVariant& value, std::string& out);
    static JSONVariant decodeValue(const std::string& json, size_t& pos);
    
    // Helper functions for decoding specific JSON types
    static JSONVariant decodeObject(const std::string& json, size_t& pos);
    static JSONVariant decodeArray(const std::string& json, size_t& pos);

    static void skipWhitespace(const std::string& json, size_t& pos);
    static std::string decodeString(const std::string& json, size_t& pos);
    static double decodeNumber(const std::string& json, size_t& pos);
    static bool decodeBool(const std::string& json, size_t& pos);
    static std::nullptr_t decodeNull(const std::string& json, size_t& pos);
};

}

#endif /* SFCxxJSON_h */

