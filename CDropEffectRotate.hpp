#pragma once

#include "CDropEffectNegative.hpp"

class CDropEffectRotate: public CDropEffectNegative
{
public:
    CDropEffectRotate(IDropEffectListener & listener);
    virtual void picked() override;
    virtual glm::vec4 getColor() override;

private:
};

