
#ifndef LIST_H
#define LIST_H

#include <memory>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"


BEGIN_WRAP

class List : public java_object {
    static std::shared_ptr<java_class> List_class;
    static inline jmethodID List_toArray_fid = nullptr;

public:
    List(JNIEnv *env, jobject obj);

    [[nodiscard]] jobjectArray to_array() const;
};

END_WRAP


#endif //LIST_H
