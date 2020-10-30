#include "global_state.h"

namespace mkb2
{

std::unique_ptr<GlobalState> gs(std::make_unique<GlobalState>());

}