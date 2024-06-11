#pragma once
#ifndef TERRAINSYSTEM_H
#define TERRAINSYSTEM_H


namespace Fracture
{

	class TerrainSystem
	{
	public:
		void GenerateTerrain(Fracture::UUID entity);
		void UpdateTerrain(Fracture::UUID terrain);
	};

}


#endif