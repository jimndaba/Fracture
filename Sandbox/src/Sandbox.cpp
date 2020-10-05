// Sandbox.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Fracture.h"
#include "SandboxScene.h"


int main(int argc, char** argv)
{  
    std::cout << "FRACTURE ENGINE 2020\n";
    Fracture::Game game;

    std::shared_ptr<SandboxScene> scene = std::shared_ptr<SandboxScene>(new SandboxScene());
    game.addScene(scene);

    game.run();
    return 0;
}
