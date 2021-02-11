#ifndef SANDBOXGAME_H
#define SANDBOXGAME_H

#include "Fracture.h"

namespace Sandbox
{

	class SandboxGame : public Fracture::Game
	{

	public:
		SandboxGame();
		~SandboxGame() = default;
	};
}

#endif

