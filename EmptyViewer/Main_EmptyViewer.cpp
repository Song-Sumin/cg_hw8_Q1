#define _CRT_SECURE_NO_WARNINGS
#define GLFW_INCLUDE_GLU
#define GLFW_DLL

#include <Windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "load_mesh.h"

int winWidth = 500;
int winHeight = 500;

float totalTime = 0.0f;
int totalFrames = 0;
GLuint timerQuery;

struct VertexData {
    glm::vec3 position;
    glm::vec3 normalVec;
};

std::vector<VertexData> meshVertices;
std::vector<int> triangleIndices;
int vertexCount = 0, triangleCount = 0;

void drawImmediateMode() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

    glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    float lightPos[] = { lightDirection.x, lightDirection.y, lightDirection.z, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < triangleCount; ++i) {
        int idx[3] = {
            triangleIndices[i * 3],
            triangleIndices[i * 3 + 1],
            triangleIndices[i * 3 + 2]
        };
        for (int j = 0; j < 3; ++j) {
            glm::vec3 norm = meshVertices[idx[j]].normalVec;
            glm::vec3 pos = meshVertices[idx[j]].position;
            glNormal3f(norm.x, norm.y, norm.z);
            glVertex3f(pos.x, pos.y, pos.z);
        }
    }
    glEnd();
}

void framebufferResize(GLFWwindow*, int newW, int newH) {
    winWidth = newW;
    winHeight = newH;
    glViewport(0, 0, newW, newH);
}

int main(int argc, char* argv[]) {
    if (!glfwInit())
        return -1;

    GLFWwindow* mainWindow = glfwCreateWindow(winWidth, winHeight, "Rendering Bunny", NULL, NULL);
    if (!mainWindow) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(mainWindow);
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    float ambientGlobal[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    float lightAmbient[] = { 0.f, 0.f, 0.f, 1.f };
    float lightDiffuse[] = { 1.f, 1.f, 1.f, 1.f };
    float lightSpecular[] = { 0.f, 0.f, 0.f, 1.f };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientGlobal);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor3f(1.f, 1.f, 1.f);

    float matSpec[] = { 0.f, 0.f, 0.f, 1.f };
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.f);

    glfwSetFramebufferSizeCallback(mainWindow, framebufferResize);
    framebufferResize(mainWindow, winWidth, winHeight);

    load_mesh("bunny.obj");
    triangleCount = static_cast<int>(gTriangles.size());
    vertexCount = static_cast<int>(gPositions.size());

    meshVertices.resize(vertexCount);
    triangleIndices.resize(triangleCount * 3);

    for (size_t i = 0; i < vertexCount; ++i) {
        meshVertices[i].position = glm::vec3(gPositions[i].x, gPositions[i].y, gPositions[i].z);
        meshVertices[i].normalVec = glm::vec3(gNormals[i].x, gNormals[i].y, gNormals[i].z);
    }

    for (size_t i = 0; i < triangleCount; ++i) {
        triangleIndices[i * 3] = gTriangles[i].indices[0];
        triangleIndices[i * 3 + 1] = gTriangles[i].indices[1];
        triangleIndices[i * 3 + 2] = gTriangles[i].indices[2];
    }

    glGenQueries(1, &timerQuery);

    while (!glfwWindowShouldClose(mainWindow)) {
        glBeginQuery(GL_TIME_ELAPSED, timerQuery);
        drawImmediateMode();
        glfwSwapBuffers(mainWindow);
        glfwPollEvents();
        glEndQuery(GL_TIME_ELAPSED);

        GLint ready = GL_FALSE;
        while (!ready)
            glGetQueryObjectiv(timerQuery, GL_QUERY_RESULT_AVAILABLE, &ready);

        GLint elapsedTime;
        glGetQueryObjectiv(timerQuery, GL_QUERY_RESULT, &elapsedTime);
        float frameTime = elapsedTime / (1000.0f * 1000.0f * 1000.0f);

        totalFrames++;
        totalTime += frameTime;
        float currentFPS = totalFrames / totalTime;

        char windowTitle[256];
        std::snprintf(windowTitle, sizeof(windowTitle), "GL Bunny : %.2f", currentFPS);
        glfwSetWindowTitle(mainWindow, windowTitle);

        if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS ||
            glfwGetKey(mainWindow, GLFW_KEY_Q) == GLFW_PRESS) {
            glfwSetWindowShouldClose(mainWindow, GL_TRUE);
        }
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
