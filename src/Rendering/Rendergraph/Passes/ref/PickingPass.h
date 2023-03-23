#pragma once
#ifndef PICKINGPASS
#define PICKINGPASS


#include "../RenderQueuenode.h"

namespace Fracture
{
	class Material;
	class RenderBucket;
	struct DrawCommand;
	class RenderTarget;
	class ICamera;

	struct PixelInfo {
		float ObjectID;
		float DrawID;
		float PrimID;

		PixelInfo() {
			ObjectID = 0.0f;
			DrawID = 0.0f;
			PrimID = 0.0f;
		}
	};

	class PickingPass:public RenderQueueNode
	{
	public:
		PickingPass(const std::string& Name, const Renderer& renderer);
		~PickingPass();

		void execute(Renderer& renderer) override;
		//void Reset() override;
		void Resize(int screenWidth, int screenHeight);

		uint32_t GetPixelInfo(unsigned int x, unsigned int y);
		static std::shared_ptr<RenderTarget> m_renderTarget;

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);
		

	private:

		int SCREEN_WIDTH, SCREEN_HEIGHT;
		std::shared_ptr<PixelInfo> m_pixelInfo;
		std::shared_ptr<Shader> m_shader;
	};

}

#endif
