//
//  SFCxxJSONBridge.cpp
//  
//
//  Created by Nevio Hirani on 15.07.24.
//

#include "SFCxxJSON.h"

extern "C" {

char* encodeToJson(void* jsonVariant) {
    std::string jsonString = sfcxx::JSON::encode(*static_cast<sfcxx::JSONVariant*>(jsonVariant));
    char* result = (char*)malloc(jsonString.size() + 1);
    strcpy(result, jsonString.c_str());
    return result;
}

void* decodeFromJson(const char* jsonString) {
    sfcxx::JSONVariant* jsonVariant = new sfcxx::JSONVariant(sfcxx::JSON::decode(std::string(jsonString)));
    return static_cast<void*>(jsonVariant);
}

}
