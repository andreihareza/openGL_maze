#pragma once
#include "utility.hpp"

class CCell
{
public:
    void addBorder(Direction direction);

    void printBitmap(std::ostream & file);

    bool hasBorder(Direction direction);

private:
    std::uint32_t mBordersBitmap;
};

