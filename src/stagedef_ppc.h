#pragma once

#include <src/mathtypes.h>

/**
 * SMB2 Stage Definition (original PowerPC)
 *
 * These structures denote a SMB2 Stagedef in its original form as loaded directly on the Gamecube's Gekko PowerPC
 * processor: 32-bit offsets and little-endian byte ordering.
 *
 * These are converted to the platform-local variants defined in `stagedef.h`.
 */

namespace mkb2
{

struct StagedefAnimHeaderPPC
{
    u32 rot_x_keyframe_count;
    u32 rot_x_keyframe_list_offset;
    u32 rot_y_keyframe_count;
    u32 rot_y_keyframe_list_offset;
    u32 rot_z_keyframe_count;
    u32 rot_z_keyframe_list_offset;
    u32 pos_x_keyframe_count;
    u32 pos_x_keyframe_list_offset;
    u32 pos_y_keyframe_count;
    u32 pos_y_keyframe_list_offset;
    u32 pos_z_keyframe_count;
    u32 pos_z_keyframe_list_offset;
    u8 unk_0x30[16];
} __attribute__((__packed__));

static_assert(sizeof(StagedefAnimHeaderPPC) == 0x40);

struct StagedefAnimKeyframePPC
{
    u32 easing;
    f32 time;
    f32 value;
    u8 unk_0xc[8];
} __attribute__((__packed__));

static_assert(sizeof(StagedefAnimKeyframePPC) == 0x14);

struct StagedefBackgroundAnim2HeaderPPC
{
    u8 unk_0x0[4];
    f32 loop_point_seconds;
    u32 unk1_keyframe_count;
    u32 unk1_keyframe_list_offset;
    u32 unk2_keyframe_count;
    u32 unk2_keyframe_list_offset;
    u32 rot_x_keyframe_count;
    u32 rot_x_keyframe_list_offset;
    u32 rot_y_keyframe_count;
    u32 rot_y_keyframe_list_offset;
    u32 rot_z_keyframe_count;
    u32 rot_z_keyframe_list_offset;
    u32 pos_x_keyframe_count;
    u32 pos_x_keyframe_list_offset;
    u32 pos_y_keyframe_count;
    u32 pos_y_keyframe_list_offset;
    u32 pos_z_keyframe_count;
    u32 pos_z_keyframe_list_offset;
    u32 unk9_keyframe_count;
    u32 unk9_keyframe_list_offset;
    u32 unk10_keyframe_count;
    u32 unk10_keyframe_list_offset;
    u32 unk11_keyframe_count;
    u32 unk11_keyframe_list_offset;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBackgroundAnim2HeaderPPC) == 0x60);

struct StagedefBackgroundAnimHeaderPPC
{
    u8 unk_0x0[4];
    f32 loop_point_seconds;
    u8 unk_0x8[8];
    u32 rot_x_keyframe_count;
    u32 rot_x_keyframe_list_offset;
    u32 rot_y_keyframe_count;
    u32 rot_y_keyframe_list_offset;
    u32 rot_z_keyframe_count;
    u32 rot_z_keyframe_list_offset;
    u32 pos_x_keyframe_count;
    u32 pos_x_keyframe_list_offset;
    u32 pos_y_keyframe_count;
    u32 pos_y_keyframe_list_offset;
    u32 pos_z_keyframe_count;
    u32 pos_z_keyframe_list_offset;
    u8 unk_0x40[16];
} __attribute__((__packed__));

static_assert(sizeof(StagedefBackgroundAnimHeaderPPC) == 0x50);

struct StagedefEffectHeaderPPC
{
    u32 fx_1_keyframe_count;
    u32 fx_1_keyframe_list_offset;
    u32 fx_2_keyframe_count;
    u32 fx_2_keyframe_list_offset;
    u32 texture_scroll_offset;
    u8 unk_0x14[28];
} __attribute__((__packed__));

static_assert(sizeof(StagedefEffectHeaderPPC) == 0x30);

struct StagedefEffect1PPC
{ /* Likely used for animated textures, such as the silhouettes in Night */
    u32 unk_0x0;
    u32 unk_0x4;
    u32 unk_0x8;
    u16 unk_0x12;
    u16 unk_0x14;
    u16 unk_0x16;
    u8 unk_0x18[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefEffect1PPC) == 0x14);

struct StagedefEffect2PPC
{ /* May be used for the flames in Storm? */
    f32 unk_0x0;
    f32 unk_0x4;
    f32 unk_0x8;
    u8 unk_0xc[4];
} __attribute__((__packed__));

static_assert(sizeof(StagedefEffect2PPC) == 0x10);

struct StagedefTextureScrollPPC
{
    struct Vec2f speed;
} __attribute__((__packed__));

static_assert(sizeof(StagedefTextureScrollPPC) == 0x8);

struct StagedefBackgroundModelPPC
{
    u8 unk_0x0[4];
    u32 model_name_offset; // Converted to char *
    u8 unk_0x8[4];
    struct Vec3f position;
    struct Vec3s rotation;
    u8 padding[2];
    struct Vec3f scale;
    u32 background_anim_header_offset;
    u32 background_anim_2_header_offset;
    u32 effect_header_offset;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBackgroundModelPPC) == 0x38);

struct StagedefBananaPPC
{
    struct Vec3f position;
    u32 type;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBananaPPC) == 0x10);

struct StagedefStageModelInstancePPC
{
    u32 stage_model_a_offset;
    struct Vec3f position;
    struct Vec3s rotation;
    u8 padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModelInstancePPC) == 0x24);

struct StagedefFogAnimHeaderPPC
{
    u32 start_distance_keyframe_count;
    u32 start_distance_keyframe_list_offset;
    u32 end_distance_keyframe_count;
    u32 end_distance_keyframe_list_offset;
    u32 red_keyframe_count;
    u32 red_keyframe_list_offset;
    u32 green_keyframe_count;
    u32 green_keyframe_list_offset;
    u32 blue_keyframe_count;
    u32 blue_keyframe_list_offset;
    u32 unk_keyframe_count;
    u32 unk_keyframe_list_offset;
} __attribute__((__packed__));

static_assert(sizeof(StagedefFogAnimHeaderPPC) == 0x30);

struct StagedefConeCollisionPPC
{
    struct Vec3f position;
    struct Vec3s rotation;
    u8 padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefConeCollisionPPC) == 0x20);

struct StagedefBumperPPC
{
    struct Vec3f position;
    struct Vec3s rotation;
    u8 padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBumperPPC) == 0x20);

struct StagedefReflectiveStageModelPPC
{
    u32 model_name_offset; // Converted to char *
    u8 unk_0x8[8];
} __attribute__((__packed__));

static_assert(sizeof(StagedefReflectiveStageModelPPC) == 0xc);

struct StagedefFalloutPPC
{
    f32 y;
} __attribute__((__packed__));

static_assert(sizeof(StagedefFalloutPPC) == 0x4);

struct StagedefStageModelPPC
{
    u8 unk_0x0[4];
    u32 model_name_offset; // Converted tdo char *
    u8 unk_0x8[8];
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModelPPC) == 0x10);

struct StagedefCollisionHeaderPPC
{
    struct Vec3f origin;
    struct Vec3s initial_rotation;
    s16 anim_loop_type_and_seesaw;
    u32 animation_header_offset;
    struct Vec3f conveyor_speed;
    u32 collision_triangle_list_offset;
    u32 collision_grid_triangle_idx_list_list_offset; // Converted to u16 **
    struct Vec2f collision_grid_start;
    struct Vec2f collision_grid_step;
    struct Vec2i collision_grid_step_count;
    u32 goal_count;
    u32 goal_list_offset;
    u32 bumper_count;
    u32 bumper_list_offset;
    u32 jamabar_count;
    u32 jamabar_list_offset;
    u32 banana_count;
    u32 banana_list_offset;
    u32 cone_collision_object_count;
    u32 cone_collision_object_list_offset;
    u32 sphere_collision_object_count;
    u32 sphere_collision_object_list_offset;
    u32 cylinder_collision_object_count;
    u32 cylinder_collision_object_list_offset;
    u32 fallout_volume_count;
    u32 fallout_volume_list_offset;
    u32 reflective_stage_model_count;
    u32 reflective_stage_model_list_offset;
    u32 stage_model_instance_count;
    u32 stage_model_instance_list_offset;
    u32 stage_model_b_count;
    u32 stage_model_b_list_offset;
    u8 unk_0x9c[8];
    u16 anim_group_id; /* For use with buttons - Must be non-zero if you want to assign a group ID */
    u8 padding[2];
    u32 button_count;
    u32 button_list_offset;
    u8 unk_0xb0[4];
    u32 mystery5_offset;
    f32 seesaw_sensitivity; /* Higher is more sensitive, negative makes the seesaw reversed */
    f32 seesaw_friction; /* Lower is looser */
    f32 seesaw_spring; /* 0 prevents the seesaw from resetting */
    u32 wormhole_count;
    u32 wormhole_list_offset;
    u32 initial_playback_state; /* Should this be split into 2x padding bytes + PlaybackState enum? */
    u8 unk_0xd0[4];
    f32 anim_loop_point_seconds;
    u32 texture_scroll_offset;
    u8 unk_0xdc[960];
} __attribute__((__packed__));

static_assert(sizeof(StagedefCollisionHeaderPPC) == 0x49c);

struct StagedefStageModelPtrAPPC
{
    u8 unk_0x0[8];
    u32 stage_model_offset;
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModelPtrAPPC) == 0xc);

struct StagedefForegroundModelPPC
{
    u8 unk_0x0[4]; /* Unknown - typically 0x0000001F, sometimes 0x00000007 or 0x0000000F */
    u32 model_name; // Converted to char *
    u8 unk_0x8[4];
    Vec3f position;
    Vec3s rotation;
    u8 padding[2];
    Vec3f scale;
    u8 unk_0x2c[4];
    u32 backgroundAnim2Header_offset;
    u32 unk_offset_0x34;
} __attribute__((__packed__));

static_assert(sizeof(StagedefForegroundModelPPC) == 0x38);

struct StagedefStageModelPtrBPPC
{
    u32 stage_model_a_offset;
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModelPtrBPPC) == 0x4);

struct StagedefMystery5PPC
{
    u8 unk_0x0[4];
    f32 unk_0x4;
    f32 unk_0x8;
    f32 unk_0xc;
    f32 unk_0x10;
} __attribute__((__packed__));

static_assert(sizeof(StagedefMystery5PPC) == 0x14);

struct StagedefSphereCollisionPPC
{
    struct Vec3f position;
    f32 radius;
    u8 unk_0x10[4]; /* Nullable */
} __attribute__((__packed__));

static_assert(sizeof(StagedefSphereCollisionPPC) == 0x14);

struct StagedefFileHeaderPPC
{
    u32 magic_number_a;
    u32 magic_number_b;
    u32 collision_header_count;
    u32 collision_header_list_offset;
    u32 start_offset;
    u32 fallout_offset;
    u32 goal_count;
    u32 goal_list_offset;
    u32 bumper_count;
    u32 bumper_list_offset;
    u32 jamabar_count;
    u32 jamabar_list_offset;
    u32 banana_count;
    u32 banana_list_offset;
    u32 cone_collision_object_count;
    u32 cone_collision_object_list_offset;
    u32 sphere_collision_object_count;
    u32 sphere_collision_object_list_offset;
    u32 cylinder_collision_object_count;
    u32 cylinder_collision_object_list_offset;
    u32 fallout_volume_count;
    u32 fallout_volume_list_offset;
    u32 background_model_count;
    u32 background_model_list_offset;
    u32 foreground_model_count;
    u32 foreground_model_list_offset;
    u8 unk_0x68[8];
    u32 reflective_stage_model_count;
    u32 reflective_stage_model_list_offset;
    u8 unk_0x78[12];
    u32 stage_model_instance_count;
    u32 stage_model_instance_list_offset;
    u32 stage_model_a_count;
    u32 stage_model_a_list_offset;
    u32 stage_model_b_count;
    u32 stage_model_b_list_offset;
    u8 unk_0x9c[12];
    u32 button_count;
    u32 button_list_offset;
    u32 fog_animation_header_offset; /* Nullable */
    u32 wormhole_count;
    u32 wormhole_list_offset;
    u32 fog_offset; /* Nullable */
    u8 unk_0xc0[20];
    u32 mystery3_offset;
    u8 unk_0xd8[1988];
} __attribute__((__packed__));

static_assert(sizeof(StagedefFileHeaderPPC) == 0x89c);

struct StagedefJamabarPPC
{
    struct Vec3f position;
    struct Vec3s rotation;
    u8 padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefJamabarPPC) == 0x20);

struct StagedefMystery3PPC
{
    f32 unk_0x0;
    f32 unk_0x4;
    f32 unk_0x8;
    u8 unk_0xc[24];
} __attribute__((__packed__));

static_assert(sizeof(StagedefMystery3PPC) == 0x24);

struct StagedefFalloutVolumePPC
{
    struct Vec3f position;
    struct Vec3f size;
    struct Vec3s rotation;
    u8 padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefFalloutVolumePPC) == 0x20);

struct StagedefCollisionTriPPC
{
    struct Vec3f point1_position;
    struct Vec3f normal;
    struct Vec3s rotation_from_xy;
    u8 padding[2];
    struct Vec2f point2_delta_pos_from_point1; /* Before rotation is applied */
    struct Vec2f point3_delta_pos_from_point1;
    struct Vec2f tangent;
    struct Vec2f bitangent;
} __attribute__((__packed__));

static_assert(sizeof(StagedefCollisionTriPPC) == 0x40);

struct StagedefGoalPPC
{
    struct Vec3f position;
    struct Vec3s rotation;
    s16 goal_flags;
} __attribute__((__packed__));

static_assert(sizeof(StagedefGoalPPC) == 0x14);

struct StagedefWormholePPC
{
    u8 field_0x0;
    u8 field_0x1;
    u8 field_0x2;
    u8 field_0x3;
    struct Vec3f positon;
    struct Vec3s rotation;
    u8 padding[2];
    u32 destination_offset;
} __attribute__((__packed__));

static_assert(sizeof(StagedefWormholePPC) == 0x1c);

struct StagedefCylinderCollisionPPC
{
    struct Vec3f position;
    f32 radius;
    f32 height;
    struct Vec3s rotation;
    u8 padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefCylinderCollisionPPC) == 0x1c);

struct StagedefStartPPC
{
    struct Vec3f position;
    struct Vec3s rotation;
    u8 padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefStartPPC) == 0x14);

struct StagedefButtonPPC
{
    struct Vec3f position;
    struct Vec3s rotation;
    u16 playback_state;
    u16 anim_group_id;
    u8 padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefButtonPPC) == 0x18);

struct StagedefFogPPC
{
    u8 type; /* Same as GXFogType */
    u8 padding[3];
    f32 fog_start_distance;
    f32 fog_end_distance;
    struct Vec3f color;
    u8 unk_0x18[12];
} __attribute__((__packed__));

static_assert(sizeof(StagedefFogPPC) == 0x24);

}