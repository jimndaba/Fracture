#pragma once
#ifndef DIFFUSEMAT_H
#define DIFFUSEMAT_H

#include "AssetManager.h"
#include "Rendering/Material.h"
#include "Rendering/Shader.h"

namespace Fracture
{

	class DiffuseMaterial :public Material
	{
	public:
		DiffuseMaterial(std::string name,std::string texture,std::shared_ptr<Shader> shader):Material(name,shader)
		{
			m_diffuse = AssetManager::getTexture(texture);
			SetTexture("material.diffuse", m_diffuse, 0);
		}	

	private:
		std::shared_ptr<Texture> m_diffuse;
	};

	class UnTexturedMaterial : public Material
	{
	public:
		UnTexturedMaterial(std::string name, std::shared_ptr<Shader> shader) :Material(name, shader)
		{			
		}

		void SetDiffuse(glm::vec3 diffuse)
		{
			getShader()->use();
			setVec3("material.diffuse", diffuse);
			m_diffuse = diffuse;
		}

		void SetAmbient(glm::vec3 ambient)
		{
			getShader()->use();
			setVec3("material.diffuse", ambient);
			m_ambient = ambient;
		}

		void SetSpecular(glm::vec3 specular)
		{
			getShader()->use();
			setVec3("material.diffuse", specular);
			m_specular = specular;
		}

		void SetShininess(float shininess) {
			getShader()->use();
			setFloat("material.shininess", shininess);
			m_shininess = shininess;
		}

	private:
		glm::vec3 m_diffuse = glm::vec3(1.0f);
		glm::vec3 m_ambient = glm::vec3(1.0f);
		glm::vec3 m_specular = glm::vec3(1.0f);
		float m_shininess = 1.0f;
	};


}

#endif