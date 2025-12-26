
#ifndef TIMER_H
#define TIMER_H

#include <memory>

#include "java/classes/java_class.h"
#include "java/classes/java_object.h"
#include "wrapper/wrapper.h"


BEGIN_WRAP

class Timer : public java_object {
    static std::shared_ptr<java_class> Timer_class;
    static inline jfieldID renderPartialTicks_mid = nullptr;

public:
    Timer(JNIEnv *env, jobject obj);

    [[nodiscard]] float get_partial_ticks() const;
};

END_WRAP


#endif //TIMER_H
