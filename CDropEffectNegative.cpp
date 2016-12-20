#include <algorithm>

#include "CDropEffectNegative.hpp"
#include "CDropEffectRotate.hpp"
#include "CDropEffectZoom.hpp"
#include "utility.hpp"

CDropEffectNegative::CDropEffectNegative(IDropEffectListener & listener)
: CDropEffect(listener)
{
}

NUtility::Position CDropEffectNegative::createSpawnPosition(
        NUtility::Position playerPosition,
        std::vector<NUtility::Position> path,
        std::uint32_t up, std::uint32_t down,
        std::uint32_t left, std::uint32_t right)
{
    NUtility::Position position = {0u, 0u};

    sort(path.begin(), path.end());

    do
    {
        std::uint32_t line = NUtility::generateRandomBetween(up, down);
        std::uint32_t column = NUtility::generateRandomBetween(left, right);

        position = {line, column};
    } while ((position == playerPosition) ||
            (std::binary_search(path.begin(), path.end(), position) == false));

    return position;
}

std::unique_ptr<CDropEffect> CDropEffectNegative::randomEffect(
        IDropEffectListener & listener)
{
    std::unique_ptr<CDropEffect> effect {nullptr};

    Effect effectId = static_cast<Effect>(NUtility::generateRandomBetween(
            toInt(Effect::First), toInt(Effect::Size)-1));

    switch (effectId)
    {
        case Effect::Rotate:
            effect = std::make_unique<CDropEffectRotate>(listener);
            break;
        case Effect::Zoom:
            effect = std::make_unique<CDropEffectZoom>(listener);
            break;
        default:
            throw 0;
            break;
    }

    return effect;
}

