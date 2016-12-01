#pragma once

class IOpenGLListener
{
public:
    virtual void upArrowPressed() = 0;
    virtual void downArrowPressed() = 0;
    virtual void leftArrowPressed() = 0;
    virtual void rightArrowPressed() = 0;

    virtual void upArrowReleased() = 0;
    virtual void downArrowReleased() = 0;
    virtual void leftArrowReleased() = 0;
    virtual void rightArrowReleased() = 0;

    virtual void newFrame() = 0;
};

