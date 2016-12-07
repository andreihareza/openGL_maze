#pragma once
#include <vector>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <deque>

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
            glm::vec4 color, float lineWidth,
            bool reDraw = true);

    std::uint32_t addFilledRectangle(glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint, glm::vec4 color,
            bool reDraw = true);

    void moveFilledRectangle(std::uint32_t figureId,
            glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint,
            std::chrono::milliseconds time = 0ms);

    void removeFilledRectangle(std::uint32_t figureId,
            bool reDraw = true);

    void clearDrawData();
    void reDraw();

    void spin(std::uint32_t figureId);
    void stopSpin(std::uint32_t figureId);

    void rotateScreen(std::chrono::milliseconds time, float angle);
    void stopRotate(std::chrono::milliseconds time);

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

    glm::mat4 mCurrentTransformMatrix;
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

    std::unordered_map<std::uint32_t, std::pair<std::uint32_t, std::uint32_t>> mExternalId;
    std::uint32_t mNextAvailableId = 0u;
    std::deque<std::uint32_t> mAvailableIds;

    /* ************************** Spin ************************* */
    glm::mat4 getSpinMatrix(glm::vec4 center, float angle);
    void updateSpinAngle(std::uint32_t start);

    std::unordered_map<std::uint32_t, bool> mIsSpinning;
    std::unordered_map<std::uint32_t, float> mSpinningAngle;

    /* ************************** Spin ************************* */

    /* ************************* Rotate ************************ */
    void smoothRotateScreen(
            std::chrono::milliseconds timeLeft, float remaningAngle);
    void smoothStopRotate(
            std::chrono::milliseconds timeLeft, float remaningAngle);

    glm::mat4 getRotateMatrix(glm::vec3 center, float angle);

    bool mIsRotating;
    float mRotationAngle;
    glm::vec3 mCenter;
    /* ************************* Rotate ************************ */
};

