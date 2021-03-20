#pragma once
#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <string>
#include <memory>
#include "glm/glm.hpp"

namespace Fracture
{
	class Shader;
	class Material;
	class Font;
	class Camera2D;

	class Renderer2D
	{
	public:
		Renderer2D();
		~Renderer2D() = default;

		void DrawText2D(const std::string& text, float x, float y, float scale, glm::vec3 color);

		void setViewport(int width, int height);

		void SetFont(const std::string& font);
		void SetCamera(const std::shared_ptr<Camera2D>& camera);
		std::shared_ptr<Camera2D> ActiveCamera();

	private:
		int m_width;
		int m_height;
		std::shared_ptr<Font> m_CurrentFont;
		std::shared_ptr<Shader> m_TextShader;
		std::shared_ptr<Camera2D> m_Camera2D;
	};
}

#endif