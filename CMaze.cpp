#include <queue>

#include "CMaze.hpp"
#include "utility.hpp"

CMaze::CMaze()
: mMaze(NUtility::lines, std::vector<CCell>(NUtility::columns))

{
}

void CMaze::generateMaze(std::uint32_t up, std::uint32_t down,
                          std::uint32_t left, std::uint32_t right)
{
    std::cout << "CMaze::" << __func__ << "():" << " up=" << up << " down=" << down << " left=" << left << " right=" << right << std::endl;

    if ((down > up) && (right > left))
    {
        createBorder();
        generateMazeRec(up, down, left, right);
        printMaze(std::cout);

        findLongestPath();
    }
}

void CMaze::findLongestPath()
{
    std::cout << "CMaze::" << __func__ << "()" << std::endl;
    std::pair<int, int> startPos = getStartPosition();

    std::vector<int> dx = {-1, 1,  0, 0};
    std::vector<int> dy = { 0, 0, -1, 1};

    std::queue<std::pair<int, int>> bfsQueue;

    std::vector<std::vector<bool>> alreadyVisited(
            mMaze.size(), std::vector<bool>(mMaze[0].size(), false));

    std::vector<std::vector<std::pair<int, int>>> from(
            mMaze.size(), std::vector<std::pair<int, int>>(mMaze[0].size(), {0, 0}));

    bfsQueue.push(startPos);
    alreadyVisited[startPos.first][startPos.second] = true;
    from[startPos.first][startPos.second] = startPos;
    std::pair<int, int> last = startPos;

    while (bfsQueue.empty() == false)
    {
        std::pair<int, int> current = bfsQueue.front();
        bfsQueue.pop();

        for (std::uint32_t currentDirection=0; currentDirection<4; ++currentDirection)
        {
            if (canMove(current, static_cast<Direction>(currentDirection)))
            {
                std::pair<int, int> next =
                     {current.first+dx[currentDirection], current.second+dy[currentDirection]};

                if (isValidCoord(next) && alreadyVisited[next.first][next.second] == false)
                {
                    alreadyVisited[next.first][next.second] = true;
                    from[next.first][next.second] = current;
                    bfsQueue.push(next);
                }
            }
        }
        last = current;
    }
    mLongestPath.clear();
    mLongestPath.push_back(last);

    while (from[last.first][last.second] != last)
    {
        last = from[last.first][last.second];
        mLongestPath.push_back(last);
    }
}

std::pair<int, int> CMaze::getStartPosition()
{
    std::cout << "CMaze::" << __func__ << "()" << std::endl;
    std::vector<int> dx = {-1, 1,  0, 0};
    std::vector<int> dy = { 0, 0, -1, 1};

    std::queue<std::pair<int, int>> bfsQueue;
    std::vector<std::vector<bool>> alreadyVisited(mMaze.size(), std::vector<bool>(mMaze[0].size(), false));

    bfsQueue.push({0, 0});
    alreadyVisited[0][0] = true;

    std::pair<int, int> last = {0, 0};

    while (bfsQueue.empty() == false)
    {
        std::pair<int, int> current = bfsQueue.front();
        bfsQueue.pop();

        for (std::uint32_t currentDirection=0; currentDirection<4; ++currentDirection)
        {
            if (canMove(current, static_cast<Direction>(currentDirection)))
            {
                std::pair<int, int> next =
                     {current.first+dx[currentDirection], current.second+dy[currentDirection]};

                if (isValidCoord(next) && alreadyVisited[next.first][next.second] == false)
                {
                    alreadyVisited[next.first][next.second] = true;
                    bfsQueue.push(next);
                }
            }
        }

        last = current;
    }

    return last;
}

std::vector<std::pair<std::uint32_t, std::uint32_t>> CMaze::getLongestPath()
{
    std::cout << "CMaze::" << __func__ << "()" << std::endl;
    return mLongestPath;
}

bool CMaze::canMove(std::pair<int, int> from, Direction dir)
{
    // std::cout << "CMaze::" << __func__ << "(): from = (" << from.first << " " << from.second << "), dir = " << static_cast<std::uint32_t>(dir) << std::endl;

    return mMaze[from.first][from.second].hasBorder(dir) == false;
}

bool CMaze::isValidCoord(std::pair<int, int> position)
{
    return position.first  >= 0 && position.first  < static_cast<int>(mMaze.size())
        && position.second >= 0 && position.second < static_cast<int>(mMaze[0].size());
}

void CMaze::generateMazeRec(std::uint32_t up, std::uint32_t down,
                          std::uint32_t left, std::uint32_t right)
{
    // std::cout << "CMaze::" << __func__ << "():" << " up=" << up << " down=" << down << " left=" << left << " right=" << right << std::endl;

    if ((down - up == 1) || (right - left == 1))
    {
        return;
    }

    std::uint32_t horizontalLine = NUtility::generateRandomBetween(up+1, down-1);
    std::uint32_t verticalLine = NUtility::generateRandomBetween(left+1, right-1);
    // std::cout << "CMaze::" << __func__ << "():" << " horizontalLine=" << horizontalLine << std::endl;
    // std::cout << "CMaze::" << __func__ << "():" << " verticalLine=" << verticalLine << std::endl;

    std::vector<bool> remaning = {true, true, true, true};
    remaning[NUtility::generateRandomBetween(0, 3)] = false;

    fillHorizontalLine(horizontalLine, left, verticalLine, remaning[toInt(Direction::Left)]);
    fillHorizontalLine(horizontalLine, verticalLine, right, remaning[toInt(Direction::Right)]);

    fillVerticalLine(verticalLine, up, horizontalLine, remaning[toInt(Direction::Up)]);
    fillVerticalLine(verticalLine, horizontalLine, down, remaning[toInt(Direction::Down)]);

    generateMazeRec(up, horizontalLine, left, verticalLine);  /* Top left */
    generateMazeRec(up, horizontalLine, verticalLine, right); /* Top right */
    generateMazeRec(horizontalLine, down, left, verticalLine);  /* Bottom left */
    generateMazeRec(horizontalLine, down, verticalLine, right); /* Bottom right */
}

void CMaze::createBorder()
{
    std::cout << "CMaze::" << __func__ << "()" << std::endl;

    for (std::uint32_t i=0; i<NUtility::lines; ++i)
    {
        mMaze[i][0].addBorder(Direction::Left);
        mMaze[i][NUtility::columns-1].addBorder(Direction::Right);
    }

    for (std::uint32_t i=0; i<NUtility::columns; ++i)
    {
        mMaze[0][i].addBorder(Direction::Up);
        mMaze[NUtility::lines-1][i].addBorder(Direction::Down);
    }
}

void CMaze::fillHorizontalLine(std::uint32_t horizontalLine,
                               std::uint32_t left, std::uint32_t right,
                               bool hole)
{
    // std::cout << "CMaze::" << __func__ << "():" << " horizontalLine=" << horizontalLine << " left=" << left << " right=" << right << std::boolalpha << " hole=" << hole <<std::endl;

    std::uint32_t holeColumn = -1;

    if (hole == true)
    {
        holeColumn = NUtility::generateRandomBetween(left, right-1);
    }

    // std::cout << "CMaze::" << __func__ << "():" << " holeColumn=" << holeColumn << std::endl;

    for (std::uint32_t i=left; i<right; ++i)
    {
        if (i != holeColumn)
        {
            mMaze[horizontalLine-1][i].addBorder(Direction::Down);
            mMaze[horizontalLine][i].addBorder(Direction::Up);
        }
    }
}

void CMaze::fillVerticalLine(std::uint32_t verticalLine,
                               std::uint32_t up, std::uint32_t down,
                               bool hole)
{
    // std::cout << "CMaze::" << __func__ << "():" << " verticalLine=" << verticalLine << " up=" << up << " down=" << down << std::boolalpha << " hole=" << hole <<std::endl;

    std::uint32_t holeLine = -1;

    if (hole == true)
    {
        holeLine = NUtility::generateRandomBetween(up, down-1);
    }

    // std::cout << "CMaze::" << __func__ << "():" << " holeLine=" << holeLine << std::endl;

    for (std::uint32_t i=up; i<down; ++i)
    {
        if (i != holeLine)
        {
            mMaze[i][verticalLine-1].addBorder(Direction::Right);
            mMaze[i][verticalLine].addBorder(Direction::Left);
        }
    }
}
void CMaze::printMaze(std::ostream & file)
{
    for (auto & line : mMaze)
    {
        for (auto & cell : line)
        {
            cell.printBitmap(file);
        }

        std::cout << std::endl;
    }
}

std::vector<std::tuple<std::uint32_t, std::uint32_t,
                       std::uint32_t, std::uint32_t>> CMaze::getWalls()
{
    std::vector<std::tuple<std::uint32_t, std::uint32_t,
                           std::uint32_t, std::uint32_t>> walls;


    for (std::uint32_t line=0; line<mMaze.size(); ++line)
    {
        for(std::uint32_t column=0; column<mMaze[line].size(); ++column)
        {
            if (mMaze[line][column].hasBorder(Direction::Up))
            {
                walls.push_back(std::make_tuple(line, column, line, column+1));
            }
            if (mMaze[line][column].hasBorder(Direction::Left))
            {
                walls.push_back(std::make_tuple(line, column, line+1, column));
            }
        }
    }

    if (mMaze.size() > 0)
    {
        for(std::uint32_t column=0; column<mMaze[mMaze.size()-1].size(); ++column)
        {
            if (mMaze[mMaze.size()-1][column].hasBorder(Direction::Down))
            {
                walls.push_back(std::make_tuple(
                            static_cast<std::uint32_t>(mMaze.size()), column,
                            static_cast<std::uint32_t>(mMaze.size()), column+1));
            }
        }
    }

    for (std::uint32_t line=0; line<mMaze.size(); ++line)
    {
        if (mMaze[line].size() > 0)
        {
            if (mMaze[line][mMaze[line].size()-1].hasBorder(Direction::Right))
            {
                walls.push_back(std::make_tuple(
                            line, static_cast<std::uint32_t>(mMaze[line].size()),
                            line+1, static_cast<std::uint32_t>(mMaze[line].size())));
            }
        }
    }


    return walls;
}

