#pragma once
#include "CGameHandler.hpp"
#include "COpenGLHandler.hpp"

class CRoot
{
public:
    CRoot(int argc, char ** argv);

private:
    COpenGLHandler mOpenGLHandler;

    CGameHandler mGameHandler;
};

