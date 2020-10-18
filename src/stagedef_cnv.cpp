#include "stagedef_cnv.h"

#include "stagedef.h"
#include "stagedef_ppc.h"
#include "endian.h"

namespace mkb2
{

/**
 * Since stagedef conversion does not happen in the original game at all,
 * I use C++ features here pretty liberally.
 */

static constexpr u16 TRI_IDX_LIST_END = 0xffff;

// Using a class here is convenient since there's a lot of shared context we need to keep track of between functions
class StagedefConverter
{
public:
    StagedefFileHeader convert_stagedef(StagedefFileHeaderPPC *ppc_stagedef)
    {
        m_ppc_stagedef = ppc_stagedef;
    }

// Convert a big-endian offset in a ppc stagedef to a pointer in the ppc stagedef
    void *offset_to_ptr(u32 offset)
    {
        return m_ppc_stagedef + big_to_native(offset);
    }

    usize compute_sizes(StagedefFileHeaderPPC *ppc_stagedef)
    {
        usize size = 0;

        size += sizeof(StagedefFileHeader);

        size += sizeof(StagedefCollisionHeader) * ppc_stagedef->collision_header_count;
        auto coli_header_list = (StagedefCollisionHeaderPPC *) offset_to_ptr(ppc_stagedef->collision_header_list_offset);

        for (u32 i = 0; i < ppc_stagedef->collision_header_count; i++)
        {
            StagedefCollisionHeaderPPC *coli_header = &coli_header_list[i];

            // Determine size of this animation header and keyframes
            if (coli_header->animation_header_offset)
            {
                size += sizeof(StagedefAnimHeader);
                auto anim_header = (StagedefAnimHeaderPPC *) offset_to_ptr(coli_header->animation_header_offset);

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
            auto tri_lists = (u32 *) offset_to_ptr(coli_header->collision_grid_triangle_idx_list_list_offset);
            for (u32 tri_list_idx = 0; i < num_tri_lists; i++)
            {
                auto tri_idx_list = (u16 *) offset_to_ptr(tri_lists[tri_list_idx]);
                for (u32 tri_idx_idx = 0; tri_idx_list[tri_idx_idx] != TRI_IDX_LIST_END; tri_idx_idx++)
                {
                    u16 tri_idx = tri_idx_list[tri_idx_idx];
                    if (tri_idx > highest_tri_idx) highest_tri_idx = tri_idx;
                }
            }
            // TODO panic if `highest_tri_idx == -1`
            size += (highest_tri_idx + 1) * sizeof(StagedefCollisionTri);

            // Record the total number of triangles on the stage
            m_num_tris = highest_tri_idx + 1;
        }

        m_native_stagedef_size = size;
    }

private:
    StagedefFileHeaderPPC *m_ppc_stagedef;
    u32 m_num_tris = 0;
    usize m_native_stagedef_size = 0;
};

StagedefFileHeader *stagedef_ppc_to_native(StagedefFileHeaderPPC *ppc_stagedef)
{

}

}