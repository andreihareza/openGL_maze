#include "CRoot.hpp"

CRoot::CRoot(int argc, char ** argv)
: mOpenGLHandler(argc, argv, mGameHandler)
, mGameHandler(mOpenGLHandler)
{
    std::cout << "CRoot::" << __func__ << "()" << std::endl;
    mOpenGLHandler.init();
}
