#pragma once
#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H


namespace Fracture
{

	struct SceneNode
	{
		SceneNode(const UUID& id);
		std::vector<std::shared_ptr<SceneNode>> Children;

		UUID entity;
		SceneNode* Parent;

		void AddNode(const UUID& id);
		void RemoveNode(const std::shared_ptr<SceneNode>& node);
	};

}



#endif