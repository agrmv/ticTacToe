#pragma once

#include <sstream>
#include <vector>
#include <mutex>
#include <cmath>
#include <GL/freeglut.h>
#include "game.hpp"

class Glut : protected Game {
public:
    const static size_t scale = 150;
    std::pair<int, int> window;
    std::pair<size_t, size_t> sizeOfWindow{sizeOfBoard  * scale, sizeOfBoard  * scale};

    static std::shared_ptr<Glut> getInstance() {
        static std::weak_ptr<Glut> pInstance;
        std::shared_ptr<Glut> instance = pInstance.lock();
        if (pInstance.expired()) {
            instance.reset(new Glut);
            pInstance = instance;
        }
        return instance;
    }
    static void displayGameWindow();
    static void displayChatWindow();
    static void mouseWheel(int button, int state, int x, int y);
    static void mousePressed(int button, int state, int x, int y);
    static void reshape(int x, int y);
    void init() const;

private:
    double offset;
    size_t massageCount, countWheelUp;
    std::vector<std::string> names;
    Glut() : massageCount(0), countWheelUp(0), offset(0), Game("gamerX", "gamerO") {}
    void drawO(const size_t& i, const size_t& j) const;
    void drawX(const size_t& i, const size_t& j) const;
    void drawField()   const;
    void drawCards()   const;
    void printResult() const;
    void printChat()   const;
    void  drawStringBitmap(const std::string& s) const;
    static void keyboard(unsigned char key, int x, int y);
    static void timer(int);

};

void Glut::drawO(const size_t& i, const size_t& j) const {
    glBegin(GL_LINE_LOOP);

    for (size_t k = 0; k < 36; ++k) {
        double angle = 2. * M_PI * static_cast<double>(k) / static_cast<double>(36);

        double dx = ((scale / 2) - 5) * cos(angle);
        double dy = ((scale / 2) - 5) * sin(angle);

        glVertex2f((j * scale + (scale / 2)) + dx, (i * scale + (scale / 2)) + dy);
    }

    glEnd();
}

void Glut::drawX(const size_t& i, const size_t& j) const {
    glBegin(GL_LINES);
    glVertex2i(j * scale, i * scale);
    glVertex2i(j * scale + scale, i * scale + scale);
    glVertex2i(j * scale, i * scale + scale);
    glVertex2i(j * scale + scale, i * scale);
    glEnd();
}

void Glut::drawCards() const{
    for (size_t i = 0; i < board.size(); i++) {
        for (size_t j = 0; j < board.size(); j++) {
            if (board[i][j] != -1) {
                glLineWidth(2);
                if (board[i][j] == 0) {
                    drawX(i, j);
                }
                if (board[i][j] == 1) {
                    drawO(i, j);
                }
            }
        }
    }
}

void Glut::drawField() const{
    glLineWidth(1);
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    for (double i = 0.01; i <= sizeOfWindow.first; i += scale) {
        glVertex2d(i, 0);
        glVertex2d(i, sizeOfWindow.first);
        i -= 0.01;

    }
    for (size_t i = 0; i <= sizeOfWindow.second; i += scale) {
        glVertex2i(0, i);
        glVertex2i(sizeOfWindow.first, i);
    }
    glEnd();
}

void Glut::printResult() const {
    glColor3f(1, 0, 0);
    if(!gameStatus.second) {
        glRasterPos2f(10, 40);
        drawStringBitmap("ESC   - restart game");
        glRasterPos2f(10, 60);
        drawStringBitmap(winnerName() + " won");
    } else if (checkDraw >= board.size() * board.size()) {
        glRasterPos2f(10, 40);
        drawStringBitmap("ESC   - restart game");
        glRasterPos2f(10, 60);
        drawStringBitmap("Draw!");
    } else {
        glRasterPos2f(10, 40);
        drawStringBitmap("ESC   - restart game");
        glRasterPos2f(10, 60);
        drawStringBitmap("ENTER - open chat");
    }
}

void Glut::printChat() const{
    // TODO PERENOS
    glColor3f(0, 0, 0);
    for (size_t i = 0; i < names.size(); ++i) {
        std::ostringstream oss;
        auto temp = (((i + 1) % 2) != 0) ? "gamerX" : "gamerO";
        glRasterPos2f(10, 80 + (15 * (i + 1)));
        oss << temp << ": " << names[i];
        drawStringBitmap(oss.str());
    }
}

void Glut::displayGameWindow() {
    auto ptr = getInstance();
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);

    ptr->drawField();
    ptr->drawCards();

    glutSwapBuffers();
}

void Glut::displayChatWindow() {
    auto ptr = getInstance();

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 1, 1, 1);
    glColor3f(0, 0, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPushMatrix();
    glEnable(GL_BLEND);

    glTranslatef(0.0f, ptr->offset, 0.0f);
    ptr->printResult();
    ptr->printChat();

    glutSwapBuffers();
}

void Glut::keyboard(unsigned char key, int x, int y) {
    auto ptr = getInstance();

    switch (key) {
        case 27:
            ptr->gameInit();
            ptr->checkDraw = 0;
            ptr->gameStatus = {0, true};
            break;
        case 13: {
            ptr->names.emplace_back("");
            ptr->massageCount++;
            if (ptr->massageCount > 25) {
                ptr->offset += 0.068;
            }
        }
            break;
        case 8:
            if ((ptr->massageCount != 0) && (!ptr->names.back().empty())) {
                ptr->names.back().pop_back();
            }
            break;
        default:
            if (ptr->massageCount != 0) {
                ptr->names.back().push_back(key);
                //++countWords;
            }
            break;
    }
    glutPostRedisplay();
}

void Glut::mousePressed(int button, int state, int x, int y) {
    auto ptr = getInstance();

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ptr->gameStatus.second) {
        size_t row = y / ptr->scale;
        size_t col = x / ptr->scale;

        if (ptr->board[row][col] == -1) {
            ptr->board[row][col] = ptr->gameStatus.first;
        } else {
            return;
        }

        ptr->checkVertical(col);
        ptr->checkHorizontal(row);
        ptr->checkMainDiagonal();
        ptr->checkSideDiagonal();
        ptr->gameStatus.first = !ptr->gameStatus.first;
        ++ptr->checkDraw;
    }
}

void Glut::mouseWheel(int button, int state, int x, int y) {
    auto ptr = getInstance();

    if (state == GLUT_DOWN) {
        if (button == 3 && ptr->offset > 0) {
            ptr->offset -= 0.05;
            ptr->countWheelUp++;

        }
        if (button == 4 && ptr->countWheelUp > 0) {
            ptr->offset += 0.05;
            ptr->countWheelUp--;
        }
    }
}

void Glut::timer(int) {
    auto ptr = getInstance();
    glutSetWindow(ptr->window.first);
    glutPostRedisplay();
    glutSetWindow(ptr->window.second);
    glutPostRedisplay();
    glutTimerFunc(10, timer, 0);
}

void Glut::reshape(int x, int y) {
    glViewport(0, 0, x, y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, x, y, 0);
}

void Glut::init() const {
    auto ptr = getInstance();
    gluOrtho2D(0, sizeOfWindow.first, sizeOfWindow.second, 0);
    glutKeyboardFunc(ptr->keyboard);
    glutTimerFunc(10, timer, 0);
}

void Glut::drawStringBitmap(const std::string& s) const {
    for (const auto &i: s) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, i);
    }
}
