#pragma once
#include <iostream>
#include <random>
#include <chrono>
using namespace std::string_literals;
using namespace std::chrono_literals;

namespace NUtility
{
    using Position = std::pair<std::uint32_t, std::uint32_t>;

    const std::uint32_t lines = 10;
    const std::uint32_t columns = 10;

    const std::uint32_t timerFuncNum = 1000u;

    const constexpr std::chrono::milliseconds effectCooldown = 15000ms;

    const constexpr std::chrono::milliseconds defaultMoveDuration = 200ms;
    const constexpr std::chrono::milliseconds speedUpMoveDuration = 140ms;
    const constexpr std::chrono::milliseconds restartDuration = 2000ms;

    const std::uint32_t width = 600;
    const std::uint32_t height = 600;
    const std::string gameName = "Maze"s;

    template<typename T>
    int generateRandomBetween(T left, T right)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(left, right);

        return dis(gen);
    }

    std::chrono::milliseconds getCurrentTime();
} // namespace NUtility


enum class Direction : std::uint32_t
{
    Up = 0,
    Down,
    Left,
    Right,
    Size,
};

template<class ENUM>
constexpr std::underlying_type<Direction>::type toInt(ENUM e)
{
    return static_cast<typename std::underlying_type<ENUM>::type>(e);
}

constexpr const char * toString(Direction e)
{
    switch (e)
    {
        case Direction::Up:
            return "Up";
            break;
        case Direction::Down:
            return "Down";
            break;
        case Direction::Left:
            return "Left";
            break;
        case Direction::Right:
            return "Right";
            break;
        default:
            return "";
            break;
    }
}

