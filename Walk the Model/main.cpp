#include <iostream>
#include <GLFW/glfw3.h>
#include <helper.h>

int walkthemodel(std::string objPath);


std::string objPath = "resources/Yoshi/Yoshi.obj";

int main() {
    walkthemodel(objPath);
}

// needed for this project
int WinMain() { 
    walkthemodel(objPath);
}