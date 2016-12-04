#pragma once

#include <vector>
#include <iostream>

#include "utility.hpp"
#include "IDropEffectListener.hpp"

class CDropEffect
{
public:

    CDropEffect(IDropEffectListener & listener);

    virtual void picked() = 0;

    NUtility::Position getSpawnPosition(
            NUtility::Position playerPosition,
            std::vector<NUtility::Position> path,
            std::uint32_t up, std::uint32_t down,
            std::uint32_t left, std::uint32_t right);

protected:
    IDropEffectListener & mListener;

private:
    virtual NUtility::Position createSpawnPosition(
            NUtility::Position playerPosition,
            std::vector<NUtility::Position> path,
            std::uint32_t up, std::uint32_t down,
            std::uint32_t left, std::uint32_t right) = 0;

    bool mHasPosition = false;
    NUtility::Position mPosition;
};

