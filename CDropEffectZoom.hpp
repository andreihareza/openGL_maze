#pragma once

#include "CDropEffectNegative.hpp"

class CDropEffectZoom : public CDropEffectNegative
{
public:
    CDropEffectZoom(IDropEffectListener & listener);
    virtual void picked() override;
    virtual glm::vec4 getColor() override;

private:
};

