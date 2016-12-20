#include "CDropEffectZoom.hpp"

CDropEffectZoom::CDropEffectZoom(IDropEffectListener & listener)
: CDropEffectNegative(listener)
{
}

void CDropEffectZoom::picked()
{
    std::cout << "CDropEffectZoom::" << __func__ << "()" << std::endl;
    mListener.zoomEffectPicked();
}

glm::vec4 CDropEffectZoom::getColor()
{
    return {0.0f, 0.0f, 0.61f, 0.0f};
}

