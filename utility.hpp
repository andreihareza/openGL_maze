#pragma once
#include <iostream>
#include <random>
#include <chrono>
using namespace std::string_literals;
using namespace std::chrono_literals;

namespace NUtility
{
    const std::uint32_t lines = 10;
    const std::uint32_t columns = 10;

    const constexpr std::chrono::milliseconds moveDuration = 300ms;

    const std::uint32_t timerFuncNum = 1000u;

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

constexpr std::underlying_type<Direction>::type toInt(Direction e)
{
    return static_cast<std::underlying_type<Direction>::type>(e);
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

