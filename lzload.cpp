#include "lzload.h"

#include "stagedef.h"

#include <cstdint>
#include <cstdio>

#define FIX_BIG_ENDIAN16(val) \
    ({                        \
        static_assert(sizeof(val) == 2); \
        auto tmp = val;       \
        uint16_t *int_ptr = (uint16_t *) (&tmp); \
        uint8_t *byte_ptr = (uint8_t *) (&tmp);  \
        *int_ptr = (byte_ptr[1] << 0) | (byte_ptr[0] << 8); \
        val = tmp;            \
    })

#define FIX_BIG_ENDIAN32(val) \
    ({                        \
        static_assert(sizeof(val) == 4); \
        auto tmp = val;       \
        uint32_t *int_ptr = (uint32_t *) (&tmp); \
        uint8_t *byte_ptr = (uint8_t *) (&tmp);  \
        *int_ptr = (byte_ptr[3] << 0) | (byte_ptr[2] << 8) | (byte_ptr[1] << 16) | (byte_ptr[0] << 24); \
        val = tmp;            \
    })

// We wouldn't need to do this much pointer casting if we were using C instead of C++
// (C doesn't even have decltype)
#define STAGEDEF_OFFSET_TO_PTR(ptr) \
    if (ptr) ptr = (decltype(ptr)) ((uintptr_t)ptr + (uintptr_t)g_stagedef)

#define STAGEDEF_OFFSET_TO_PTR_NOCHECK(ptr) \
    ptr = (decltype(ptr)) ((uintptr_t)ptr + (uintptr_t)g_stagedef)

namespace mkb2
{

StagedefFileHeader *g_stagedef;

static void fix_vec3f_endianness(Vec3f *vec)
{
    FIX_BIG_ENDIAN32(vec->x);
    FIX_BIG_ENDIAN32(vec->y);
    FIX_BIG_ENDIAN32(vec->z);
}

static void fix_vec2s_endianness(Vec2i *vec)
{
    FIX_BIG_ENDIAN32(vec->x);
    FIX_BIG_ENDIAN32(vec->y);
}

static void fix_vec3s_endianness(Vec3s *vec)
{
    FIX_BIG_ENDIAN16(vec->x);
    FIX_BIG_ENDIAN16(vec->y);
    FIX_BIG_ENDIAN16(vec->z);
}

// Fix the endianness of all stagedef values excluding pointers and list counts
static void fix_remaining_stagedef_endianness()
{
    FIX_BIG_ENDIAN32(g_stagedef->magic_number_a);
    FIX_BIG_ENDIAN32(g_stagedef->magic_number_b);
}

void load_stagedef(uint32_t stage_id)
{
    char stage_lz_filename[32];
    sprintf(stage_lz_filename, "STAGE%03d.lz", stage_id);

    // Vanilla SMB2 uses some DVD-reading functions to read in the compressed stage LZ file.
    // Here we just assume a generic function that translates (stage file name) -> (pointer to compressed LZ)
    void *compressed_lz = read_file(stage_lz_filename);

    uint32_t compressed_filesize_including_header = OSRoundUp32B(((uint32_t *) compressed_lz)[0]);
    uint32_t uncompressed_filesize_not_including_header = OSRoundUp32B(((uint32_t *) compressed_lz)[1]);
    void *uncompressed_lz = MKB2_ALLOC_OR_PANIC(uncompressed_filesize_not_including_header);
    decompress_lz(compressed_lz, uncompressed_lz);

    // Vanilla SMB2 frees the uncompressed lz buffer here
    g_stagedef = (StagedefFileHeader *) uncompressed_lz;
    if (!g_stagedef) OSPanic("cannot open stcoli");

    /*
     * The stagedef frequently contains offsets from the beginning of the stagedef
     * to other parts of the stagedef where more stuff is located.
     * For more efficient future access, the game converts these offsets into pointers, based on where
     * the uncompressed stagedef was allocated in memory.
     *
     * We also need to convert the values in the stagedef from big-endian to the current platform's
     * endianness. Endian conversion was not performed in the original game as the big-endian stagedef was meant to
     * be loaded by the Gamecube's big-endian PowerPC processor. All endianness-related calls do not exist in any
     * form in the original game.
     *
     * Fixing endianness is done in two phases:
     * 1. The endianness of each offset is fixed just prior to converting the offset to a pointer.
     *    If the offset corresponds to a list pointer, the count of elements in the list is also converted.
     * 2. The endianness of the remaining data is performed all at once after converting all offsets to pointers.
     */

    FIX_BIG_ENDIAN32(g_stagedef->collision_header_list);
    FIX_BIG_ENDIAN32(g_stagedef->collision_header_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->collision_header_list);
    for (uint32_t i = 0; i < g_stagedef->collision_header_count; i++)
    {
        StagedefCollisionHeader *coli_header = &g_stagedef->collision_header_list[i];

        FIX_BIG_ENDIAN32(coli_header->animation_header);
        STAGEDEF_OFFSET_TO_PTR(coli_header->animation_header);
        if (coli_header->animation_header)
        {
            FIX_BIG_ENDIAN32(coli_header->animation_header->rot_x_keyframe_list);
            FIX_BIG_ENDIAN32(coli_header->animation_header->rot_x_keyframe_count);
            STAGEDEF_OFFSET_TO_PTR(coli_header->animation_header->rot_x_keyframe_list);

            FIX_BIG_ENDIAN32(coli_header->animation_header->rot_y_keyframe_list);
            FIX_BIG_ENDIAN32(coli_header->animation_header->rot_y_keyframe_count);
            STAGEDEF_OFFSET_TO_PTR(coli_header->animation_header->rot_y_keyframe_list);

            FIX_BIG_ENDIAN32(coli_header->animation_header->rot_z_keyframe_list);
            FIX_BIG_ENDIAN32(coli_header->animation_header->rot_z_keyframe_count);
            STAGEDEF_OFFSET_TO_PTR(coli_header->animation_header->rot_z_keyframe_list);

            FIX_BIG_ENDIAN32(coli_header->animation_header->pos_x_keyframe_list);
            FIX_BIG_ENDIAN32(coli_header->animation_header->pos_x_keyframe_count);
            STAGEDEF_OFFSET_TO_PTR(coli_header->animation_header->pos_x_keyframe_list);

            FIX_BIG_ENDIAN32(coli_header->animation_header->pos_y_keyframe_list);
            FIX_BIG_ENDIAN32(coli_header->animation_header->pos_y_keyframe_count);
            STAGEDEF_OFFSET_TO_PTR(coli_header->animation_header->pos_y_keyframe_list);

            FIX_BIG_ENDIAN32(coli_header->animation_header->pos_z_keyframe_list);
            FIX_BIG_ENDIAN32(coli_header->animation_header->pos_z_keyframe_count);
            STAGEDEF_OFFSET_TO_PTR(coli_header->animation_header->pos_z_keyframe_list);
        }

        FIX_BIG_ENDIAN32(coli_header->collision_triangle_list);
        STAGEDEF_OFFSET_TO_PTR(coli_header->collision_triangle_list);

        // Convert the offsets representing collision triangle index lists to pointers
        // ... confusing I know
        if (coli_header->collision_grid_triangle_idx_list_list)
        {
            FIX_BIG_ENDIAN32(coli_header->collision_grid_triangle_idx_list_list);
            fix_vec2s_endianness(&coli_header->collision_grid_step_count);
            STAGEDEF_OFFSET_TO_PTR_NOCHECK(coli_header->collision_grid_triangle_idx_list_list);

            uint32_t num_grid_cells = coli_header->collision_grid_step_count.x * coli_header->collision_grid_step_count.y;
            for (uint32_t grid_cell_idx = 0; grid_cell_idx < num_grid_cells; grid_cell_idx++)
            {
                // Manually convert each triangle index list to a pointer without STAGEDEF_OFFSET_TO_PTR for now.
                // Using the macro seems to give some casting issues with reference types... it _may_ work in C
                // and just not in C++ though
                uint16_t **triangle_idx_list = &coli_header->collision_grid_triangle_idx_list_list[grid_cell_idx];
                FIX_BIG_ENDIAN32(*triangle_idx_list);
                *triangle_idx_list = (uint16_t *) ((uintptr_t) *triangle_idx_list + (uintptr_t) g_stagedef);
            }
        }

        FIX_BIG_ENDIAN32(coli_header->goal_list);
        FIX_BIG_ENDIAN32(coli_header->goal_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->goal_list);

        FIX_BIG_ENDIAN32(coli_header->bumper_list);
        FIX_BIG_ENDIAN32(coli_header->bumper_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->bumper_list);

        FIX_BIG_ENDIAN32(coli_header->jamabar_list);
        FIX_BIG_ENDIAN32(coli_header->jamabar_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->jamabar_list);

        FIX_BIG_ENDIAN32(coli_header->banana_list);
        FIX_BIG_ENDIAN32(coli_header->banana_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->banana_list);

        FIX_BIG_ENDIAN32(coli_header->cone_collision_object_list);
        FIX_BIG_ENDIAN32(coli_header->cone_collision_object_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->cone_collision_object_list);

        FIX_BIG_ENDIAN32(coli_header->sphere_collision_object_list);
        FIX_BIG_ENDIAN32(coli_header->sphere_collision_object_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->sphere_collision_object_list);

        FIX_BIG_ENDIAN32(coli_header->cylinder_collision_object_list);
        FIX_BIG_ENDIAN32(coli_header->cylinder_collision_object_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->cylinder_collision_object_list);

        FIX_BIG_ENDIAN32(coli_header->fallout_volume_list);
        FIX_BIG_ENDIAN32(coli_header->fallout_volume_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->fallout_volume_list);

        FIX_BIG_ENDIAN32(coli_header->reflective_stage_model_list);
        FIX_BIG_ENDIAN32(coli_header->reflective_stage_model_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->reflective_stage_model_list);

        FIX_BIG_ENDIAN32(coli_header->stage_model_instance_list);
        FIX_BIG_ENDIAN32(coli_header->stage_model_instance_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->stage_model_instance_list);

        FIX_BIG_ENDIAN32(coli_header->stage_model_b_list);
        FIX_BIG_ENDIAN32(coli_header->stage_model_b_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->stage_model_b_list);

        FIX_BIG_ENDIAN32(coli_header->button_list);
        FIX_BIG_ENDIAN32(coli_header->button_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->button_list);

        FIX_BIG_ENDIAN32(coli_header->mystery5);
        STAGEDEF_OFFSET_TO_PTR(coli_header->mystery5);

        FIX_BIG_ENDIAN32(coli_header->wormhole_list);
        FIX_BIG_ENDIAN32(coli_header->wormhole_count);
        STAGEDEF_OFFSET_TO_PTR(coli_header->wormhole_list);

        // TODO uncover field 0xa0

        FIX_BIG_ENDIAN32(coli_header->texture_scroll);
        STAGEDEF_OFFSET_TO_PTR(coli_header->texture_scroll);
    }

    FIX_BIG_ENDIAN32(g_stagedef->start);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->start);

    FIX_BIG_ENDIAN32(g_stagedef->fallout);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->fallout);

    FIX_BIG_ENDIAN32(g_stagedef->goal_list);
    FIX_BIG_ENDIAN32(g_stagedef->goal_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->goal_list);

    FIX_BIG_ENDIAN32(g_stagedef->bumper_list);
    FIX_BIG_ENDIAN32(g_stagedef->bumper_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->bumper_list);

    FIX_BIG_ENDIAN32(g_stagedef->jamabar_list);
    FIX_BIG_ENDIAN32(g_stagedef->jamabar_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->jamabar_list);

    FIX_BIG_ENDIAN32(g_stagedef->banana_list);
    FIX_BIG_ENDIAN32(g_stagedef->banana_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->banana_list);

    FIX_BIG_ENDIAN32(g_stagedef->cone_collision_object_list);
    FIX_BIG_ENDIAN32(g_stagedef->cone_collision_object_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->cone_collision_object_list);

    FIX_BIG_ENDIAN32(g_stagedef->sphere_collision_object_list);
    FIX_BIG_ENDIAN32(g_stagedef->sphere_collision_object_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->sphere_collision_object_list);

    FIX_BIG_ENDIAN32(g_stagedef->cylinder_collision_object_list);
    FIX_BIG_ENDIAN32(g_stagedef->cylinder_collision_object_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->cylinder_collision_object_list);

    FIX_BIG_ENDIAN32(g_stagedef->fallout_volume_list);
    FIX_BIG_ENDIAN32(g_stagedef->fallout_volume_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->fallout_volume_list);

    FIX_BIG_ENDIAN32(g_stagedef->background_model_list);
    FIX_BIG_ENDIAN32(g_stagedef->background_model_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->background_model_list);

    FIX_BIG_ENDIAN32(g_stagedef->foreground_model_list);
    FIX_BIG_ENDIAN32(g_stagedef->foreground_model_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->foreground_model_list);

    // Fix reflective stage models
    FIX_BIG_ENDIAN32(g_stagedef->reflective_stage_model_list);
    FIX_BIG_ENDIAN32(g_stagedef->reflective_stage_model_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->reflective_stage_model_list);
    for (uint32_t i = 0; i < g_stagedef->reflective_stage_model_count; i++)
    {
        StagedefReflectiveStageModel *reflective_model = &g_stagedef->reflective_stage_model_list[i];
        FIX_BIG_ENDIAN32(reflective_model->model_name);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(reflective_model->model_name);
    }

    // TODO uncover field 0x80

    // Fix stage model instances
    FIX_BIG_ENDIAN32(g_stagedef->stage_model_instance_list);
    FIX_BIG_ENDIAN32(g_stagedef->stage_model_instance_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->stage_model_instance_list);
    for (uint32_t i = 0; i < g_stagedef->stage_model_instance_count; i++)
    {
        StagedefStageModelInstance *model_instance = &g_stagedef->stage_model_instance_list[i];
        FIX_BIG_ENDIAN32(model_instance->stage_model_a);
        STAGEDEF_OFFSET_TO_PTR(model_instance->stage_model_a);
    }

    FIX_BIG_ENDIAN32(g_stagedef->stage_model_a_list);
    FIX_BIG_ENDIAN32(g_stagedef->stage_model_a_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->stage_model_a_list);

    // Fix stage model b stuff
    FIX_BIG_ENDIAN32(g_stagedef->stage_model_b_list);
    FIX_BIG_ENDIAN32(g_stagedef->stage_model_b_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->stage_model_b_list);
    for (uint32_t i = 0; i < g_stagedef->stage_model_b_count; i++)
    {
        StagedefStageModelPtrB *model_b = &g_stagedef->stage_model_b_list[i];
        FIX_BIG_ENDIAN32(model_b->stage_model_a);
        STAGEDEF_OFFSET_TO_PTR(model_b->stage_model_a);
    }

    FIX_BIG_ENDIAN32(g_stagedef->button_list);
    FIX_BIG_ENDIAN32(g_stagedef->button_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->button_list);

    // Fix fog animation
    if (g_stagedef->fog_animation_header)
    {
        FIX_BIG_ENDIAN32(g_stagedef->fog_animation_header);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(g_stagedef->fog_animation_header);

        StagedefFogAnimHeader *fog_anim_header = g_stagedef->fog_animation_header;

        FIX_BIG_ENDIAN32(fog_anim_header->start_distance_keyframe_list);
        FIX_BIG_ENDIAN32(fog_anim_header->start_distance_keyframe_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(fog_anim_header->start_distance_keyframe_list);

        FIX_BIG_ENDIAN32(fog_anim_header->end_distance_keyframe_list);
        FIX_BIG_ENDIAN32(fog_anim_header->end_distance_keyframe_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(fog_anim_header->end_distance_keyframe_list);

        FIX_BIG_ENDIAN32(fog_anim_header->red_keyframe_list);
        FIX_BIG_ENDIAN32(fog_anim_header->red_keyframe_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(fog_anim_header->red_keyframe_list);

        FIX_BIG_ENDIAN32(fog_anim_header->green_keyframe_list);
        FIX_BIG_ENDIAN32(fog_anim_header->green_keyframe_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(fog_anim_header->green_keyframe_list);

        FIX_BIG_ENDIAN32(fog_anim_header->blue_keyframe_list);
        FIX_BIG_ENDIAN32(fog_anim_header->blue_keyframe_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(fog_anim_header->blue_keyframe_list);

        FIX_BIG_ENDIAN32(fog_anim_header->unk_keyframe_list);
        FIX_BIG_ENDIAN32(fog_anim_header->unk_keyframe_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(fog_anim_header->unk_keyframe_list);
    }

    FIX_BIG_ENDIAN32(g_stagedef->fog);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->fog);

    // Fix wormholes
    FIX_BIG_ENDIAN32(g_stagedef->wormhole_list);
    FIX_BIG_ENDIAN32(g_stagedef->wormhole_count);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->wormhole_list);
    for (uint32_t i = 0; i < g_stagedef->wormhole_count; i++)
    {
        StagedefWormhole *wormhole = &g_stagedef->wormhole_list[i];
        FIX_BIG_ENDIAN32(wormhole->destination);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(wormhole->destination);
    }

    FIX_BIG_ENDIAN32(g_stagedef->mystery3);
    STAGEDEF_OFFSET_TO_PTR(g_stagedef->mystery3);

    if (g_stagedef->background_model_list)
    {
        FIX_BIG_ENDIAN32(g_stagedef->background_model_list);
        FIX_BIG_ENDIAN32(g_stagedef->background_model_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(g_stagedef->background_model_list);
        // TODO there's a lot of corrections done to background model stuff with fiends that aren't documented
    }

    if (g_stagedef->foreground_model_list)
    {
        FIX_BIG_ENDIAN32(g_stagedef->foreground_model_list);
        FIX_BIG_ENDIAN32(g_stagedef->foreground_model_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(g_stagedef->foreground_model_list);
        // TODO same with foreground models as with background models...
    }

    if (g_stagedef->stage_model_a_list)
    {
        FIX_BIG_ENDIAN32(g_stagedef->stage_model_a_list);
        FIX_BIG_ENDIAN32(g_stagedef->stage_model_a_count);
        STAGEDEF_OFFSET_TO_PTR_NOCHECK(g_stagedef->stage_model_a_list);
        // TODO same with model a stuff...
    }

    fix_remaining_stagedef_endianness();
}

}
