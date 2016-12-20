#pragma once

#include <iostream>
#include <chrono>
#include <memory>

#include "COpenGLHandler.hpp"
#include "CDropEffect.hpp"
#include "IDropEffectListener.hpp"

class CDropItem
{
public:
    enum class Type : std::uint32_t
    {
        First = 0u,

        Positive = First,
        Negative,

        Size,
    };

    CDropItem(Type, IDropEffectListener & listener);

    bool canAppear();
    bool exists();
    void pick();

    NUtility::Position appear(
        NUtility::Position playerPosition,
        const std::vector<NUtility::Position> & path,
        std::uint32_t up, std::uint32_t down,
        std::uint32_t left, std::uint32_t right);

    NUtility::Position getPosition();
    glm::vec4 getColor();

private:
    NUtility::Position mPosition;
    std::chrono::milliseconds mLastTimeUsed;

    std::unique_ptr<CDropEffect> mEffect;

    const Type mType;

    IDropEffectListener & mListener;
};

