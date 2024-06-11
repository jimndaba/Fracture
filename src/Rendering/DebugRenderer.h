#pragma once
#ifndef DEBUGRENDERER_H
#define DEBUGRENDERER_H

namespace Fracture
{
	struct Buffer;
	struct RenderContext;
	struct AABB;

	struct BillboardData
	{
		uint32_t Image;
		std::vector<Fracture::UUID> IDs;
		std::vector<glm::vec3> Positions;
		std::vector<glm::vec4> Colors;
	};

	struct Line
	{
		glm::vec4 Point;
		glm::vec4 Color;
	};

	class DebugRenderer
	{

	public :
		DebugRenderer();

		void OnInit();

		void OnBeginFrame();

		void OnRender();

		static void OnRenderSceneBillboards(RenderContext* Context);

		void OnShutdown();

		void OnReloadShaders();

		static void DrawLine(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color = glm::vec4(0.9f, 0.4f, 0.3f, 1.0f));
		//static void DrawLineRetained(const glm::vec3& start, const glm::vec3& end);
		static void DrawAABB(const AABB& aabb, const glm::vec4& color = glm::vec4(0.6f, 0.9f, 0.3f, 1.0f));
		static void DrawAABB(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color = glm::vec4(0.6f, 0.9f, 0.3f, 1.0f));
		static void DrawCircle(const glm::vec3& center, const float& radius, const glm::vec3& rotation = glm::vec3(0.0), const glm::vec4& color = glm::vec4(0.9f, 0.4f, 0.3f, 1.0f));
		static void DrawBillboard(const uint32_t& texture, const glm::vec3& position, const glm::vec4& color);
		static void DrawEntityBillboard(UUID entity, const uint32_t& texture, const glm::vec3& position, const glm::vec4& color);
		static void DrawSphere(const glm::vec3& position, const float& radius, const glm::vec4& color = glm::vec4(0.9f, 0.4f, 0.3f, 1.0f));

		static void DrawArrow(const glm::vec3& pos, const float& angle, const glm::vec4& color = glm::vec4(0.9f, 0.4f, 0.3f, 1.0f), const float& length = 1, const float& tipSize = 0.25f, const float& width = 0.5f);

		static void DrawArrow(const glm::vec3& pos, const glm::vec2& direction, const glm::vec4& color = glm::vec4(0.9f, 0.4f, 0.3f, 1.0f), const float& length = 1, const float& tipSize = 0.25f, const float& width = 0.5f);

		static void DrawArrow(const glm::vec3& pos, const glm::vec3& direction, const glm::vec4& color = glm::vec4(0.9f, 0.4f, 0.3f, 1.0f), const float& length = 1, const float& tipSize = 0.25f, const float& width = 0.5f);

		void DrawCone(const glm::vec3& pos, const glm::quat& rotation, float length, float radius, const glm::vec4& color = glm::vec4(0.9f, 0.4f, 0.3f, 1.0f));

		const uint32_t MaxLines = 10000;

		std::unique_ptr<RenderContext> mContext;

	private:
		static std::vector<Line> mLines;
		static std::vector<glm::vec4> mLinesColors;
		static std::map<uint32_t, BillboardData> mBillboardDrawCalls;
		static std::map<uint32_t, BillboardData> mEntitybillboardscalls;

		std::shared_ptr<Buffer> mlineBufferSSBO;
	
		uint32_t LineVAO;
		static uint32_t QuadVAO;
		static uint32_t QuadVBO;
		static int g_numCircleVertices;

		static void RenderBillboardQuad(RenderContext* Context);
	};





}

#endif