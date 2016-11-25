#pragma once

#include "CMaze.hpp"
#include "COpenGLHandler.hpp"

class CGameHandler
{
public:
    CGameHandler(COpenGLHandler & openGLHandler);

private:
    void drawMaze();
    void requestDrawPath();
    void requestDrawLines();

    COpenGLHandler & mOpenGLHandler;
    CMaze mMaze;
};

