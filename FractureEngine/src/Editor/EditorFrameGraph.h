#pragma once
#ifndef EDITORFG_H
#define EDITORFG_H

#include "Fracture.h"

namespace Fracture
{

	class EditorFrameGraph : public FrameGraph
	{
	public:
		EditorFrameGraph(Fracture::Renderer& renderer):FrameGraph(renderer)
		{
			
			{
				auto depthbuffer = std::make_shared<DepthNode>("global_depthbuffer", renderer.Width(), renderer.Height(), renderer.m_Bucket.get());
				addnode(depthbuffer);
			}

			{
				auto clear = std::make_shared<ClearFrame>("clearframe");
				addnode(clear);
			}


			{
				auto lambertian = std::make_shared<LambertianNode>("lamertianPass", renderer.Width(), renderer.Height(), renderer.m_Bucket.get());
				addnode(lambertian);
			}

			{
				BrightPass = std::make_shared<ThresholdNode>("thresholdPass", renderer.Width(), renderer.Height());
				addnode(BrightPass);
			}

			{
				ToneMap = std::make_shared<ToneMappingNode>("ToneMapPass", renderer.Width(), renderer.Height());
				addnode(ToneMap);
			}

			{
				auto blurPass = std::make_shared<BoxBlurNode>("BoxBlurPass", renderer.Width(), renderer.Height());
				addnode(blurPass);
			}

			{
				auto mixColor = std::make_shared<AdditiveMixNode>("mixPass", renderer.Width(), renderer.Height());
				addnode(mixColor);
			}


			{
				ssao = std::make_shared<SSAONode>("ssaoPass", renderer.Width(), renderer.Height());
				addnode(ssao);
			}

			{
				ssaoblur = std::make_shared<BoxBlurNode>("ssaoBlur", renderer.Width(), renderer.Height());
				addnode(ssaoblur);
			}


			{
				//auto intermediate = std::make_shared<IntermediateNode>("intermediatePass", renderer.Width(), renderer.Height());
				//addnode(intermediate);
			}



			addLink("global_output", "rendertarget", "mixPass", "output");// "mixPass", "output");



			addLink("mixPass", "colorA", "lamertianPass", "outputColor");
			addLink("mixPass", "colorB", "BoxBlurPass", "blurOutput");

			addLink("BoxBlurPass", "colorTexture", "thresholdPass", "thresholdMap");

			addLink("thresholdPass", "colorTexture", "ToneMapPass", "colorOut");

			addLink("ToneMapPass", "buffer", "lamertianPass", "outputColor");

			//addLink("intermediatePass", "inputbuffer", "lamertianPass",  "outputColor");

			addLink("lamertianPass", "buffer", "clearframe", "buffer");
			addLink("lamertianPass", "SSAOMap", "ssaoBlur", "blurOutput");



			addLink("clearframe", "buffer", "global_backbuffer", "rendertarget");


			addLink("ssaoBlur", "colorTexture", "ssaoPass", "SSAOOutput");

			addLink("ssaoPass", "DepthTexture", "global_depthbuffer", "outputDepthMap");
		}

		std::shared_ptr<ToneMappingNode> ToneMap;
		std::shared_ptr<ThresholdNode> BrightPass;
		std::shared_ptr<SSAONode> ssao;
		std::shared_ptr<BoxBlurNode> ssaoblur;

	};


}
#endif 