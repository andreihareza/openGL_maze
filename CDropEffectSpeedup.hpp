#pragma once

#include "CDropEffectPositive.hpp"

class CDropEffectSpeedup : public CDropEffectPositive
{
public:
    CDropEffectSpeedup(IDropEffectListener & listener);
    virtual void picked() override;

private:
};

