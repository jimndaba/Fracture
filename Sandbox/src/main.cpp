#pragma once
#include <iostream>
#include <Windows.h>
#include "Game.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	std::shared_ptr<Sandbox::SandboxGame> m_game = std::make_shared<Sandbox::SandboxGame>();

	std::shared_ptr<Fracture::Scene> m_scene = std::make_shared<Fracture::Scene>();
	
	m_game->addScene("test", m_scene);
	m_game->changeScene("test");

	m_game->run();
	std::cout << " Hello" << std::endl;
	return 0;
}