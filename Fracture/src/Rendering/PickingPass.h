#pragma once
#ifndef PICKINGPASS
#define PICKINGPASS

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

	class PickingPass
	{
	public:
		PickingPass(int screenWidth, int screenHeight);
		~PickingPass();

		void Begin();
		void End();
		void Render(std::shared_ptr<ICamera> camera, std::shared_ptr<Material> material, RenderBucket& bucket);

		void Resize(int screenWidth, int screenHeight);

		unsigned int GetPixelInfo(unsigned int x, unsigned int y);
		std::shared_ptr<RenderTarget> m_renderTarget;
	private:
		
		int SCREEN_WIDTH, SCREEN_HEIGHT;
		std::shared_ptr<PixelInfo> m_pixelInfo;
	};

}

#endif
