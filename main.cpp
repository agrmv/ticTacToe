#include "glut.hpp"

int main(int argc, char** argv) {
    auto gPtr = Glut::getInstance();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(gPtr->sizeOfWindow.first * 2, gPtr->sizeOfWindow.second);
    glutInitWindowPosition(480, 200);

    gPtr->window.first = glutCreateWindow("TicTacToe");
    glutDisplayFunc(gPtr->displayChatWindow);
    glutMouseFunc(gPtr->mouseWheel);
    glutReshapeFunc(gPtr->reshape);
    gPtr->init();

    gPtr->window.second = glutCreateSubWindow(gPtr->window.first, gPtr->sizeOfWindow.first, 0, gPtr->sizeOfWindow.first,
            gPtr->sizeOfWindow.second);
    glutMouseFunc(gPtr->mousePressed);
    glutDisplayFunc(gPtr->displayGameWindow);
    gPtr->init();

    glutMainLoop();
    return 0;
}
