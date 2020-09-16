#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <utility>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "game.h"

struct my_color
{
    float r;
    float g;
    float b;
};


const float STEP = 1.f/static_cast<float>(BOARD_SIZE);
my_color black_obj, white_obj, chose_obj;
bool isChouse = false;
bool isFinalGame = false;
Position actualFromCoord;
Game<Pawn> * testGame = nullptr;

Position getNumInBoard(int height, int width, int xpos, int ypos) {
    int StepInPix = height/(2*static_cast<float>(BOARD_SIZE));

    int leftBorder = width/2 - StepInPix *4;
    int bottomBorder = height/2 + StepInPix *4;

    return {(xpos - leftBorder)/StepInPix, (-ypos + bottomBorder)/StepInPix};
}

static void error_callback(int error, const char* description) {
    fputs(description, stderr);
}
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (isFinalGame)
        return;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        int width, height;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window,&width, &height);

        auto res = getNumInBoard(height, width, xpos, ypos);
        res.first++;
        res.second++;

        if (isChouse) {
            if (testGame->DoStep(actualFromCoord, res)) { 
                if(testGame->whoFin() != Sides::none)
                    isFinalGame=true;
                isChouse = false;
                if (needDebug) std::cout << "UI DoStep(" << actualFromCoord <<"," << res<<std::endl;
                auto Step = testGame->AIBestStep();
                testGame->DoStep(Step.first, Step.second);
            } else {
                if (needDebug) std::cout << "UI Not DoStep(" << actualFromCoord <<"," << res<<std::endl; 
                isChouse = false;
            }
        } else {
            if(testGame->ChooseObj(res)) {
                actualFromCoord = res;
                isChouse = true;
            }
        }
        
    }
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, GL_TRUE);
}
void drawBoardCell(const int row, const int col, const bool isBlack) {
    float xFrom, xTo, yFrom, yTo;
    xFrom = -0.5f+static_cast<float>(row)*STEP;
    yFrom = -0.5f+static_cast<float>(col)*STEP;
    xTo = xFrom+STEP;
    yTo = yFrom+STEP;
    glBegin(GL_TRIANGLES);
    if (isBlack)
        glColor3f(0.3f, 0.3f, 0.3f);
    else
        glColor3f(0.8f, 0.8f, 0.8f);
    
    glVertex3f(xFrom, yFrom, 0.f);
    glVertex3f(xFrom, yTo, 0.f);
    glVertex3f(xTo, yTo, 0.f);
    glEnd();
    glBegin(GL_TRIANGLES);
    if (isBlack)
        glColor3f(0.3f, 0.3f, 0.3f);
    else
        glColor3f(0.8f, 0.8f, 0.8f);
    glVertex3f(xFrom, yFrom, 0.f);
    glVertex3f(xTo, yFrom, 0.f);
    glVertex3f(xTo, yTo, 0.f);
    glEnd();
}
void drawObjInCell(const int row, const int col, const my_color& color) {

    float xFrom, xTo, yFrom, yTo;
    xFrom = -0.5f+static_cast<float>(row)*STEP;
    yFrom = -0.5f+static_cast<float>(col)*STEP;
    xTo = xFrom+STEP;
    yTo = yFrom+STEP;

    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    
    glVertex3f(xTo, yFrom, 0.f);
    glVertex3f((xFrom+xTo)/2, yTo, 0.f);
    glVertex3f(xFrom, yFrom, 0.f);
    glEnd();
}

void drawFinal(const my_color& color) {
    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    
    glVertex3f(0.0f, -0.5f, 0.f);
    glVertex3f(-0.5f, 0.5f, 0.f);
    glVertex3f(-0.5f, -0.5f, 0.f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    
    glVertex3f(0.3f, -0.5f,  0.f);
    glVertex3f(0.0f, 0.7f, 0.f);
    glVertex3f(-0.3f, -0.5f, 0.f);
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3f(color.r, color.g, color.b);
    
    glVertex3f(0.0f, -0.5f, 0.f);
    glVertex3f(0.5f, 0.5f, 0.f);
    glVertex3f(0.5f, -0.5f, 0.f);
    glEnd();
}

void display (GLFWwindow * window) {
    int width, height;
    
    glfwGetFramebufferSize(window, &width, &height);

    float ratio = width/(float) height;
    
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (isFinalGame) {
        if (testGame != nullptr)
            drawFinal(testGame->whoFin() == Sides::white ? white_obj: black_obj);
        return;
    }

    for (unsigned i = 0; i < BOARD_SIZE; ++i) {
        for(unsigned j = 0; j < BOARD_SIZE; ++j) {
            drawBoardCell(i, j, (i + j) % 2 == 0);
        }
    }
    
    for (auto& cell: testGame->Cells()) {
        if (cell.second != Sides::none)
            drawObjInCell(cell.first.first-1, cell.first.second-1,
                isChouse && actualFromCoord == cell.first ? chose_obj :
                cell.second == Sides::black ? black_obj : white_obj);
    }
}

void createBaseInfo() {
    black_obj.r = black_obj.g = black_obj.b = 0.5f;
    white_obj.r = white_obj.g = white_obj.b = 1.0f;
    white_obj.r = white_obj.g = white_obj.b = 1.0f;

    chose_obj.r = 0.2f;
    chose_obj.g = chose_obj.b = 1.0f;

    std::vector<Position> whitePosition, blackPosition;

    std::map<Sides, std::vector<Position>> startPosition, finalPosition;
    whitePosition.reserve(3*3);
    blackPosition.reserve(3*3);

    for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            whitePosition.push_back({i, j});
        }
    }
    for (int i = 8; i > 5; i--) {
        for (int j = 8; j > 5; j--) {
            blackPosition.push_back({i, j});
        }
    }


    startPosition[Sides::black] = blackPosition;
    startPosition[Sides::white] = whitePosition;

    finalPosition[Sides::white] = blackPosition;
    finalPosition[Sides::black] = whitePosition;

    testGame = new Game<Pawn> (startPosition, finalPosition);


}

GLFWwindow* createWindow() {
    GLFWwindow* window ;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    return window;
}
int main(int argc, char **argv)
{
    createBaseInfo();
    GLFWwindow* window = createWindow();

    while (!glfwWindowShouldClose(window))
    {
        display(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    if (testGame)
        delete testGame;
    
    exit(EXIT_SUCCESS);
}
