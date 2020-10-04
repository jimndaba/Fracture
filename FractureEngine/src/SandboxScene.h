#pragma once
#ifndef SANDBOXSCENE_H
#define SANDBOXSCENE_H

#include "Fracture.h"
#include "Scene/Scene.h"


namespace Fracture
{

	class SandboxScene :public Fracture::Scene
	{
	public:
		SandboxScene();
		~SandboxScene();
		void onLoad() override;
	};


}

#endif