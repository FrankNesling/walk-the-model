#include <iostream>
#include <GLFW/glfw3.h>
#include <helper.h>

int walkthemodel(std::string objPath1, std::string objPath2, std::string objPath3);

// TODO: Find SimPO & Migu
// other ideas: Balkanferrari, Hakki/Pogel Reihenhaus
std::string objPath1 = "resources/Tower/Tower.obj";
std::string objPath2 = "resources/Urs/Urs.obj";
std::string objPath3 = "resources/Kapelle/Kapelle.obj";

// TODO: clipping

int main() {
    walkthemodel(objPath1, objPath2, objPath3);
}

// needed for this project
int WinMain() { 
    walkthemodel(objPath1, objPath2, objPath3);
}