#include <iostream>
#include <GLFW/glfw3.h>
#include <helper.h>

int walkthemodel(std::string objPath1, std::string objPath2);


std::string objPath1 = "resources/Yoshi/Yoshi.obj";
std::string objPath2 = "resources/Kleinklotz/Kleinklotz.obj";


int main() {
    walkthemodel(objPath1, objPath2);
}

// needed for this project
int WinMain() { 
    walkthemodel(objPath1, objPath2);
}