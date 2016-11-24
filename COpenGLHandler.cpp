#include "COpenGLHandler.hpp"
#include "utility.hpp"
#include <iostream>
#include <fstream>

COpenGLHandler * COpenGLHandler::stCurrentObject {nullptr};

COpenGLHandler::COpenGLHandler(int argc, char ** argv)
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;
    /* Initialize open GL */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    /* Initialize window */
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(NUtility::width, NUtility::height);
    glutCreateWindow(NUtility::gameName.c_str());

    /* Initialize glew */
    glewInit();

    /* Background color black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

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
        glUniformMatrix4fv(mProjectionMatrixLocation, 1, false, &mDefaultProjectionMatrix[0][0]);
    }
}

void COpenGLHandler::setOpenGLCallbacks()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;
    glutDisplayFunc(COpenGLHandler::renderFunctionCallback);
}

void COpenGLHandler::renderFunctionCallback()
{
    stCurrentObject->renderFunction();
}

void COpenGLHandler::renderFunction()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    glClear(GL_COLOR_BUFFER_BIT);

    for (auto & func : mDrawFunctions)
    {
        func();
    }

    glFlush();
}

void COpenGLHandler::addLine(glm::vec4 firstPoint, glm::vec4 secondPoint,
        glm::vec4 color, float lineWidth)
{
    // std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

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
        glDrawArrays(GL_LINES, start, 2);
    };

    mDrawFunctions.push_back(drawFunc);
}

void COpenGLHandler::clearDrawData()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

    mVboData.clear();
    mDrawFunctions.clear();
}

void COpenGLHandler::reDraw()
{
    std::cout << "COpenGLHandler::" << __func__ << "()" << std::endl;

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












