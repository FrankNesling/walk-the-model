#include <iostream>
#include <GLFW/glfw3.h>
#include <helper.h>

int camera();


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


int main() {
    camera();
}

// needed for this project
int WinMain() { 
    camera(); 
}