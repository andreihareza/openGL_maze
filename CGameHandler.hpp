#pragma once

#include "CMaze.hpp"
#include "COpenGLHandler.hpp"

class CGameHandler
{
public:
    CGameHandler(COpenGLHandler & openGLHandler);

private:
    void drawMaze();

    COpenGLHandler & mOpenGLHandler;
    CMaze mMaze;
};

