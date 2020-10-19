#pragma once

namespace mkb2
{

enum BananaType
{
    BANANA_BUNCH = 1,
    BANANA_SINGLE = 0
};

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

}