#pragma once

#include <mathtypes.h>

#include <cstdint>

namespace mkb2
{

struct StagedefAnimHeader
{
    uint32_t rot_x_keyframe_count;
    struct StagedefAnimKeyframe *rot_x_keyframe_list;
    uint32_t rot_y_keyframe_count;
    struct StagedefAnimKeyframe *rot_y_keyframe_list;
    uint32_t rot_z_keyframe_count;
    struct StagedefAnimKeyframe *rot_z_keyframe_list;
    uint32_t pos_x_keyframe_count;
    struct StagedefAnimKeyframe *pos_x_keyframe_list;
    uint32_t pos_y_keyframe_count;
    struct StagedefAnimKeyframe *pos_y_keyframe_list;
    uint32_t pos_z_keyframe_count;
    struct StagedefAnimKeyframe *pos_z_keyframe_list;
    uint8_t unk_0x30[16];
} __attribute__((__packed__));

static_assert(sizeof(StagedefAnimHeader) == 0x40);

struct StagedefAnimKeyframe
{
    uint32_t easing;
    float time;
    float value;
    uint8_t unk_0xc[8];
} __attribute__((__packed__));

static_assert(sizeof(StagedefAnimKeyframe) == 0x14);

struct StagedefBackgroundAnim2Header
{
    uint8_t unk_0x0[4];
    float loop_point_seconds;
    uint32_t unk1_keyframe_count;
    struct StagedefAnimKeyframe *unk1_keyframe_list;
    uint32_t unk2_keyframe_count;
    struct StagedefAnimKeyframe *unk2_keyframe_list;
    uint32_t rot_x_keyframe_count;
    struct StagedefAnimKeyframe *rot_x_keyframe_list;
    uint32_t rot_y_keyframe_count;
    struct StagedefAnimKeyframe *rot_y_keyframe_list;
    uint32_t rot_z_keyframe_count;
    struct StagedefAnimKeyframe *rot_z_keyframe_list;
    uint32_t pos_x_keyframe_count;
    struct StagedefAnimKeyframe *pos_x_keyframe_list;
    uint32_t pos_y_keyframe_count;
    struct StagedefAnimKeyframe *pos_y_keyframe_list;
    uint32_t pos_z_keyframe_count;
    struct StagedefAnimKeyframe *pos_z_keyframe_list;
    uint32_t unk9_keyframe_count;
    struct StagedefAnimKeyframe *unk9_keyframe_list;
    uint32_t unk10_keyframe_count;
    struct StagedefAnimKeyframe *unk10_keyframe_list;
    uint32_t unk11_keyframe_count;
    struct StagedefAnimKeyframe *unk11_keyframe_list;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBackgroundAnim2Header) == 0x60);

struct StagedefBackgroundAnimHeader
{
    uint8_t unk_0x0[4];
    float loop_point_seconds;
    uint8_t unk_0x8[8];
    uint32_t rot_x_keyframe_count;
    struct StagedefAnimKeyframe *rot_x_keyframe_list;
    uint32_t rot_y_keyframe_count;
    struct StagedefAnimKeyframe *rot_y_keyframe_list;
    uint32_t rot_z_keyframe_count;
    struct StagedefAnimKeyframe *rot_z_keyframe_list;
    uint32_t pos_x_keyframe_count;
    struct StagedefAnimKeyframe *pos_x_keyframe_list;
    uint32_t pos_y_keyframe_count;
    struct StagedefAnimKeyframe *pos_y_keyframe_list;
    uint32_t pos_z_keyframe_count;
    struct StagedefAnimKeyframe *pos_z_keyframe_list;
    uint8_t unk_0x40[16];
} __attribute__((__packed__));

static_assert(sizeof(StagedefBackgroundAnimHeader) == 0x50);

struct StagedefEffectHeader
{
    uint32_t fx_1_keyframe_count;
    struct StagedefEffect1 *fx_1_keyframe_list;
    uint32_t fx_2_keyframe_count;
    struct StagedefEffect2 *fx_2_keyframe_list;
    struct StagedefTextureScroll *texture_scroll;
    uint8_t unk_0x14[28];
} __attribute__((__packed__));

static_assert(sizeof(StagedefEffectHeader) == 0x30);

struct StagedefEffect1
{ /* Likely used for animated textures, such as the silhouettes in Night */
    uint32_t unk_0x0;
    uint32_t unk_0x4;
    uint32_t unk_0x8;
    uint16_t unk_0x12;
    uint16_t unk_0x14;
    uint16_t unk_0x16;
    uint8_t unk_0x18[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefEffect1) == 0x14);

struct StagedefEffect2
{ /* May be used for the flames in Storm? */
    float unk_0x0;
    float unk_0x4;
    float unk_0x8;
    uint8_t unk_0xc[4];
} __attribute__((__packed__));

static_assert(sizeof(StagedefEffect2) == 0x10);

struct StagedefTextureScroll
{
    struct Vec2f speed;
} __attribute__((__packed__));

static_assert(sizeof(StagedefTextureScroll) == 0x8);

struct StagedefBackgroundModel
{
    uint8_t unk_0x0[4];
    char *model_name;
    uint8_t unk_0x8[4];
    struct Vec3f position;
    struct Vec3s rotation;
    uint8_t padding[2];
    struct Vec3f scale;
    struct StagedefBackgroundAnimHeader *background_anim_header;
    struct StagedefBackgroundAnim2Header *background_anim_2_header;
    struct StagedefEffectHeader *effect_header;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBackgroundModel) == 0x38);

enum BananaType
{
    BANANA_BUNCH = 1,
    BANANA_SINGLE = 0
};

struct StagedefBanana
{
    struct Vec3f position;
    uint32_t type;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBanana) == 0x10);

enum StagedefAnimType
{
    ANIM_LOOP,
    ANIM_ONCE,
    ANIM_SEESAW,
};

enum PlaybackState
{
    PLAYBACK_BACKWARD = 2,
    PLAYBACK_FAST_BACKWARD = 4,
    PLAYBACK_FAST_FORWARD = 3,
    PLAYBACK_FORWARD = 0,
    PLAYBACK_PAUSE = 1
};

struct StagedefStageModelInstance
{
    struct StagedefStageModelPtrA *stage_model_a;
    struct Vec3f position;
    struct Vec3s rotation;
    uint8_t padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModelInstance) == 0x24);

struct StagedefFogAnimHeader
{
    uint32_t start_distance_keyframe_count;
    struct StagedefAnimKeyframe *start_distance_keyframe_list;
    uint32_t end_distance_keyframe_count;
    struct StagedefAnimKeyframe *end_distance_keyframe_list;
    uint32_t red_keyframe_count;
    struct StagedefAnimKeyframe *red_keyframe_list;
    uint32_t green_keyframe_count;
    struct StagedefAnimKeyframe *green_keyframe_list;
    uint32_t blue_keyframe_count;
    struct StagedefAnimKeyframe *blue_keyframe_list;
    uint32_t unk_keyframe_count;
    struct StagedefAnimKeyframe *unk_keyframe_list;
} __attribute__((__packed__));

static_assert(sizeof(StagedefFogAnimHeader) == 0x30);

struct StagedefConeCollision
{
    struct Vec3f position;
    struct Vec3s rotation;
    uint8_t padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefConeCollision) == 0x20);

struct StagedefBumper
{
    struct Vec3f position;
    struct Vec3s rotation;
    uint8_t padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefBumper) == 0x20);

struct StagedefReflectiveStageModel
{
    char *model_name;
    uint8_t unk_0x8[8];
} __attribute__((__packed__));

static_assert(sizeof(StagedefReflectiveStageModel) == 0xc);

struct StagedefFallout
{
    float y;
} __attribute__((__packed__));

static_assert(sizeof(StagedefFallout) == 0x4);

struct StagedefStageModel
{
    uint8_t unk_0x0[4];
    char *model_name;
    uint8_t unk_0x8[8];
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModel) == 0x10);

struct StagedefCollisionHeader
{
    struct Vec3f origin;
    struct Vec3s initial_rotation;
    int16_t anim_loop_type_and_seesaw;
    struct StagedefAnimHeader *animation_header;
    struct Vec3f conveyor_speed;
    struct StagedefCollisionTri *collision_triangle_list;
    uint16_t **collision_grid_triangle_idx_list_list;
    struct Vec2f collision_grid_start;
    struct Vec2f collision_grid_step;
    struct Vec2i collision_grid_step_count;
    uint32_t goal_count;
    struct StagedefGoal *goal_list;
    uint32_t bumper_count;
    struct StagedefBumper *bumper_list;
    uint32_t jamabar_count;
    struct StagedefJamabar *jamabar_list;
    uint32_t banana_count;
    struct StagedefBanana *banana_list;
    uint32_t cone_collision_object_count;
    struct StagedefConeCollision *cone_collision_object_list;
    uint32_t sphere_collision_object_count;
    struct StagedefSphereCollision *sphere_collision_object_list;
    uint32_t cylinder_collision_object_count;
    struct StagedefCylinderCollision *cylinder_collision_object_list;
    uint32_t fallout_volume_count;
    struct StagedefFalloutVolume *fallout_volume_list;
    uint32_t reflective_stage_model_count;
    struct StagedefReflectiveStageModel *reflective_stage_model_list;
    uint32_t stage_model_instance_count;
    struct StagedefStageModelInstance *stage_model_instance_list;
    uint32_t stage_model_b_count;
    struct StagedefStageModelPtrB *stage_model_b_list;
    uint8_t unk_0x9c[8];
    uint16_t anim_group_id; /* For use with buttons - Must be non-zero if you want to assign a group ID */
    uint8_t padding[2];
    uint32_t button_count;
    struct StagedefSwitch *button_list;
    uint8_t unk_0xb0[4];
    struct StagedefMystery5 *mystery5;
    float seesaw_sensitivity; /* Higher is more sensitive, negative makes the seesaw reversed */
    float seesaw_friction; /* Lower is looser */
    float seesaw_spring; /* 0 prevents the seesaw from resetting */
    uint32_t wormhole_count;
    struct stagedef_wormhole *wormhole_list;
    uint32_t initial_playback_state; /* Should this be split into 2x padding bytes + PlaybackState enum? */
    uint8_t unk_0xd0[4];
    float anim_loop_point_seconds;
    struct StagedefTextureScroll *texture_scroll;
    uint8_t unk_0xdc[960];
} __attribute__((__packed__));

static_assert(sizeof(StagedefCollisionHeader) == 0x49c);

struct StagedefStageModelPtrA
{
    uint8_t unk_0x0[8];
    struct StagedefStageModel *stage_model;
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModelPtrA) == 0xc);

struct StagedefForegroundModel
{
    uint8_t unk_0x0[4]; /* Unknown - typically 0x0000001F, sometimes 0x00000007 or 0x0000000F */
    char *model_name;
    uint8_t unk_0x8[4];
    Vec3f position;
    Vec3s rotation;
    uint8_t padding[2];
    Vec3f scale;
    uint8_t unk_0x2c[4];
    struct BackgroundAnim2Header *backgroundAnim2Header;
    void *unk_0x34;
} __attribute__((__packed__));

static_assert(sizeof(StagedefForegroundModel) == 0x38);

struct StagedefStageModelPtrB
{
    struct StagedefStageModelPtrA *stage_model_a;
} __attribute__((__packed__));

static_assert(sizeof(StagedefStageModelPtrB) == 0x4);

struct StagedefMystery5
{
    uint8_t unk_0x0[4];
    float unk_0x4;
    float unk_0x8;
    float unk_0xc;
    float unk_0x10;
} __attribute__((__packed__));

static_assert(sizeof(StagedefMystery5) == 0x14);

struct StagedefSphereCollision
{
    struct Vec3f position;
    float radius;
    uint8_t unk_0x10[4]; /* Nullable */
} __attribute__((__packed__));

static_assert(sizeof(StagedefSphereCollision) == 0x14);

struct StagedefFileHeader
{
    uint32_t magic_number_a;
    uint32_t magic_number_b;
    uint32_t collision_header_count;
    struct StagedefCollisionHeader *collision_header_list;
    struct StagedefStart *start;
    struct StagedefFallout *fallout;
    uint32_t goal_count;
    struct StagedefGoal *goal_list;
    uint32_t bumper_count;
    struct StagedefBumper *bumper_list;
    uint32_t jamabar_count;
    struct StagedefJamabar *jamabar_list;
    uint32_t banana_count;
    struct StagedefBanana *banana_list;
    uint32_t cone_collision_object_count;
    struct StagedefConeCollision *cone_collision_object_list;
    uint32_t sphere_collision_object_count;
    struct StagedefSphereCollision *sphere_collision_object_list;
    uint32_t cylinder_collision_object_count;
    struct StagedefCylinderCollision *cylinder_collision_object_list;
    uint32_t fallout_volume_count;
    struct StagedefFalloutVolume *fallout_volume_list;
    uint32_t background_model_count;
    struct StagedefBackgroundModel *background_model_list;
    uint32_t foreground_model_count;
    struct StagedefForegroundModel *foreground_model_list;
    uint8_t unk_0x68[8];
    uint32_t reflective_stage_model_count;
    struct StagedefReflectiveStageModel *reflective_stage_model_list;
    uint8_t unk_0x78[12];
    uint32_t stage_model_instance_count;
    struct StagedefStageModelInstance *stage_model_instance_list;
    uint32_t stage_model_a_count;
    struct StagedefStageModelPtrA *stage_model_a_list;
    uint32_t stage_model_b_count;
    struct StagedefStageModelPtrB *stage_model_b_list;
    uint8_t unk_0x9c[12];
    uint32_t button_count;
    struct StagedefSwitch *button_list;
    struct StagedefFogAnimHeader *fog_animation_header; /* Nullable */
    uint32_t wormhole_count;
    struct StagedefWormhole *wormhole_list;
    struct StagedefFog *fog; /* Nullable */
    uint8_t unk_0xc0[20];
    struct StagedefMystery3 *mystery3;
    uint8_t unk_0xd8[1988];
} __attribute__((__packed__));

static_assert(sizeof(StagedefFileHeader) == 0x89c);

struct StagedefJamabar
{
    struct Vec3f position;
    struct Vec3s rotation;
    uint8_t padding[2];
    struct Vec3f scale;
} __attribute__((__packed__));

static_assert(sizeof(StagedefJamabar) == 0x20);

struct StagedefMystery3
{
    float unk_0x0;
    float unk_0x4;
    float unk_0x8;
    uint8_t unk_0xc[24];
} __attribute__((__packed__));

static_assert(sizeof(StagedefMystery3) == 0x24);

struct StagedefFalloutVolume
{
    struct Vec3f position;
    struct Vec3f size;
    struct Vec3s rotation;
    uint8_t padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefFalloutVolume) == 0x20);

struct StagedefCollisionTri
{
    struct Vec3f point1_position;
    struct Vec3f normal;
    struct Vec3s rotation_from_xy;
    uint8_t padding[2];
    struct Vec2f point2_delta_pos_from_point1; /* Before rotation is applied */
    struct Vec2f point3_delta_pos_from_point1;
    struct Vec2f tangent;
    struct Vec2f bitangent;
} __attribute__((__packed__));

static_assert(sizeof(StagedefCollisionTri) == 0x40);

struct StagedefGoal
{
    struct Vec3f position;
    struct Vec3s rotation;
    int16_t goal_flags;
} __attribute__((__packed__));

static_assert(sizeof(StagedefGoal) == 0x14);

struct StagedefWormhole
{
    uint8_t field_0x0;
    uint8_t field_0x1;
    uint8_t field_0x2;
    uint8_t field_0x3;
    struct Vec3f positon;
    struct Vec3s rotation;
    uint8_t padding[2];
    struct StagedefWormhole *destination;
} __attribute__((__packed__));

static_assert(sizeof(StagedefWormhole) == 0x1c);

struct StagedefCylinderCollision
{
    struct Vec3f position;
    float radius;
    float height;
    struct Vec3s rotation;
    uint8_t padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefCylinderCollision) == 0x1c);

struct StagedefStart
{
    struct Vec3f position;
    struct Vec3s rotation;
    uint8_t padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefStart) == 0x14);

struct StagedefButton
{
    struct Vec3f position;
    struct Vec3s rotation;
    uint16_t playback_state;
    uint16_t anim_group_id;
    uint8_t padding[2];
} __attribute__((__packed__));

static_assert(sizeof(StagedefSwitch) == 0x18);

struct StagedefFog
{
    uint8_t type; /* Same as GXFogType */
    uint8_t padding[3];
    float fog_start_distance;
    float fog_end_distance;
    struct Vec3f color;
    uint8_t unk_0x18[12];
} __attribute__((__packed__));

static_assert(sizeof(StagedefFog) == 0x24);

}