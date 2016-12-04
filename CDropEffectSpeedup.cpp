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

