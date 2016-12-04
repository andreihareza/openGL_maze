#include <algorithm>

#include "CGameHandler.hpp"

CGameHandler::CGameHandler(COpenGLHandler & openGLHandler)
: mOpenGLHandler(openGLHandler)
, mKeyPressed{0}
, mLastMoveTime{0ms}
, mPositiveItem{CDropItem::Type::Positive, *this}
, mNegativeItem{CDropItem::Type::Negative, *this}
{
    if ((NUtility::lines > 1) && (NUtility::columns > 1))
    {
        mMaze.generateMaze(0, NUtility::lines, 0, NUtility::columns);

        mPath = mMaze.getLongestPath();
        mPlayer = {mPath[0].first, mPath[0].second};

        drawMaze();
        mOpenGLHandler.reDraw();
    }
}

void CGameHandler::drawMaze()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;

    requestDrawPath();
    requestDrawLines();
}

void CGameHandler::requestDrawLines()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;

    std::vector<std::tuple<std::uint32_t, std::uint32_t,
                           std::uint32_t, std::uint32_t>> walls = mMaze.getWalls();

    for (auto & wall : walls)
    {
        glm::vec4 firstPoint {std::get<0>(wall), std::get<1>(wall), 0.0, 1.0};
        glm::vec4 secondPoint {std::get<2>(wall), std::get<3>(wall), 0.0, 1.0};

        glm::vec4 color {1.0, 0.55, 0.0, 1.0};

        mOpenGLHandler.addLine(firstPoint, secondPoint, color, 2.0, false);
    }
}

void CGameHandler::requestDrawPath()
{
    std::cout << "CGameHandler::" << __func__ << "(): mPath.size()=" << mPath.size() << std::endl;

    /* ********************************* All cells on path ******************************** */
    if (mPath.size() > 1)
    {
        for (std::uint32_t i=0; i<mPath.size(); ++i)
        {
            NUtility::Position upCell = {mPath[i].first-1, mPath[i].second};
            NUtility::Position downCell = {mPath[i].first+1, mPath[i].second};
            NUtility::Position leftCell = {mPath[i].first, mPath[i].second-1};
            NUtility::Position rightCell = {mPath[i].first, mPath[i].second+1};

            float upAdd = 0.0;
            float downAdd = 0.0;
            float leftAdd = 0.0;
            float rightAdd = 0.0;
            if (i == 0)
            {
                upAdd = (mPath[i+1] == upCell)? 0.0 : 0.05;
                downAdd = (mPath[i+1] == downCell)? 0.0 : -0.05;
                leftAdd = (mPath[i+1] == leftCell)? 0.0 : 0.05;
                rightAdd = (mPath[i+1] == rightCell)? 0.0 : -0.05;
            }
            else if (i == mPath.size()-1)
            {
                upAdd = (mPath[i-1] == upCell)? 0.0 : 0.05;
                downAdd = (mPath[i-1] == downCell)? 0.0 : -0.05;
                leftAdd = (mPath[i-1] == leftCell)? 0.0 : 0.05;
                rightAdd = (mPath[i-1] == rightCell)? 0.0 : -0.05;
            }
            else
            {
                upAdd = ((mPath[i-1] == upCell) || (mPath[i+1] == upCell))? 0.0 : 0.05;
                downAdd = ((mPath[i-1] == downCell) || (mPath[i+1] == downCell))? 0.0 : -0.05;
                leftAdd = ((mPath[i-1] == leftCell) || (mPath[i+1] == leftCell))? 0.0 : 0.05;
                rightAdd = ((mPath[i-1] == rightCell) || (mPath[i+1] == rightCell))? 0.0 : -0.05;
            }

            glm::vec4 firstPoint  {mPath[i].first+upAdd, mPath[i].second+leftAdd, 0.0, 1.0};
            glm::vec4 secondPoint {mPath[i].first+1+downAdd, mPath[i].second+leftAdd, 0.0, 1.0};
            glm::vec4 thirdPoint  {mPath[i].first+upAdd, mPath[i].second+1+rightAdd, 0.0, 1.0};
            glm::vec4 fourthPoint {mPath[i].first+1+downAdd, mPath[i].second+1+rightAdd, 0.0, 1.0};

            glm::vec4 color {0.05, 0.05, 0.05, 1.0};

            mOpenGLHandler.addFilledRectangle(
                    firstPoint, secondPoint, thirdPoint, fourthPoint, color, false);
        }
    }

    /* ********************************* All cells on path ******************************** */

    drawPlayer();
    drawEnd();
}

void CGameHandler::drawPlayer()
{
    /* *********************************** Current cell ********************************** */
    float upAdd = 0.05;
    float downAdd = -0.05;
    float leftAdd = 0.05;
    float rightAdd = -0.05;

    glm::vec4 firstPoint  {mPlayer.first+upAdd, mPlayer.second+leftAdd, 0.0, 1.0};
    glm::vec4 secondPoint {mPlayer.first+1+downAdd, mPlayer.second+leftAdd, 0.0, 1.0};
    glm::vec4 thirdPoint  {mPlayer.first+upAdd, mPlayer.second+1+rightAdd, 0.0, 1.0};
    glm::vec4 fourthPoint {mPlayer.first+1+downAdd, mPlayer.second+1+rightAdd, 0.0, 1.0};

    glm::vec4 color {0.0, 1.0, 0.0, 1.0};

    mPlayerId = mOpenGLHandler.addFilledRectangle(
            firstPoint, secondPoint, thirdPoint, fourthPoint, color);

    /* *********************************** Current cell ********************************** */
}

void CGameHandler::drawEnd()
{
    /* ************************************ Last cell ************************************ */
    const std::uint32_t lst = mPath.size()-1;

    NUtility::Position upCell = {mPath[lst].first-1, mPath[lst].second};
    NUtility::Position downCell = {mPath[lst].first+1, mPath[lst].second};
    NUtility::Position leftCell = {mPath[lst].first, mPath[lst].second-1};
    NUtility::Position rightCell = {mPath[lst].first, mPath[lst].second+1};
    float upAdd = (mPath[lst-1] == upCell)? 0.0 : 0.05;
    float downAdd = (mPath[lst-1] == downCell)? 0.0 : -0.05;
    float leftAdd = (mPath[lst-1] == leftCell)? 0.0 : 0.05;
    float rightAdd = (mPath[lst-1] == rightCell)? 0.0 : -0.05;

    glm::vec4 firstPoint  {mPath[lst].first+upAdd, mPath[lst].second+leftAdd, 0.0, 1.0};
    glm::vec4 secondPoint {mPath[lst].first+1+downAdd, mPath[lst].second+leftAdd, 0.0, 1.0};
    glm::vec4 thirdPoint  {mPath[lst].first+upAdd, mPath[lst].second+1+rightAdd, 0.0, 1.0};
    glm::vec4 fourthPoint {mPath[lst].first+1+downAdd, mPath[lst].second+1+rightAdd, 0.0, 1.0};

    glm::vec4 color {1.0, 0.0, 0.0, 1.0};

    mOpenGLHandler.addFilledRectangle(
            firstPoint, secondPoint, thirdPoint, fourthPoint, color);

    /* ************************************ Last cell ************************************ */
}

void CGameHandler::upArrowPressed()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed |= (1 << toInt(Direction::Up));
}

void CGameHandler::downArrowPressed()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed |= (1 << toInt(Direction::Down));
}

void CGameHandler::leftArrowPressed()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed |= (1 << toInt(Direction::Left));
}

void CGameHandler::rightArrowPressed()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed |= (1 << toInt(Direction::Right));
}

void CGameHandler::upArrowReleased()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed &= ~(1 << toInt(Direction::Up));
}

void CGameHandler::downArrowReleased()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed &= ~(1 << toInt(Direction::Down));
}

void CGameHandler::leftArrowReleased()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed &= ~(1 << toInt(Direction::Left));
}

void CGameHandler::rightArrowReleased()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mKeyPressed &= ~(1 << toInt(Direction::Right));
}

void CGameHandler::newFrame()
{
    // std::cout << "CGameHandler::" << __func__ << "()" << std::endl;

    tryMove();
    trySpawnPositive();
    trySpawnNegative();
    tryEndSpeedUp();
}

void CGameHandler::tryMove()
{

    std::chrono::milliseconds currentTime = NUtility::getCurrentTime();

    if ((currentTime - mLastMoveTime > mMoveDuration) &&  (mOpenGLHandler.isMoving() == false))
    {
        // std::cout << "CGameHandler::" << __func__ << "(): mKeyPressed = " << mKeyPressed << std::endl;
        // std::cout << "CGameHandler::" << __func__ << "(): mPlayer = (" << mPlayer.first << " " << mPlayer.second << ")" << std::endl;

        if (mKeyPressed == (1 << toInt(Direction::Up)))
        {
            if (canMovePlayer(mPlayer, Direction::Up) == true)
            {
                movePlayer(mPlayer, Direction::Up);
                mLastMoveTime = currentTime;
            }
        }

        if (mKeyPressed == (1 << toInt(Direction::Down)))
        {
            if (canMovePlayer(mPlayer, Direction::Down) == true)
            {
                movePlayer(mPlayer, Direction::Down);
                mLastMoveTime = currentTime;
            }
        }

        if (mKeyPressed == (1 << toInt(Direction::Left)))
        {
            if (canMovePlayer(mPlayer, Direction::Left) == true)
            {
                movePlayer(mPlayer, Direction::Left);
                mLastMoveTime = currentTime;
            }
        }

        if (mKeyPressed == (1 << toInt(Direction::Right)))
        {
            if (canMovePlayer(mPlayer, Direction::Right) == true)
            {
                movePlayer(mPlayer, Direction::Right);
                mLastMoveTime = currentTime;
            }
        }
    }
}

bool CGameHandler::canMovePlayer(CPlayer player, Direction direction)
{
    return mMaze.canMove(
            static_cast<std::pair<uint32_t, uint32_t>>(player), direction);
}

void CGameHandler::movePlayer(CPlayer & player, Direction direction)
{
    std::cout << "CGameHandler::" << __func__ << "(): direction = " << toString(direction) << std::endl;

    player.move(direction);

    if (&player == &mPlayer)
    {
        NUtility::Position playerPosition =
            static_cast<NUtility::Position>(player);

        if (mPositiveItem.exists() && (playerPosition == mPositiveItem.getPosition()))
        {
            mPositiveItem.pick();
            mOpenGLHandler.removeFilledRectangle(mPositiveItemId);
            mPositiveItemId = 0u;
        }

        if (mNegativeItem.exists() && (playerPosition == mNegativeItem.getPosition()))
        {
            mNegativeItem.pick();
            mOpenGLHandler.removeFilledRectangle(mNegativeItemId);
            mNegativeItemId = 0u;
        }
    }

    requestMovePlayer();
}

void CGameHandler::requestMovePlayer()
{
    float upAdd = 0.05;
    float downAdd = -0.05;
    float leftAdd = 0.05;
    float rightAdd = -0.05;

    glm::vec4 firstPoint  {mPlayer.first+upAdd, mPlayer.second+leftAdd, 0.0, 1.0};
    glm::vec4 secondPoint {mPlayer.first+1+downAdd, mPlayer.second+leftAdd, 0.0, 1.0};
    glm::vec4 thirdPoint  {mPlayer.first+upAdd, mPlayer.second+1+rightAdd, 0.0, 1.0};
    glm::vec4 fourthPoint {mPlayer.first+1+downAdd, mPlayer.second+1+rightAdd, 0.0, 1.0};

    mOpenGLHandler.moveFilledRectangle(mPlayerId,
            firstPoint, secondPoint, thirdPoint, fourthPoint, mMoveDuration);
}

void CGameHandler::trySpawnPositive()
{
    if (mPositiveItem.canAppear() == true)
    {
        mPositiveItem.appear(
                static_cast<std::pair<uint32_t, uint32_t>>(mPlayer), mPath,
                0, NUtility::lines-1, 0, NUtility::columns-1);

        NUtility::Position itemPosition = mPositiveItem.getPosition();

        glm::vec4 firstPoint {itemPosition.first + 0.25f, itemPosition.second + 0.25f, 0.0f, 1.0f};
        glm::vec4 secondPoint {itemPosition.first + 0.75f, itemPosition.second + 0.25f, 0.0f, 1.0f};
        glm::vec4 thirdPoint {itemPosition.first + 0.25f, itemPosition.second + 0.75f, 0.0f, 1.0f};
        glm::vec4 fourthPoint {itemPosition.first + 0.75f, itemPosition.second + 0.75f, 0.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.5f, 0.0f, 0.0f};

        mPositiveItemId = mOpenGLHandler.addFilledRectangle(
                firstPoint, secondPoint, thirdPoint, fourthPoint, color);
    }
}

void CGameHandler::trySpawnNegative()
{
    if (mNegativeItem.canAppear() == true)
    {
        mNegativeItem.appear(
                static_cast<std::pair<uint32_t, uint32_t>>(mPlayer), mPath,
                0, NUtility::lines-1, 0, NUtility::columns-1);

        NUtility::Position itemPosition = mNegativeItem.getPosition();

        glm::vec4 firstPoint {itemPosition.first + 0.25f, itemPosition.second + 0.25f, 0.0f, 1.0f};
        glm::vec4 secondPoint {itemPosition.first + 0.75f, itemPosition.second + 0.25f, 0.0f, 1.0f};
        glm::vec4 thirdPoint {itemPosition.first + 0.25f, itemPosition.second + 0.75f, 0.0f, 1.0f};
        glm::vec4 fourthPoint {itemPosition.first + 0.75f, itemPosition.second + 0.75f, 0.0f, 1.0f};
        glm::vec4 color = {0.53f, 0.12f, 0.47f, 0.0f};

        mNegativeItemId = mOpenGLHandler.addFilledRectangle(
                firstPoint, secondPoint, thirdPoint, fourthPoint, color);
    }
}

void CGameHandler::tryEndSpeedUp()
{
    if (mSpeedUpActive &&
       (NUtility::getCurrentTime() - speedUpEffectDuration > mSpeedUpTime))
    {
        std::cout << "CGameHandler::" << __func__ << "(): ended" << std::endl;
        mMoveDuration = defaultMoveDuration;
        mSpeedUpActive = false;
    }
}

void CGameHandler::rotateEffectPicked()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
}

void CGameHandler::speedUpEffectPicked()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;
    mMoveDuration = speedUpMoveDuration;

    mSpeedUpActive = true;
    mSpeedUpTime = NUtility::getCurrentTime();
}

