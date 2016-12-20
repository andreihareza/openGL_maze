#pragma once

class IDropEffectListener
{
public:
    virtual void speedUpEffectPicked() = 0;
    virtual void rotateEffectPicked() = 0;
    virtual void zoomEffectPicked() = 0;
};

