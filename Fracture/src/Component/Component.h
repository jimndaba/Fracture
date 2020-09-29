#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

namespace Fracture
{
	enum class ComponentType
	{
		None,
		Transform,
		Render,
		Camera,
		Mesh,
		Physics,
		Script,
		DirectLight,
		PointLight,
		SpotLight,
		Audio,
	};

	class Component
	{
	public:
		Component(int id,ComponentType mtype):entityID(id),componentType(mtype)
		{
		};
		~Component() {};
		virtual void onAttach() = 0;
		virtual void onDettach() = 0;

		int entityID;
		ComponentType componentType = ComponentType::None;

		
	};
	
}
#endif