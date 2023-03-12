#pragma once
#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H


namespace Fracture
{
	struct TransformComponent;

	struct TransformSystem
	{
		TransformSystem();


		static void Update(float dt, TransformComponent& component);


	};



}


#endif