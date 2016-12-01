#pragma once
#include <vector>
#include <functional>
#include <chrono>
#include <unordered_map>

#include "IOpenGLListener.hpp"
#include "utility.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std::chrono_literals;

class COpenGLHandler
{
public:
    COpenGLHandler(int argc, char ** argv, IOpenGLListener & listener);
    ~COpenGLHandler();

    void init();

    std::uint32_t addLine(glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 color, float lineWidth);

    std::uint32_t addFilledRectangle(glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint, glm::vec4 color);

    void moveFilledRectangle(std::uint32_t startPos,
            glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint,
            std::chrono::milliseconds time = 0ms);

    void clearDrawData();
    void reDraw();

    bool isMoving();

private:
    /* ********************** Smooth move ********************** */
    void smoothMoveFilledRectangle(std::uint32_t startPos,
            glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint,
            std::chrono::milliseconds time);

    void movePoint(glm::vec4 & from, glm::vec4 to,
            std::chrono::milliseconds time);

    std::unordered_map<std::uint32_t, std::function<void()>> mTimerFunctions;
    bool mIsMoving;
    /* ********************** Smooth move ********************** */

    /* ************************ Shaders ************************ */
    void createShaders();
    std::uint32_t loadShaders(const char * vertex_file_path,
        const char * fragment_file_path);

    void useShader(std::uint32_t shaderId, bool sendData = true);

    void destroyShaders();

    std::uint32_t mDefaultShaderId;

    glm::mat4 mDefaultObserverMatrix;
    glm::mat4 mDefaultTransformMatrix;
    glm::mat4 mDefaultProjectionMatrix;
    std::uint32_t mObserverMatrixLocation;
    std::uint32_t mTransformMatrixLocation;
    std::uint32_t mProjectionMatrixLocation;
    /* ************************ Shaders ************************ */

    /* *********************** Callbacks *********************** */
    void setOpenGLCallbacks();

    static void renderFunctionCallback();
    void renderFunction();

    static void specialKeyFunctionCallback(int key, int x, int y);
    void specialKeyFunction(int key);

    static void specialKeyUpFunctionCallback(int key, int x, int y);
    void specialKeyUpFunction(int key);

    static void idleFunctionCallback();
    void idleFunction();

    static void timerFuncCallback(int value);
    void timerFunc(std::uint32_t functionId);
    /* *********************** Callbacks *********************** */

    /* ************************** VBO ************************** */
    void destroyVBO();

    std::vector<glm::vec4> mVboData;
    std::vector<glm::vec4> mColorData;
    std::vector<std::function<void()>> mDrawFunctions;
    float mCurrentLineWidth;

    std::uint32_t mVboId;
    std::uint32_t mColorBufferId;
    /* ************************** VBO ************************** */


    static COpenGLHandler * stCurrentObject;
    IOpenGLListener & mListener;

    static const constexpr std::chrono::milliseconds timeInFrame = 17ms;
};

