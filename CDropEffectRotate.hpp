#pragma once

#include "CDropEffectNegative.hpp"

class CDropEffectRotate: public CDropEffectNegative
{
public:
    CDropEffectRotate(IDropEffectListener & listener);
    virtual void picked() override;

private:
};

