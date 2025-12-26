
#ifndef MAPPING_H
#define MAPPING_H

#include <unordered_map>
#include <string>
#include <jni.h>

#include "utils/logs/logger.h"


namespace mapping {
    static std::unordered_map<std::string, std::string> class_mappings;
    static std::unordered_map<std::string, std::string> field_mappings;
    static std::unordered_map<std::string, std::pair<std::string, std::string>> method_mappings;

    bool initialize();

    [[nodiscard]] std::string& get_class_path(const std::string& class_path);
    [[nodiscard]] std::pair<std::string, std::string>& get_method_name_and_signature(const std::string& method_name, const std::string& method_signature);
    [[nodiscard]] std::string& get_field_name(const std::string& field_name);

    template<typename T>
    std::string get_obfuscated_field_signature(const std::string& signature) {
        if (std::is_same_v<T, jobject>) {
            if (signature.substr(0, 14) != "Lnet/minecraft" && signature.substr(0, 15) != "[Lnet/minecraft")
                return signature;

            return "L" + get_class_path(signature.substr(1, signature.size() - 2)) + ";";
        }
        if (std::is_same_v<T, jboolean>)  return "Z";
        if (std::is_same_v<T, jint>)  return "I";
        if (std::is_same_v<T, jfloat>)  return "F";
        if (std::is_same_v<T, jdouble>)  return "D";

        logger::get().error("Unsupported field type");
        return "";
    }
}


#endif //MAPPING_H
