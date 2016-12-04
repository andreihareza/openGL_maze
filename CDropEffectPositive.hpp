#pragma once

#include <memory>

#include "CDropEffect.hpp"

class CDropEffectPositive : public CDropEffect
{
public:
    CDropEffectPositive(IDropEffectListener & listener);

    virtual void picked() override = 0;

    virtual NUtility::Position createSpawnPosition(
            NUtility::Position playerPosition,
            std::vector<NUtility::Position> path,
            std::uint32_t up, std::uint32_t down,
            std::uint32_t left, std::uint32_t right) override;

    static std::unique_ptr<CDropEffect> randomEffect(
            IDropEffectListener & listener);
private:

    enum class Effect : std::uint32_t
    {
        First = 0u,

        SpeedUp = First,

        Size,
    };
};

