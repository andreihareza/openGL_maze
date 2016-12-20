#include "CDropEffectRotate.hpp"

CDropEffectRotate::CDropEffectRotate(IDropEffectListener & listener)
: CDropEffectNegative(listener)
{
}

void CDropEffectRotate::picked()
{
    std::cout << "CDropEffectRotate::" << __func__ << "()" << std::endl;
    mListener.rotateEffectPicked();
}

glm::vec4 CDropEffectRotate::getColor()
{
    return {0.53f, 0.12f, 0.47f, 0.0f};
}

