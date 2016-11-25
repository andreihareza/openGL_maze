#include "CGameHandler.hpp"

CGameHandler::CGameHandler(COpenGLHandler & openGLHandler)
: mOpenGLHandler(openGLHandler)
{
    mMaze.generateMaze(0, NUtility::lines, 0, NUtility::columns);
    drawMaze();
}

void CGameHandler::drawMaze()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;

    requestDrawPath();
    requestDrawLines();

    mOpenGLHandler.reDraw();
}

void CGameHandler::requestDrawLines()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;

    std::vector<std::tuple<std::uint32_t, std::uint32_t,
                           std::uint32_t, std::uint32_t>> walls = mMaze.getWalls();

    for (auto & wall : walls)
    {
        glm::vec4 firstPoint {std::get<0>(wall), std::get<1>(wall), 0.0, 1.0};
        glm::vec4 secondPoint {std::get<2>(wall), std::get<3>(wall), 0.0, 1.0};

        glm::vec4 color {1.0, 0.55, 0.0, 1.0};

        mOpenGLHandler.addLine(firstPoint, secondPoint, color, 2.0);
    }
}

void CGameHandler::requestDrawPath()
{
    std::cout << "CGameHandler::" << __func__ << "()" << std::endl;

    std::vector<std::pair<std::uint32_t, std::uint32_t>> path = mMaze.getLongestPath();

    std::cout << "CGameHandler::" << __func__ << "(): path.size()=" << path.size() << std::endl;

    /* ******************** All cells on path except first and last ******************** */
    if (path.size() > 2)
    {
        for (std::uint32_t i=1; i<path.size()-1; ++i)
        {
            std::pair<std::uint32_t, std::uint32_t> upCell = {path[i].first-1, path[i].second};
            std::pair<std::uint32_t, std::uint32_t> downCell = {path[i].first+1, path[i].second};
            std::pair<std::uint32_t, std::uint32_t> leftCell = {path[i].first, path[i].second-1};
            std::pair<std::uint32_t, std::uint32_t> rightCell = {path[i].first, path[i].second+1};
            float upAdd = ((path[i-1] == upCell) || (path[i+1] == upCell))? 0.0 : 0.05;
            float downAdd = ((path[i-1] == downCell) || (path[i+1] == downCell))? 0.0 : -0.05;
            float leftAdd = ((path[i-1] == leftCell) || (path[i+1] == leftCell))? 0.0 : 0.05;
            float rightAdd = ((path[i-1] == rightCell) || (path[i+1] == rightCell))? 0.0 : -0.05;

            glm::vec4 firstPoint  {path[i].first+upAdd, path[i].second+leftAdd, 0.0, 1.0};
            glm::vec4 secondPoint {path[i].first+1+downAdd, path[i].second+leftAdd, 0.0, 1.0};
            glm::vec4 thirdPoint  {path[i].first+upAdd, path[i].second+1+rightAdd, 0.0, 1.0};
            glm::vec4 fourthPoint {path[i].first+1+downAdd, path[i].second+1+rightAdd, 0.0, 1.0};

            glm::vec4 color {0.05, 0.05, 0.05, 1.0};

            mOpenGLHandler.addFilledRectangle(
                    firstPoint, secondPoint, thirdPoint, fourthPoint, color);
        }
    }

    /* ******************** All cells on path except first and last ******************** */

    /* *********************************** First cell ********************************** */
    if (path.size() > 1)
    {
        std::pair<std::uint32_t, std::uint32_t> upCell = {path[0].first-1, path[0].second};
        std::pair<std::uint32_t, std::uint32_t> downCell = {path[0].first+1, path[0].second};
        std::pair<std::uint32_t, std::uint32_t> leftCell = {path[0].first, path[0].second-1};
        std::pair<std::uint32_t, std::uint32_t> rightCell = {path[0].first, path[0].second+1};
        float upAdd = (path[1] == upCell)? 0.0 : 0.05;
        float downAdd = (path[1] == downCell)? 0.0 : -0.05;
        float leftAdd = (path[1] == leftCell)? 0.0 : 0.05;
        float rightAdd = (path[1] == rightCell)? 0.0 : -0.05;

        glm::vec4 firstPoint  {path[0].first+upAdd, path[0].second+leftAdd, 0.0, 1.0};
        glm::vec4 secondPoint {path[0].first+1+downAdd, path[0].second+leftAdd, 0.0, 1.0};
        glm::vec4 thirdPoint  {path[0].first+upAdd, path[0].second+1+rightAdd, 0.0, 1.0};
        glm::vec4 fourthPoint {path[0].first+1+downAdd, path[0].second+1+rightAdd, 0.0, 1.0};

        glm::vec4 color {1.0, 0.0, 0.0, 1.0};

        mOpenGLHandler.addFilledRectangle(
                firstPoint, secondPoint, thirdPoint, fourthPoint, color);
    }

    /* *********************************** First cell ********************************** */

    /* *********************************** Last cell *********************************** */
    const int lst = path.size()-1;

    std::pair<std::uint32_t, std::uint32_t> upCell = {path[lst].first-1, path[lst].second};
    std::pair<std::uint32_t, std::uint32_t> downCell = {path[lst].first+1, path[lst].second};
    std::pair<std::uint32_t, std::uint32_t> leftCell = {path[lst].first, path[lst].second-1};
    std::pair<std::uint32_t, std::uint32_t> rightCell = {path[lst].first, path[lst].second+1};
    float upAdd = (path[lst-1] == upCell)? 0.0 : 0.05;
    float downAdd = (path[lst-1] == downCell)? 0.0 : -0.05;
    float leftAdd = (path[lst-1] == leftCell)? 0.0 : 0.05;
    float rightAdd = (path[lst-1] == rightCell)? 0.0 : -0.05;

    glm::vec4 firstPoint  {path[lst].first+upAdd, path[lst].second+leftAdd, 0.0, 1.0};
    glm::vec4 secondPoint {path[lst].first+1+downAdd, path[lst].second+leftAdd, 0.0, 1.0};
    glm::vec4 thirdPoint  {path[lst].first+upAdd, path[lst].second+1+rightAdd, 0.0, 1.0};
    glm::vec4 fourthPoint {path[lst].first+1+downAdd, path[lst].second+1+rightAdd, 0.0, 1.0};

    glm::vec4 color {0.0, 1.0, 0.0, 1.0};

    mOpenGLHandler.addFilledRectangle(
            firstPoint, secondPoint, thirdPoint, fourthPoint, color);

    /* *********************************** Last cell *********************************** */
}




