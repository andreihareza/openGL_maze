#include <algorithm>

#include "CDropEffectPositive.hpp"
#include "CDropEffectSpeedup.hpp"
#include "utility.hpp"

CDropEffectPositive::CDropEffectPositive(IDropEffectListener & listener)
: CDropEffect(listener)
{
}

NUtility::Position CDropEffectPositive::createSpawnPosition(
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
            (std::binary_search(path.begin(), path.end(), position) == true));

    return position;
}

std::unique_ptr<CDropEffect> CDropEffectPositive::randomEffect(
        IDropEffectListener & listener)
{
    std::unique_ptr<CDropEffect> effect {nullptr};

    Effect effectId = static_cast<Effect>(NUtility::generateRandomBetween(
            toInt(Effect::First), toInt(Effect::Size)-1));

    switch (effectId)
    {
        case Effect::SpeedUp:
            effect = std::make_unique<CDropEffectSpeedup>(listener);
            break;
        default:
            throw 0;
            break;
    }

    return effect;
}

