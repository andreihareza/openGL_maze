#include "CDropEffectSpeedup.hpp"

CDropEffectSpeedup::CDropEffectSpeedup(IDropEffectListener & listener)
: CDropEffectPositive(listener)
{
}

void CDropEffectSpeedup::picked()
{
    std::cout << "CDropEffectSpeedup::" << __func__ << "()" << std::endl;
    mListener.speedUpEffectPicked();
}

glm::vec4 CDropEffectSpeedup::getColor()
{
    return {1.0f, 0.5f, 0.0f, 0.0f};
}

