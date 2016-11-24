#pragma once
#include <iostream>
#include <random>
using namespace std::string_literals;

namespace NUtility
{
    const std::uint32_t lines = 10;
    const std::uint32_t columns = 10;

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
}

template <typename E>
constexpr typename std::underlying_type<E>::type toInt(E e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

enum class Direction
{
    Up = 0,
    Down,
    Left,
    Right,
};

