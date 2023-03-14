#pragma once
#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include "SceneManager.h"

namespace Fracture
{

	struct TransformSystem
	{
		TransformSystem();

		static void Update(Fracture::SceneManager* manager, Fracture::UUID root);
	};



}


#endif