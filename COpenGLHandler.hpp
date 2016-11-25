#pragma once
#include <vector>
#include <functional>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class COpenGLHandler
{
public:
    COpenGLHandler(int argc, char ** argv);
    ~COpenGLHandler();

    void init();

    void addLine(glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 color, float lineWidth);

    void addFilledRectangle(glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint, glm::vec4 color);

    void clearDrawData();
    void reDraw();

private:

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
};

