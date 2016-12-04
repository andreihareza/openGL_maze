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

