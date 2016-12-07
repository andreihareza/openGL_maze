#pragma once

#include <bitset>

#include "CMaze.hpp"
#include "COpenGLHandler.hpp"
#include "CPlayer.hpp"
#include "IOpenGLListener.hpp"
#include "IDropEffectListener.hpp"
#include "CDropItem.hpp"

class CGameHandler : public IOpenGLListener
                   , public IDropEffectListener
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

    virtual void speedUpEffectPicked() override;
    virtual void rotateEffectPicked() override;

private:
    void drawMaze();

    void requestDrawPath();
    void drawPlayer();
    void drawEnd();

    void requestDrawLines();

    bool canMovePlayer(CPlayer player, Direction direction);
    void movePlayer(CPlayer & player, Direction direction);
    void requestMovePlayer();

    void tryMove();
    void trySpawnPositive();
    void trySpawnNegative();
    void tryEndSpeedUp();
    void tryEndRotate();


    COpenGLHandler & mOpenGLHandler;

    CMaze mMaze;
    std::vector<NUtility::Position> mPath;

    CPlayer mPlayer;
    std::uint32_t mPlayerId;

    std::uint32_t mKeyPressed;
    std::chrono::milliseconds mLastMoveTime;

    CDropItem mPositiveItem;
    CDropItem mNegativeItem;

    std::uint32_t mPositiveItemId;
    std::uint32_t mNegativeItemId;

    static const constexpr std::chrono::milliseconds
        defaultMoveDuration = 200ms;
    static const constexpr std::chrono::milliseconds
        speedUpMoveDuration = 140ms;

    std::chrono::milliseconds mMoveDuration = defaultMoveDuration;

    static const constexpr std::chrono::milliseconds
        speedUpEffectDuration = 8000ms;

    static const constexpr std::chrono::milliseconds
        rotationEffectDuration = 8000ms;


    bool mSpeedUpActive = false;
    std::chrono::milliseconds mSpeedUpTime = 0ms;

    bool mRotationActive = false;
    std::chrono::milliseconds mRotationTime = 0ms;
};

