#include <iostream>
#include <GLFW/glfw3.h>
#include <helper.h>

int walkthemodel(std::string objPath);


//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}

std::string objPath = "resources/bobrMickBlue/bobr_turm_mick_blue.obj";

int main() {
    walkthemodel(objPath);
}

// needed for this project
int WinMain() { 
    walkthemodel(objPath);
}