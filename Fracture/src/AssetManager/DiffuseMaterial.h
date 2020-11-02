#pragma once
#ifndef DIFFUSEMAT_H
#define DIFFUSEMAT_H

#include "Rendering/Material.h"
#include "Rendering/Shader.h"

namespace Fracture
{

	class DiffuseMaterial :public Material
	{
	public:
		DiffuseMaterial(std::string name,std::string texture,std::shared_ptr<Shader> shader):Material(name,shader)
		{
		}	

	private:
		std::shared_ptr<Texture> m_diffuse;
	};


}

#endif