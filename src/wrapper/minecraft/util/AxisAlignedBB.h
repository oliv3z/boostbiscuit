
#ifndef AXISALIGNEDBB_H
#define AXISALIGNEDBB_H

#include <memory>

#include "java/classes/java_object.h"
#include "java/classes/java_class.h"
#include "wrapper/wrapper.h"


BEGIN_WRAP

struct bbox_coords {
    double minX;
    double minY;
    double minZ;
    double maxX;
    double maxY;
    double maxZ;
};


class AxisAlignedBB : public java_object {
    static std::shared_ptr<java_class> AxisAlignedBB_class;

    inline static jfieldID min_x_fid = nullptr;
    inline static jfieldID min_y_fid = nullptr;
    inline static jfieldID min_z_fid = nullptr;
    inline static jfieldID max_x_fid = nullptr;
    inline static jfieldID max_y_fid = nullptr;
    inline static jfieldID max_z_fid = nullptr;

public:
    AxisAlignedBB(JNIEnv *env, jobject obj);

    [[nodiscard]] bbox_coords get_bbox_coords();
};

END_WRAP


#endif //AXISALIGNEDBB_H
