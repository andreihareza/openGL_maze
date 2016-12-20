#pragma once

#include <fstream>

#include "utility.hpp"

class CPlayer
{
public:

    CPlayer();

    void operator=(NUtility::Position position);
    explicit operator NUtility::Position () const;

    void move(Direction direction);
    void move(NUtility::Position to);

    const std::uint32_t & first;
    const std::uint32_t & second;

private:
    NUtility::Position mPosition;
};

