#include <iostream>
#include <GLFW/glfw3.h>
#include <helper.h>

int walkthemodel(std::string objPath1, std::string objPath2);


std::string objPath1 = "resources/bobr_turm_mick_blue/bobr_turm_mick_blue.obj";
std::string objPath2 = "resources/Urs/Urs.obj";


int main() {
    walkthemodel(objPath1, objPath2);
}

// needed for this project
int WinMain() { 
    walkthemodel(objPath1, objPath2);
}