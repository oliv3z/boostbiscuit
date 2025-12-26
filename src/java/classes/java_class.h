
#ifndef JAVA_CLASS_H
#define JAVA_CLASS_H

#include <jni.h>
#include <string>

#include "mapping/mapping.h"
#include "utils/logs/logger.h"


class java_class {
    JNIEnv* env;
    jclass clazz;
    std::string class_path;

    jclass make_global(jclass obj_) const;

public:
    java_class(JNIEnv* env_, const std::string& class_path_);

    ~java_class();

    [[nodiscard]] jclass get_clazz() const;

    [[nodiscard]] jmethodID get_mid(const std::string& method_name, const std::string& method_signature, bool is_static) const;

    template<typename Ret>
    [[nodiscard]] jfieldID get_field_id(const std::string& fieldName, const std::string& signature, const bool isStatic = false) const {
        const std::string obfFieldName = mapping::get_field_name(class_path + "/" + fieldName);
        const std::string obfSignature = mapping::get_obfuscated_field_signature<Ret>(signature);
        if (obfSignature.empty()) {
            logger::get().info("Signature is empty for field: " + fieldName);
            return nullptr;
        }

        jfieldID fieldId;
        if (isStatic)
            fieldId = env->GetStaticFieldID(clazz, obfFieldName.c_str(), obfSignature.c_str());
        else
            fieldId = env->GetFieldID(clazz, obfFieldName.c_str(), obfSignature.c_str());

        return fieldId;
    }
};


#endif //JAVA_CLASS_H
