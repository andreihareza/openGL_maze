#include <iomanip>

#include "CCell.hpp"

void CCell::addBorder(Direction direction)
{
    mBordersBitmap |= 1 << static_cast<std::uint32_t>(direction);
}

void CCell::printBitmap(std::ostream & file)
{
    file << std::setw(2) << mBordersBitmap << ' ';
}

bool CCell::hasBorder(Direction direction)
{
    return mBordersBitmap & 1 << static_cast<std::uint32_t>(direction);
}

