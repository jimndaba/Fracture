#pragma once
#ifndef PICKINGPASS
#define PICKINGPASS


#include "../RenderQueueNode.h"
#include <vector>
#include <memory>

namespace Fracture
{
	class Material;
	class RenderBucket;
	class DrawCommand;
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
		PickingPass(const std::string& Name, const int& width,const int& height, const std::shared_ptr<RenderBucket>& opaque);
		~PickingPass();

		void execute(Renderer& renderer) override;
		//void Reset() override;
		void Resize(int screenWidth, int screenHeight);

		unsigned int GetPixelInfo(unsigned int x, unsigned int y);
		static std::shared_ptr<RenderTarget> m_renderTarget;

		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor)
		{
			json j;
			return j;
		}

	private:

		int SCREEN_WIDTH, SCREEN_HEIGHT;
		std::shared_ptr<PixelInfo> m_pixelInfo;
	};

}

#endif
