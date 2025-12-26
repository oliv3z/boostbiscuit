
#include "List.h"


std::shared_ptr<java_class> wrapper::List::List_class = nullptr;


wrapper::List::List(JNIEnv *env, jobject obj) : java_object(env, obj) {
    List_class = List_class ? List_class : std::make_shared<java_class>(env, "java/util/List");
}


jobjectArray wrapper::List::to_array() const {
    if (!List_toArray_fid)
        List_toArray_fid = List_class->get_mid("toArray", "()[Ljava/lang/Object;", false);

    if (!obj) return nullptr;

    const auto arr = (jobjectArray)env->CallObjectMethod(obj, List_toArray_fid);
    const auto global_arr = (jobjectArray)env->NewGlobalRef(arr);
    env->DeleteLocalRef(arr);

    return global_arr;
}
