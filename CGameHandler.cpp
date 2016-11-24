#include "CGameHandler.hpp"

CGameHandler::CGameHandler(COpenGLHandler & openGLHandler)
: mOpenGLHandler(openGLHandler)
{
    drawMaze();
}

void CGameHandler::drawMaze()
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
    mOpenGLHandler.reDraw();
}

