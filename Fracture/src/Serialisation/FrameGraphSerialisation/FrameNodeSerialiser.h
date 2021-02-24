#pragma once
#ifndef FRAMENODESERIALISER_H
#define FRAMENODESERIALISER_H

#include <memory>
#include "nlohmann/json.hpp"

namespace Fracture
{
	using json = nlohmann::json;

	class FrameNode;
	class ClearFrame;
	class AdditiveMixNode;
	class GaussianBlurNode;
	class DepthNode;
	class IntermediateNode;
	class LambertianNode;
	class MixNode;
	class MultiplyMixNode;
	class OutlineNode;
	class SSAONode;
	class ThresholdNode;
	class ToneMappingNode;
	class Link;
	class SinkNode;
	class SourceNode;

	class FrameNodeSerialiser
	{
	public:
		FrameNodeSerialiser(nlohmann::json node):m_json(node)
		{};
		~FrameNodeSerialiser() = default;

		json j()const { return m_json; }

		virtual void visitClearFrameNode(const std::shared_ptr<ClearFrame>& node)const {};
		virtual void visitAdditiveMixNode(const std::shared_ptr<AdditiveMixNode>& node)const {};
		virtual void visitGaussianBlurNode(const std::shared_ptr<GaussianBlurNode>& node)const {};
		virtual void visitDepthNode(const std::shared_ptr<DepthNode>& node)const {};
		virtual void visitIntermediateNode(const std::shared_ptr<IntermediateNode>& node)const {};
		virtual void visitLambertianNode(const std::shared_ptr<LambertianNode>& node)const {};
		virtual void visitMixNode(const std::shared_ptr<MixNode>& node)const {};
		virtual void visitMultiplyMixNode(const std::shared_ptr<MultiplyMixNode>& node)const {};
		virtual void visitOutlineNode(const std::shared_ptr<OutlineNode>& node)const {};
		virtual void visitSSAONode(const std::shared_ptr<SSAONode>& node)const {};
		virtual void visitThresholdNode(const std::shared_ptr<ThresholdNode>& node)const {};
		virtual void visitToneMappingNode(const std::shared_ptr<ToneMappingNode>& node)const {};
		virtual void visitSinkNode(const std::shared_ptr<SinkNode>& node)const {};
		virtual void visitSourceNode(const std::shared_ptr<SourceNode>& node)const {};
		virtual void visitLink(const std::shared_ptr<Link>& link)const {};
	private:
		json m_json;


	};

}

#endif