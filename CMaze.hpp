#pragma once

#include "CCell.hpp"
#include <vector>
#include <tuple>

class CMaze
{
public:
    CMaze();

    void generateMaze(std::uint32_t up, std::uint32_t down,
                      std::uint32_t left, std::uint32_t right);

    std::vector<std::tuple<std::uint32_t, std::uint32_t,
                std::uint32_t, std::uint32_t>> getWalls();

    std::vector<std::pair<
        std::uint32_t, std::uint32_t>> getLongestPath();

    bool canMove(std::pair<int, int> from, Direction dir);

private:

    void createBorder();

    void generateMazeRec(std::uint32_t up, std::uint32_t down,
                      std::uint32_t left, std::uint32_t right);

    void fillHorizontalLine(std::uint32_t horizontalLine,
                            std::uint32_t left, std::uint32_t right,
                            bool hole);

    void fillVerticalLine(std::uint32_t verticalLine,
                          std::uint32_t up, std::uint32_t down,
                          bool hole);

    void findLongestPath();

    std::pair<int, int> getStartPosition();

    bool isValidCoord(std::pair<int, int> position);

    void printMaze(std::ostream & file);



    std::vector<std::vector<CCell>> mMaze;

    std::vector<std::pair<
        std::uint32_t, std::uint32_t>> mLongestPath;
};

