#include <iostream>
#include <fstream>
#include <algorithm>

#include "COpenGLHandler.hpp"
#include "utility.hpp"

COpenGLHandler * COpenGLHandler::stCurrentObject {nullptr};

COpenGLHandler::COpenGLHandler(int argc, char ** argv, IOpenGLListener & listener)
: mIsMoving{false}
, mListener(listener)
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;
    /* Initialize open GL */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    /* Initialize window */
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(NUtility::width, NUtility::height);
    glutCreateWindow(NUtility::gameName.c_str());

    /* Initialize glew */
    glewInit();

    /* Background color black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();

    glutIgnoreKeyRepeat(GLUT_KEY_REPEAT_ON);

    createShaders();
    setOpenGLCallbacks();

    stCurrentObject = this;
}

COpenGLHandler::~COpenGLHandler()
{
    destroyVBO();
    destroyShaders();
}

void COpenGLHandler::init()
{
    glutMainLoop();
}

void COpenGLHandler::createShaders()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;
    glm::vec3 obsPos(0.0f, 0.0f, 1.0f);
    glm::vec3 refPos(0.0f, 0.0f, 0.0f);
    glm::vec3 vert  (0.0f, 1.0f, 0.0f);
    mDefaultObserverMatrix = glm::lookAt(obsPos, refPos, vert);

    mDefaultProjectionMatrix = glm::ortho(-1.0f, static_cast<float>(NUtility::lines+1),
                                           static_cast<float>(NUtility::columns+1), -1.0f);

    mDefaultTransformMatrix = glm::mat4();

    mCenter = glm::vec3
    {
        static_cast<float>(NUtility::columns)/2,
        static_cast<float>(NUtility::lines)/2,
        0.0f
    };

    mDefaultShaderId = loadShaders("shader.vert", "shader.frag");

    useShader(mDefaultShaderId);
}

std::uint32_t COpenGLHandler::loadShaders(const char * vertex_file_path,
        const char * fragment_file_path)
{
    // Creeaza shadere
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Citeste din fisier shader-ul de varf
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Citeste din fisier shader-ul de fragment
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compileaza shader-ul de varf
    printf("Compilare shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Verifica
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compileaza shader-ul de fragemnt
    printf("Compilare shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);

    // Verifica
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

    // Leaga programul
    fprintf(stdout, "Legare program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Verifica
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

	glUseProgram(0);

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(FragmentShaderID);
    glDeleteShader(VertexShaderID);

    return ProgramID;
}

void COpenGLHandler::useShader(std::uint32_t shaderId, bool sendData)
{
    std::cout << "COpenGLHandler::" << __func__ << "(): sendData is " << std::boolalpha << sendData << std::endl;
    glUseProgram(shaderId);

    mObserverMatrixLocation = glGetUniformLocation(shaderId, "observerMatrix");
    mTransformMatrixLocation = glGetUniformLocation(shaderId, "transformMatrix");
    mProjectionMatrixLocation = glGetUniformLocation(shaderId, "projectionMatrix");

    if (sendData == true)
    {
        glUniformMatrix4fv(mObserverMatrixLocation, 1, false, &mDefaultObserverMatrix[0][0]);
        glUniformMatrix4fv(mTransformMatrixLocation, 1, false, &mDefaultTransformMatrix[0][0]);
        mCurrentTransformMatrix = mDefaultTransformMatrix;
        glUniformMatrix4fv(mProjectionMatrixLocation, 1, false, &mDefaultProjectionMatrix[0][0]);
    }
}

void COpenGLHandler::setOpenGLCallbacks()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;
    glutDisplayFunc(COpenGLHandler::renderFunctionCallback);
    glutSpecialFunc(COpenGLHandler::specialKeyFunctionCallback);
    glutSpecialUpFunc(COpenGLHandler::specialKeyUpFunctionCallback);
    glutIdleFunc(COpenGLHandler::idleFunctionCallback);
}

void COpenGLHandler::renderFunctionCallback()
{
    stCurrentObject->renderFunction();
}

void COpenGLHandler::specialKeyFunctionCallback(int key, int x, int y)
{
    (void) x;
    (void) y;

    stCurrentObject->specialKeyFunction(key);
}

void COpenGLHandler::specialKeyUpFunctionCallback(int key, int x, int y)
{
    (void) x;
    (void) y;

    stCurrentObject->specialKeyUpFunction(key);
}

void COpenGLHandler::idleFunctionCallback()
{
    stCurrentObject->idleFunction();
}

void COpenGLHandler::timerFuncCallback(int value)
{
    stCurrentObject->timerFunc(value);
}

void COpenGLHandler::renderFunction()
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    glClear(GL_COLOR_BUFFER_BIT);

    for (auto & func : mDrawFunctions)
    {
        func();
    }

    glutSwapBuffers();
}

void COpenGLHandler::specialKeyFunction(int key)
{
    // std::cout << "COpenGLHandler::" << __func__ << "(): key = " << key << std::endl;

    switch(key)
    {
        case GLUT_KEY_UP:
            mListener.upArrowPressed();
            break;
        case GLUT_KEY_DOWN:
            mListener.downArrowPressed();
            break;
        case GLUT_KEY_LEFT:
            mListener.leftArrowPressed();
            break;
        case GLUT_KEY_RIGHT:
            mListener.rightArrowPressed();
            break;
        default:
            break;
    }
}

void COpenGLHandler::specialKeyUpFunction(int key)
{
    // std::cout << "COpenGLHandler::" << __func__ << "(): key = " << key << std::endl;

    switch(key)
    {
        case GLUT_KEY_UP:
            mListener.upArrowReleased();
            break;
        case GLUT_KEY_DOWN:
            mListener.downArrowReleased();
            break;
        case GLUT_KEY_LEFT:
            mListener.leftArrowReleased();
            break;
        case GLUT_KEY_RIGHT:
            mListener.rightArrowReleased();
            break;
        default:
            break;
    }
}

void COpenGLHandler::idleFunction()
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    mListener.newFrame();
}

void COpenGLHandler::timerFunc(std::uint32_t functionId)
{
    mTimerFunctions[functionId]();
    mTimerFunctions.erase(functionId);
}

std::uint32_t COpenGLHandler::addLine(glm::vec4 firstPoint, glm::vec4 secondPoint,
        glm::vec4 color, float lineWidth, bool reDraw)
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    std::swap(firstPoint[0], firstPoint[1]);
    std::swap(secondPoint[0], secondPoint[1]);

    std::uint32_t sizeBeforeAdd = mVboData.size();

    mVboData.push_back(firstPoint);
    mColorData.push_back(color);

    mVboData.push_back(secondPoint);
    mColorData.push_back(color);

    std::function<void()> drawFunc = [start = sizeBeforeAdd, lineWidth, this]()
    {
        if (mCurrentLineWidth != lineWidth)
        {
            mCurrentLineWidth = lineWidth;
            glLineWidth(lineWidth);
        }

        glm::mat4 transformMatrix = mDefaultTransformMatrix;
        if (mIsSpinning.find(start) != mIsSpinning.end())
        {
            glm::vec4 spinCenter = (mVboData[start] + mVboData[start+1] +
                mVboData[start+2] + mVboData[start+3]) / 4.0f;

            transformMatrix =
                getSpinMatrix(spinCenter, mSpinningAngle[start]) * transformMatrix;
        }

        if (mIsRotating == true)
        {
            transformMatrix =
                getRotateMatrix(mCenter, mRotationAngle) * transformMatrix;
        }

        if (transformMatrix != mCurrentTransformMatrix)
        {
            glUniformMatrix4fv(mTransformMatrixLocation, 1, false, &transformMatrix[0][0]);
            mCurrentTransformMatrix = transformMatrix;
        }

        glDrawArrays(GL_LINES, start, 2);
    };

    mDrawFunctions.push_back(drawFunc);

    if (reDraw == true)
    {
        this->reDraw();
    }

    std::uint32_t figureId = 0u;

    if (mAvailableIds.empty() == true)
    {
        figureId = ++mNextAvailableId;
    }
    else
    {
        figureId = mAvailableIds.front();
        mAvailableIds.pop_front();
    }
    mExternalId[figureId] = {sizeBeforeAdd, mDrawFunctions.size()-1};

    return figureId;
}

std::uint32_t COpenGLHandler::addFilledRectangle(glm::vec4 firstPoint, glm::vec4 secondPoint,
        glm::vec4 thirdPoint, glm::vec4 fourthPoint, glm::vec4 color, bool reDraw)
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    std::swap(firstPoint[0], firstPoint[1]);
    std::swap(secondPoint[0], secondPoint[1]);
    std::swap(thirdPoint[0], thirdPoint[1]);
    std::swap(fourthPoint[0], fourthPoint[1]);

    std::uint32_t sizeBeforeAdd = mVboData.size();

    mVboData.push_back(firstPoint);
    mColorData.push_back(color);

    mVboData.push_back(secondPoint);
    mColorData.push_back(color);

    mVboData.push_back(thirdPoint);
    mColorData.push_back(color);

    mVboData.push_back(fourthPoint);
    mColorData.push_back(color);

    std::function<void()> drawFunc = [start = sizeBeforeAdd, this]()
    {
        glm::mat4 transformMatrix = mDefaultTransformMatrix;
        if (mIsSpinning.find(start) != mIsSpinning.end())
        {
            glm::vec4 spinCenter = (mVboData[start] + mVboData[start+1] +
                mVboData[start+2] + mVboData[start+3]) / 4.0f;

            transformMatrix =
                getSpinMatrix(spinCenter, mSpinningAngle[start]) * transformMatrix;
        }

        if (mIsRotating == true)
        {
            transformMatrix =
                getRotateMatrix(mCenter, mRotationAngle) * transformMatrix;
        }

        if (transformMatrix != mCurrentTransformMatrix)
        {
            glUniformMatrix4fv(mTransformMatrixLocation, 1, false, &transformMatrix[0][0]);
            mCurrentTransformMatrix = transformMatrix;
        }
        glDrawArrays(GL_TRIANGLE_STRIP, start, 4);
    };

    mDrawFunctions.push_back(drawFunc);

    if (reDraw == true)
    {
        this->reDraw();
    }

    std::uint32_t figureId = 0u;

    if (mAvailableIds.empty() == true)
    {
        figureId = ++mNextAvailableId;
    }
    else
    {
        figureId = mAvailableIds.front();
        mAvailableIds.pop_front();
    }
    mExternalId[figureId] = {sizeBeforeAdd, mDrawFunctions.size()-1};

    return figureId;
}

void COpenGLHandler::moveFilledRectangle(std::uint32_t figureId,
            glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint,
            std::chrono::milliseconds time)
{
    mIsMoving = true;
    std::swap(firstPoint[0], firstPoint[1]);
    std::swap(secondPoint[0], secondPoint[1]);
    std::swap(thirdPoint[0], thirdPoint[1]);
    std::swap(fourthPoint[0], fourthPoint[1]);

    smoothMoveFilledRectangle(mExternalId[figureId].first, firstPoint, secondPoint,
            thirdPoint, fourthPoint, time);
}

void COpenGLHandler::smoothMoveFilledRectangle(std::uint32_t startPos,
            glm::vec4 firstPoint, glm::vec4 secondPoint,
            glm::vec4 thirdPoint, glm::vec4 fourthPoint,
            std::chrono::milliseconds time)
{
    // std::cout << "COpenGLHandler::" << __func__ << "(): time = " << time.count() << std::endl;

    if (time <= 0ms)
    {
        mVboData[startPos] = firstPoint;
        mVboData[startPos+1] = secondPoint;
        mVboData[startPos+2] = thirdPoint;
        mVboData[startPos+3] = fourthPoint;
        reDraw();
        mIsMoving = false;
    }
    else
    {
        movePoint(mVboData[startPos],   firstPoint, time);
        movePoint(mVboData[startPos+1], secondPoint, time);
        movePoint(mVboData[startPos+2], thirdPoint, time);
        movePoint(mVboData[startPos+3], fourthPoint, time);
        reDraw();

        std::uint32_t funcId = 0;

        do
        {
            funcId = NUtility::generateRandomBetween(0u, NUtility::timerFuncNum);
        }
        while (mTimerFunctions.find(funcId) != mTimerFunctions.end());

        mTimerFunctions[funcId] = std::bind(&COpenGLHandler::smoothMoveFilledRectangle,
                this, startPos, firstPoint, secondPoint, thirdPoint, fourthPoint,
                time-timeInFrame);

        glutTimerFunc(timeInFrame.count(), timerFuncCallback, funcId);
    }
}

void COpenGLHandler::movePoint(glm::vec4 & from, glm::vec4 to,
        std::chrono::milliseconds time)
{
    std::uint32_t framesLeft = time / timeInFrame + 1;
    float distanceX = to[0] - from[0];
    float distanceY = to[1] - from[1];
    float distanceToMoveX = distanceX / framesLeft;
    float distanceToMoveY = distanceY / framesLeft;

    // std::cout << "COpenGLHandler::" << __func__ << "(): " << distanceToMoveX << ' ' << distanceToMoveY << std::endl;

    from[0] += distanceToMoveX;
    from[1] += distanceToMoveY;
}

void COpenGLHandler::removeFilledRectangle(std::uint32_t figureId, bool reDraw)
{
    std::cout << "COpenGLHandler::" << __func__ << "(): figureId = " << figureId << std::endl;
    std::uint32_t startPos = mExternalId[figureId].first;
    std::uint32_t functionPos = mExternalId[figureId].first;

    mVboData.erase(mVboData.begin()+startPos, mVboData.begin()+startPos+4);
    mColorData.erase(mColorData.begin()+startPos, mColorData.begin()+startPos+4);
    mDrawFunctions.erase(mDrawFunctions.begin()+functionPos);

    mExternalId.erase(figureId);

    for (auto & id : mExternalId)
    {
        if (id.second.first > startPos)
        {
            id.second.first -= 4;
        }
        if (id.second.second > functionPos)
        {
            --id.second.second;
        }
    }

    mAvailableIds.push_back(figureId);

    if (reDraw == true)
    {
        this->reDraw();
    }
}

void COpenGLHandler::clearDrawData()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    mVboData.clear();
    mDrawFunctions.clear();
    mExternalId.clear();
}

void COpenGLHandler::reDraw()
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    /* Create buffer and set as active */
    glGenBuffers(1, &mVboId);
    glBindBuffer(GL_ARRAY_BUFFER, mVboId);

    /* Fill buffer with point coords */
    glBufferData(GL_ARRAY_BUFFER, mVboData.size()*sizeof(decltype(mVboData)::value_type),
            mVboData.data(), GL_STATIC_DRAW);

    /* Set buffer as coord buffer */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);


    /* Create buffer and set as active */
    glGenBuffers(1, &mColorBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, mColorBufferId);

    /* Fill buffer with point colors */
    glBufferData(GL_ARRAY_BUFFER, mColorData.size()*sizeof(decltype(mColorData)::value_type),
            mColorData.data(), GL_STATIC_DRAW);

    /* Set buffer as color buffer */
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, 0);

    glutPostRedisplay();
}


void COpenGLHandler::destroyVBO()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    /* Cleanup */
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &mVboId);
    glDeleteBuffers(1, &mColorBufferId);
}

void COpenGLHandler::destroyShaders()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    /* Cleanup */
    glDeleteProgram(mDefaultShaderId);
}

bool COpenGLHandler::isMoving()
{
    return mIsMoving;
}

void COpenGLHandler::spin(std::uint32_t figureId)
{
    mIsSpinning[mExternalId[figureId].first] = true;
    updateSpinAngle(mExternalId[figureId].first);
}

void COpenGLHandler::stopSpin(std::uint32_t figureId)
{
    mIsSpinning.erase(mExternalId[figureId].first);
    mSpinningAngle.erase(mExternalId[figureId].first);
    reDraw();
}

void COpenGLHandler::updateSpinAngle(std::uint32_t start)
{
    if (mIsSpinning.find(start) != mIsSpinning.end())
    {
        mSpinningAngle[start] += 10;
        reDraw();

        std::uint32_t funcId = 0;
        do
        {
            funcId = NUtility::generateRandomBetween(0u, NUtility::timerFuncNum);
        }
        while (mTimerFunctions.find(funcId) != mTimerFunctions.end());

        mTimerFunctions[funcId] = std::bind(&COpenGLHandler::updateSpinAngle,
                this, start);

        glutTimerFunc(timeInFrame.count(), timerFuncCallback, funcId);
    }
}

glm::mat4 COpenGLHandler::getSpinMatrix(glm::vec4 center, float angle)
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    glm::mat4 rotationMatrix;
    glm::vec3 spinCenter = {center[0], center[1], center[2]};

    rotationMatrix = glm::translate(rotationMatrix, spinCenter);

    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle),
            glm::vec3{{0.0f, 0.0f, 1.0f}});

    rotationMatrix = glm::translate(rotationMatrix, -spinCenter);

    return rotationMatrix;
}

void COpenGLHandler::rotateScreen(std::chrono::milliseconds time, float angle)
{
    mIsRotating = true;
    smoothRotateScreen(time, angle);
}

void COpenGLHandler::stopRotate(std::chrono::milliseconds time)
{
    smoothStopRotate(time, mRotationAngle);
}

void COpenGLHandler::smoothRotateScreen(
        std::chrono::milliseconds remaningTime, float angle)
{
    if (remaningTime > 0ms)
    {
        std::uint32_t framesLeft = remaningTime / timeInFrame + 1;
        float moveAngle = angle / framesLeft;

        mRotationAngle += moveAngle;

        std::uint32_t funcId = 0u;

        do
        {
            funcId = NUtility::generateRandomBetween(0u, NUtility::timerFuncNum);
        }
        while (mTimerFunctions.find(funcId) != mTimerFunctions.end());

        mTimerFunctions[funcId] = std::bind(&COpenGLHandler::smoothRotateScreen,
                this, remaningTime - timeInFrame, angle - moveAngle);

        glutTimerFunc(timeInFrame.count(), timerFuncCallback, funcId);
    }
    else
    {
        mRotationAngle += angle;
    }
    reDraw();
}

void COpenGLHandler::smoothStopRotate(
        std::chrono::milliseconds remaningTime, float angle)
{
    if (remaningTime > 0ms)
    {
        std::uint32_t framesLeft = remaningTime / timeInFrame + 1;
        float moveAngle = angle / framesLeft;

        mRotationAngle -= moveAngle;

        std::uint32_t funcId = 0u;

        do
        {
            funcId = NUtility::generateRandomBetween(0u, NUtility::timerFuncNum);
        }
        while (mTimerFunctions.find(funcId) != mTimerFunctions.end());

        mTimerFunctions[funcId] = std::bind(&COpenGLHandler::smoothStopRotate,
                this, remaningTime - timeInFrame, angle - moveAngle);

        glutTimerFunc(timeInFrame.count(), timerFuncCallback, funcId);
    }
    else
    {
        mRotationAngle -= angle;
        mIsRotating = false;
    }
    reDraw();
}

glm::mat4 COpenGLHandler::getRotateMatrix(glm::vec3 center, float angle)
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    glm::mat4 rotationMatrix;

    rotationMatrix = glm::translate(rotationMatrix, center);

    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(angle),
            glm::vec3{{0.0f, 0.0f, 1.0f}});

    rotationMatrix = glm::translate(rotationMatrix, -center);

    return rotationMatrix;
}




