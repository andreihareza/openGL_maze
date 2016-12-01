#pragma once

#include <fstream>

#include "utility.hpp"

class CPlayer
{
public:

    CPlayer();

    void operator=(std::pair<std::uint32_t, std::uint32_t> position);
    explicit operator std::pair<std::uint32_t, std::uint32_t> () const;

    void move(Direction direction);

    const std::uint32_t & first;
    const std::uint32_t & second;

private:
    std::pair<std::uint32_t, std::uint32_t> mPosition;
};

