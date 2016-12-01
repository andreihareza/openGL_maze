#pragma once

#include <bitset>

#include "CMaze.hpp"
#include "COpenGLHandler.hpp"
#include "CPlayer.hpp"
#include "IOpenGLListener.hpp"

class CGameHandler : public IOpenGLListener
{
public:
    CGameHandler(COpenGLHandler & openGLHandler);

    virtual void upArrowPressed() override;
    virtual void downArrowPressed() override;
    virtual void leftArrowPressed() override;
    virtual void rightArrowPressed() override;

    virtual void upArrowReleased() override;
    virtual void downArrowReleased() override;
    virtual void leftArrowReleased() override;
    virtual void rightArrowReleased() override;

    virtual void newFrame() override;

private:
    void drawMaze();

    void requestDrawPath();
    void drawPlayer();
    void drawEnd();

    void requestDrawLines();

    bool canMovePlayer(CPlayer player, Direction direction);
    void movePlayer(CPlayer & player, Direction direction);



    COpenGLHandler & mOpenGLHandler;

    CMaze mMaze;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> mPath;

    CPlayer mPlayer;
    std::uint32_t mPlayerId;

    std::uint32_t mKeyPressed;
    std::chrono::milliseconds mLastMoveTime;
};

