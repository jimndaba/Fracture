#pragma once
#ifndef PHYSICSMATERIAL_H
#define PHYSICSMATERIAL_H

namespace Fracture
{
	struct PhyiscsMaterialReg
	{
		Fracture::UUID ID;
		std::string Name;
		std::string Path;
	};

	struct PhysicsMaterial
	{
		PhysicsMaterial();
		Fracture::UUID ID;

		float DynamicFrition;
		float StaticFrition;
		float Restitution;
		float Density;
	};



}

#endif