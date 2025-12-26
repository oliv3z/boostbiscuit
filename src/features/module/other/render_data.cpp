
#include "render_data.h"

#include <jni.h>

#include "utils/logs/logger.h"
#include "java/jvm/jvm_manager.h"
#include "features/module/common_data.h"
#include "features/module/module_manager.h"
#include "features/module/modules/render/esp.h"
#include "features/module/register/class_register.h"
#include "wrapper/minecraft/client/Minecraft.h"
#include "wrapper/minecraft/client/renderer/ActiveRenderInfo.h"


std::vector<float> get_blc_model_view(JNIEnv *env) {
    static jclass blc_common_clazz = nullptr;
    static jfieldID model_view_fid = nullptr;
    static jclass FloatBuffer_clazz = nullptr;
    static jmethodID FloatBuffer_get_mid = nullptr;

    if (!blc_common_clazz) {
        blc_common_clazz = env->FindClass("net/badlion/clientcommon/util/aF");
        if (!blc_common_clazz) {
            logger::get().info("BLC client common class not found.");
            return {};
        }
    }

    if (!model_view_fid)
        model_view_fid = env->GetStaticFieldID(blc_common_clazz, "cGc", "Ljava/nio/FloatBuffer;");

    const auto model_view_FloatBuffer = env->GetStaticObjectField(blc_common_clazz, model_view_fid);

    if (!FloatBuffer_clazz)
        FloatBuffer_clazz = env->FindClass("java/nio/FloatBuffer");

    if (!FloatBuffer_get_mid)
        FloatBuffer_get_mid = env->GetMethodID(FloatBuffer_clazz, "get", "(I)F");

    std::vector<float> ret;
    for (int i = 0; i < 16; ++i) {
        float model_view_value = env->CallFloatMethod(model_view_FloatBuffer, FloatBuffer_get_mid, i);
        ret.push_back(model_view_value);
    }

    env->DeleteLocalRef(model_view_FloatBuffer);

    return ret;
}


std::vector<float> get_blc_projection(JNIEnv *env) {
    static jclass blc_common_clazz = nullptr;
    static jfieldID proj_fid = nullptr;
    static jclass Matrix4f_clazz = nullptr;
    static const char* Matrix4f_field_names[16] = {
        "m00","m01","m02","m03",
        "m10","m11","m12","m13",
        "m20","m21","m22","m23",
        "m30","m31","m32","m33"
    };
    static jfieldID Matrix4f_fid_arr[16]{ nullptr };

    if (!blc_common_clazz) {
        blc_common_clazz = env->FindClass("net/badlion/clientcommon/util/aF");
        if (!blc_common_clazz) {
            logger::get().info("BLC client common class not found.");
            return {};
        }
    }

    if (!proj_fid)
        proj_fid = env->GetStaticFieldID(blc_common_clazz, "cFZ", "Lorg/joml/Matrix4f;");

    const auto proj_Matrix4f = env->GetStaticObjectField(blc_common_clazz, proj_fid);

    if (!Matrix4f_clazz)
        Matrix4f_clazz = env->FindClass("org/joml/Matrix4f");

    if (Matrix4f_fid_arr[0] == nullptr) {
        for (int i = 0; i < 16; ++i) {
            const auto fid = env->GetFieldID(Matrix4f_clazz, Matrix4f_field_names[i], "F");
            if (!fid) {
                logger::get().error("Failed to get field ID for Matrix4f field " + std::string(Matrix4f_field_names[i]));
                return {};
            }
            Matrix4f_fid_arr[i] = fid;
        }
    }

    std::vector<float> ret;
    for (const auto &fid : Matrix4f_fid_arr) {
        ret.push_back(env->GetFloatField(proj_Matrix4f, fid));
    }

    return ret;
}


void update_render_data() {
    JNIEnv *env = nullptr;
    if (jvm_manager::get().get_jvm()->AttachCurrentThread((void **)&env, nullptr) != JNI_OK) {
        return logger::get().error("Failed to attach current thread to JVM in wglSwapBuffers::render_esp!");
    }

    game_classes gc;
    if (!class_register::safety_check_and_get_thread_game_classes(env, &gc)) {
        {
            std::lock_guard lock(common_data::esp_render_data_mutex);
            common_data::esp_render_data = esp_snapshot{ {}, {}, {} };
        }
        return logger::get().warn("Failed to get game classes in update_render_data!");
    }

    const std::shared_ptr<wrapper::RenderManager> render_manager = gc.minecraft->get_RenderManager();

    vec3 render_pos = render_manager->get_render_pos();
    if (render_pos.x == 420.f && render_pos.y == 420.f && render_pos.z == 420.f)
        return logger::get().warn("render_pos not set, skipping update_render_data!!");

    const float partial_ticks = gc.minecraft->get_timer()->get_partial_ticks();

    const std::shared_ptr<wrapper::WorldClient> world = gc.minecraft->get_world();

    const std::vector<std::shared_ptr<wrapper::EntityPlayer>> players = world->get_players();
    if (players.empty())  return logger::get().warn("no players found in update_render_data!!");

    esp_snapshot new_render_data = {};

    for (const auto& player : players) {
        if (env->IsSameObject(player->get_obj(), gc.local_player->get_obj()))
            continue;  // skip local player

        const vec3 player_pos = player->get_pos();
        const vec3 player_prev_pos = player->get_prev_pos();
        const std::shared_ptr<wrapper::AxisAlignedBB> bbox = player->getEntityBoundingBox();

        const wrapper::bbox_coords coords = bbox->get_bbox_coords();
        const vec3 interp = (player_pos - player_prev_pos) * partial_ticks;
        const auto min_x = coords.minX - player_pos.x + player_prev_pos.x + interp.x - render_pos.x;
        const auto min_y = coords.minY - player_pos.y + player_prev_pos.y + interp.y - render_pos.y;
        const auto min_z = coords.minZ - player_pos.z + player_prev_pos.z + interp.z - render_pos.z;
        const auto max_x = coords.maxX - player_pos.x + player_prev_pos.x + interp.x - render_pos.x;
        const auto max_y = coords.maxY - player_pos.y + player_prev_pos.y + interp.y - render_pos.y + 0.2;
        const auto max_z = coords.maxZ - player_pos.z + player_prev_pos.z + interp.z - render_pos.z;

        const vec3 bbox_corners[8] {
            { min_x, min_y, min_z },
            { min_x, max_y, min_z },
            { max_x, max_y, min_z },
            { max_x, min_y, min_z },
            { min_x, min_y, max_z },
            { min_x, max_y, max_z },
            { max_x, max_y, max_z },
            { max_x, min_y, max_z }
        };

        entity_snapshot ss;
        ss.bbox = { min_x - 0.1, min_y, min_z - 0.1, max_x + 0.1, max_y - 0.1, max_z + 0.1 };
        memcpy(ss.bbox_corners, bbox_corners, sizeof(bbox_corners));
        ss.distance = gc.local_player->get_distance_to_entity(player);
        ss.health = player->get_health();
        ss.max_health = player->get_max_health();
        ss.is_invisible = player->is_invisible();

        new_render_data.entities.push_back(ss);
    }

    if (jvm_manager::get().get_client_brand() == ClientBrand::BADLION) {
        new_render_data.model_view = get_blc_model_view(env);
        new_render_data.projection = get_blc_projection(env);
    } else {
        new_render_data.model_view = wrapper::ActiveRenderInfo::get_model_view(env);
        new_render_data.projection = wrapper::ActiveRenderInfo::get_projection(env);
    }

    {
        std::lock_guard lock(common_data::esp_render_data_mutex);
        common_data::esp_render_data = new_render_data;
    }
}
