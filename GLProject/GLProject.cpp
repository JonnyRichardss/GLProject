//to start -- following https://www.glfw.org/docs/3.3/quick.html
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
int main()
{
    std::cout << "Hello World!\n";
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        printf("GLFW init failed!\n");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    GLFWwindow* mainWindow = glfwCreateWindow(800, 450, "Jonny's Window", 0, 0);
    if (!mainWindow) {
        printf("GLFW window creation failed!\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(mainWindow);
    //context MUST be made current BEFORE glew init
    if (glewInit() != GLEW_OK) {
        printf("GLEW init failed!\n");
        return 1;
    }
    glfwSetKeyCallback(mainWindow, key_callback);

    int frameW, frameH;
    glfwGetFramebufferSize(mainWindow, &frameW, &frameH);
    glViewport(0, 0, frameW, frameH);//not entirely sure what this does, something to do with camera-space vs window-space

    glfwSwapInterval(1);//vsync
    printf("OpenGL version: %s\n",glGetString(GL_VERSION));
    //for-ever loops are funny, I will die on this hill
    double time = 0;//clock class incoming soonTM
    for (;;){
        if (glfwWindowShouldClose(mainWindow)) break;
        time = glfwGetTime();
        //input
        glfwPollEvents();
        //update
        //render
        glClear(GL_COLOR_BUFFER_BIT);
        //google says these are defined in "clip space"(i think)
        glBegin(GL_TRIANGLES);
            glVertex2d(-1,-1);
            glVertex2d(0,1);
            glVertex2d(1, -1);
        glEnd();
        glfwSwapBuffers(mainWindow);
    }
    

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
