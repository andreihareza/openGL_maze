#include "CPlayer.hpp"

CPlayer::CPlayer()
 : first(mPosition.first)
 , second(mPosition.second)
{
}

void CPlayer::operator=(std::pair<std::uint32_t, std::uint32_t> position)
{
    mPosition = position;
}

CPlayer::operator std::pair<std::uint32_t, std::uint32_t> () const
{
    return mPosition;
}

void CPlayer::move(Direction direction)
{
    switch(direction)
    {
        case Direction::Up:
            mPosition.first -= 1;
            break;
        case Direction::Down:
            mPosition.first += 1;
            break;
        case Direction::Left:
            mPosition.second -= 1;
            break;
        case Direction::Right:
            mPosition.second += 1;
            break;
        default:
            break;
    }
}
