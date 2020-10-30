#pragma once

/*
 * Convert an original Gamecube PowerPC stagedef binary to a native stagedef
 */

namespace mkb2
{

// Forward declarations
struct StagedefFileHeader;
struct StagedefFileHeaderPPC;

StagedefFileHeader *stagedef_ppc_to_native(StagedefFileHeaderPPC *ppc_stagedef);

}