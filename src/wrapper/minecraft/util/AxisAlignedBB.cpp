
#include "AxisAlignedBB.h"


std::shared_ptr<java_class> wrapper::AxisAlignedBB::AxisAlignedBB_class = nullptr;


wrapper::AxisAlignedBB::AxisAlignedBB(JNIEnv *env, jobject obj) : java_object(env, obj) {
    AxisAlignedBB_class = AxisAlignedBB_class ? AxisAlignedBB_class : std::make_shared<java_class>(env, "net/minecraft/util/AxisAlignedBB");
}


wrapper::bbox_coords wrapper::AxisAlignedBB::get_bbox_coords() {
    if (!min_x_fid || !min_y_fid || !min_z_fid || !max_x_fid || !max_y_fid || !max_z_fid) {
        min_x_fid = AxisAlignedBB_class->get_field_id<double>("minX", "D", false);
        min_y_fid = AxisAlignedBB_class->get_field_id<double>("minY", "D", false);
        min_z_fid = AxisAlignedBB_class->get_field_id<double>("minZ", "D", false);
        max_x_fid = AxisAlignedBB_class->get_field_id<double>("maxX", "D", false);
        max_y_fid = AxisAlignedBB_class->get_field_id<double>("maxY", "D", false);
        max_z_fid = AxisAlignedBB_class->get_field_id<double>("maxZ", "D", false);
    }

    return {
        env->GetDoubleField(obj, min_x_fid),
        env->GetDoubleField(obj, min_y_fid),
        env->GetDoubleField(obj, min_z_fid),
        env->GetDoubleField(obj, max_x_fid),
        env->GetDoubleField(obj, max_y_fid),
        env->GetDoubleField(obj, max_z_fid)
    };
}
