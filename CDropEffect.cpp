#include "CDropEffect.hpp"

CDropEffect::CDropEffect(IDropEffectListener & listener)
: mListener(listener)
{
}

NUtility::Position CDropEffect::getSpawnPosition(
        NUtility::Position playerPosition,
        std::vector<NUtility::Position> path,
        std::uint32_t up, std::uint32_t down,
        std::uint32_t left, std::uint32_t right)
{
    if (mHasPosition == false)
    {
        mPosition = createSpawnPosition(playerPosition, path,
                up, down, left, right);

        mHasPosition = true;
    }

    return mPosition;
}

