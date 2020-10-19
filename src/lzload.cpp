#include "../include/lzload.h"

#include "../include/stagedef.h"

#include <cstdint>
#include <cstdio>

#define TRI_IDX_LIST_END 0xffff

#define FIX_BIG_ENDIAN16(val) \
    ({                        \
        static_assert(sizeof(val) == 2); \
        auto tmp = val;       \
        u16 *int_ptr = (u16 *) (&tmp); \
        u8 *byte_ptr = (u8 *) (&tmp);  \
        *int_ptr = (byte_ptr[1] << 0) | (byte_ptr[0] << 8); \
        val = tmp;            \
    })

#define FIX_BIG_ENDIAN32(val) \
    ({                        \
        static_assert(sizeof(val) == 4); \
        auto tmp = val;       \
        u32 *int_ptr = (u32 *) (&tmp); \
        u8 *byte_ptr = (u8 *) (&tmp);  \
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

static void fix_vec2f_endianness(Vec2f *vec)
{
    FIX_BIG_ENDIAN32(vec->x);
    FIX_BIG_ENDIAN32(vec->y);
}

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

static void fix_keyframe_list_endianness(u32 keyframe_count, StagedefAnimKeyframe *keyframe_list)
{
    for (u32 i = 0; i < keyframe_count; i++)
    {
        StagedefAnimKeyframe *keyframe = &keyframe_list[i];
        FIX_BIG_ENDIAN32(keyframe->easing);
        FIX_BIG_ENDIAN32(keyframe->time);
        FIX_BIG_ENDIAN32(keyframe->value);
    }
}

static void fix_collision_header_endianness(StagedefCollisionHeader *coli_header)
{
    fix_vec3f_endianness(&coli_header->origin);
    fix_vec3s_endianness(&coli_header->initial_rotation);
    FIX_BIG_ENDIAN16(coli_header->anim_loop_type_and_seesaw);

    StagedefAnimHeader *anim_header = coli_header->animation_header;
    fix_keyframe_list_endianness(anim_header->rot_x_keyframe_count, anim_header->rot_x_keyframe_list);
    fix_keyframe_list_endianness(anim_header->rot_y_keyframe_count, anim_header->rot_y_keyframe_list);
    fix_keyframe_list_endianness(anim_header->rot_z_keyframe_count, anim_header->rot_z_keyframe_list);
    fix_keyframe_list_endianness(anim_header->pos_x_keyframe_count, anim_header->pos_x_keyframe_list);
    fix_keyframe_list_endianness(anim_header->pos_y_keyframe_count, anim_header->pos_y_keyframe_list);
    fix_keyframe_list_endianness(anim_header->pos_z_keyframe_count, anim_header->pos_z_keyframe_list);

    fix_vec3f_endianness(&coli_header->conveyor_speed);

    // Fix collision triangle and collision grid endianness
    // I don't think we can easily know the total number of triangles, so we'll fix the endianness of triangles
    // we discover in the collision grid for now. This may involve fixing triangles multiple times.
    u32 num_grid_cells = coli_header->collision_grid_step_count.x * coli_header->collision_grid_step_count.y;
    for (u32 cell_idx = 0; cell_idx < num_grid_cells; cell_idx++)
    {
        u16 *tri_idx_list = coli_header->collision_grid_triangle_idx_list_list[cell_idx];
        for (u32 tri_idx_idx = 0; tri_idx_list[tri_idx_idx] != TRI_IDX_LIST_END; tri_idx_idx++)
        {
            FIX_BIG_ENDIAN16(tri_idx_list[tri_idx_idx]);
            StagedefCollisionTri *tri = &coli_header->collision_triangle_list[tri_idx_list[tri_idx_idx]];
            fix_vec3f_endianness(&tri->point1_position);
            fix_vec3f_endianness(&tri->normal);
            fix_vec3s_endianness(&tri->rotation_from_xy);
            fix_vec2f_endianness(&tri->point2_delta_pos_from_point1);
            fix_vec2f_endianness(&tri->point3_delta_pos_from_point1);
            fix_vec2f_endianness(&tri->tangent);
            fix_vec2f_endianness(&tri->bitangent);
        }
    }
    fix_vec2f_endianness(&coli_header->collision_grid_start);
    fix_vec2f_endianness(&coli_header->collision_grid_step);

    // Fix goals
    for (u32 i = 0; i < coli_header->goal_count; i++)
    {
        StagedefGoal *goal = &coli_header->goal_list[i];
        fix_vec3f_endianness(&goal->position);
        fix_vec3s_endianness(&goal->rotation);
        FIX_BIG_ENDIAN16(goal->goal_flags);
    }

    // Fix bumpers
    for (u32 i = 0; i < coli_header->bumper_count; i++)
    {
        StagedefBumper *bumper = &coli_header->bumper_list[i];
        fix_vec3f_endianness(&bumper->position);
        fix_vec3s_endianness(&bumper->rotation);
        fix_vec3f_endianness(&bumper->scale);
    }

    // Fix jamabars
    for (u32 i = 0; i < coli_header->jamabar_count; i++)
    {
        StagedefJamabar *jamabar = &coli_header->jamabar_list[i];
        fix_vec3f_endianness(&jamabar->position);
        fix_vec3s_endianness(&jamabar->rotation);
        fix_vec3f_endianness(&jamabar->scale);
    }

    // Fix bananas
    for (u32 i = 0; i < coli_header->banana_count; i++)
    {
        StagedefBanana *banana = &coli_header->banana_list[i];
        fix_vec3f_endianness(&banana->position);
        FIX_BIG_ENDIAN32(banana->type);
    }

    // Fix cone collision objects
    for (u32 i = 0; i < coli_header->cone_collision_object_count; i++)
    {
        StagedefConeCollision *cone = &coli_header->cone_collision_object_list[i];
        fix_vec3f_endianness(&cone->position);
        fix_vec3s_endianness(&cone->rotation);
        fix_vec3f_endianness(&cone->scale);
    }

    // Fix sphere collision objects
    for (u32 i = 0; i < coli_header->sphere_collision_object_count; i++)
    {
        StagedefSphereCollision *sphere = &coli_header->sphere_collision_object_list[i];
        fix_vec3f_endianness(&sphere->position);
        FIX_BIG_ENDIAN32(sphere->radius);
    }

    // Fix cylinder collision objects
    for (u32 i = 0; i < coli_header->cylinder_collision_object_count; i++)
    {
        StagedefCylinderCollision *cylinder = &coli_header->cylinder_collision_object_list[i];
        fix_vec3f_endianness(&cylinder->position);
        FIX_BIG_ENDIAN32(cylinder->radius);
        FIX_BIG_ENDIAN32(cylinder->height);
        fix_vec3s_endianness(&cylinder->rotation);
    }

    // TODO the rest
}

// Fix the endianness of all stagedef values excluding pointers and list counts
static void fix_stagedef_endianness()
{
    FIX_BIG_ENDIAN32(g_stagedef->magic_number_a);
    FIX_BIG_ENDIAN32(g_stagedef->magic_number_b);

    for (u32 i = 0; i < g_stagedef->collision_header_count; i++)
    {
        fix_collision_header_endianness(&g_stagedef->collision_header_list[i]);
    }
}

void load_stagedef(u32 stage_id)
{
    char stage_lz_filename[32];
    sprintf(stage_lz_filename, "STAGE%03d.lz", stage_id);

    // Vanilla SMB2 uses some DVD-reading functions to read in the compressed stage LZ file.
    // Here we just assume a generic function that translates (stage file name) -> (pointer to compressed LZ)
    void *compressed_lz = read_file(stage_lz_filename);

    u32 compressed_filesize_including_header = OSRoundUp32B(((u32 *) compressed_lz)[0]);
    u32 uncompressed_filesize_not_including_header = OSRoundUp32B(((u32 *) compressed_lz)[1]);
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
    for (u32 i = 0; i < g_stagedef->collision_header_count; i++)
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

            u32 num_grid_cells = coli_header->collision_grid_step_count.x * coli_header->collision_grid_step_count.y;
            for (u32 grid_cell_idx = 0; grid_cell_idx < num_grid_cells; grid_cell_idx++)
            {
                // Manually convert each triangle index list to a pointer without STAGEDEF_OFFSET_TO_PTR for now.
                // Using the macro seems to give some casting issues with reference types... it _may_ work in C
                // and just not in C++ though
                u16 **triangle_idx_list = &coli_header->collision_grid_triangle_idx_list_list[grid_cell_idx];
                FIX_BIG_ENDIAN32(*triangle_idx_list);
                *triangle_idx_list = (u16 *) ((uintptr_t) *triangle_idx_list + (uintptr_t) g_stagedef);
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
    for (u32 i = 0; i < g_stagedef->reflective_stage_model_count; i++)
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
    for (u32 i = 0; i < g_stagedef->stage_model_instance_count; i++)
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
    for (u32 i = 0; i < g_stagedef->stage_model_b_count; i++)
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
    for (u32 i = 0; i < g_stagedef->wormhole_count; i++)
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

    fix_stagedef_endianness();
}

}
