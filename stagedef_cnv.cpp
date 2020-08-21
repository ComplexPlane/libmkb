#include "stagedef_cnv.h"

#include "stagedef.h"
#include "stagedef_ppc.h"
#include "endian.h"

namespace mkb2
{

/** Since stagedef conversion does not happen in the original game at all,
 *  I use C++ features here pretty liberally.
 */

#define OFFSET_TO_PTR(offset) ((void *) ((uintptr_t) ppc_stagedef + (uintptr_t) big_to_native(offset)));

// Convert a big-endian offset in a ppc stagedef to a pointer in the ppc stagedef
static void *offset_to_ptr(StagedefFileHeaderPPC *ppc_stagedef, u32 offset)
{
    return ppc_stagedef + big_to_native(offset);
}

static usize get_native_stagedef_size(StagedefFileHeaderPPC *ppc_stagedef)
{
    usize size = 0;

    size += sizeof(StagedefFileHeader);

    size += sizeof(StagedefCollisionHeader) * ppc_stagedef->collision_header_count;
    auto coli_header_list = (StagedefCollisionHeaderPPC *) OFFSET_TO_PTR(ppc_stagedef->collision_header_list_offset);
    for (u32 i = 0; i < ppc_stagedef->collision_header_count; i++)
    {
        StagedefCollisionHeaderPPC *coli_header = &coli_header_list[i];

        // Determine size of this animation header and keyframes
        if (coli_header->animation_header_offset)
        {
            size += sizeof(StagedefAnimHeader);
            auto anim_header = (StagedefAnimHeaderPPC *) OFFSET_TO_PTR(coli_header->animation_header_offset);

            usize total_keyframes = anim_header->rot_x_keyframe_count
                + anim_header->rot_y_keyframe_count
                + anim_header->rot_z_keyframe_count
                + anim_header->pos_x_keyframe_count
                + anim_header->pos_y_keyframe_count
                + anim_header->pos_z_keyframe_count;
            size += total_keyframes * sizeof(StagedefAnimKeyframe);
        }

        // Determine size of collision triangle list by finding the highest-used collision triangle index
        s32 highest_tri_idx = -1;
        u32 num_tri_lists = coli_header->collision_grid_step_count.x * coli_header->collision_grid_step_count.y;
        auto tri_lists = (u16 **) OFFSET_TO_PTR(coli_header->collision_grid_triangle_idx_list_list_offset);
        for (u32 i = 0; i < num_tri_lists; i++)
        {
            auto tri_list = (u16 *) OFFSET_TO_PTR(tri_lists[i]);
        }
    }

    return size;
}

StagedefFileHeader *stagedef_ppc_to_native(StagedefFileHeaderPPC *ppc_stagedef)
{

}

}