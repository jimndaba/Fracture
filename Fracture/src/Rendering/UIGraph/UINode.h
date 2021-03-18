#pragma once
#ifndef UINODE_H
#define UINODE_H

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "UISocket.h"
#include "UIMultiSocket.h"

namespace Fracture
{
	class Renderer;
	class UIResource;

	class UINode
	{
	public:
		UINode(const std::string& name);

		virtual void execute(Renderer& renderer) = 0;

		void AddInputSocket(UISocket input);
		void AddMultiInputSocket(UIMultiSocket input);
		void AddOutputSocket(UISocket output);

		void AddInputResource(UISocket input, std::shared_ptr<UIResource> resource);
		void AddMultiInputResource(UISocket input, std::shared_ptr<UIResource> resource);
		void AddOutputResource(UISocket output, std::shared_ptr<UIResource> resource);

		void AddResource(std::string name, std::shared_ptr<UIResource> res);
		std::shared_ptr<UIResource> getResource(std::string name);

		void LinkUIResource(std::string name, std::shared_ptr<UIResource> res);

		std::string GetName() const;	

		bool isVisited = false;
		std::map<std::string, std::shared_ptr<UIResource>> resources;

		template<class T, typename... Args>
		static std::shared_ptr<T> Make(Args&&... params);
	private:
		std::string m_name;
		std::vector<UISocket> m_inputs;
		std::vector<UISocket> m_outputs;
		std::vector<UIMultiSocket> m_MultiInputs;


	};


	template<class T, typename ...Args>
	inline std::shared_ptr<T> UINode::Make(Args && ...params)
	{
		return std::make_shared<T>(params...);
	}

}

#endif