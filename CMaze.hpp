#pragma once

#include "CCell.hpp"
#include <vector>
#include <tuple>

class CMaze
{
public:
    CMaze();

    std::vector<std::tuple<std::uint32_t, std::uint32_t,
                           std::uint32_t, std::uint32_t>> getWalls();

private:
    void generateMaze(std::uint32_t up, std::uint32_t down,
                      std::uint32_t left, std::uint32_t right);

    void createBorder();

    void fillHorizontalLine(std::uint32_t horizontalLine,
                            std::uint32_t left, std::uint32_t right,
                            bool hole);

    void fillVerticalLine(std::uint32_t verticalLine,
                          std::uint32_t up, std::uint32_t down,
                          bool hole);

    void printMaze(std::ostream & file);

    std::vector<std::vector<CCell>> mMaze;
};

