#include "FracturePCH.h"
#include "Scenegraph.h"

Fracture::SceneNode::SceneNode(const UUID& id):entity(id)
{
}

void Fracture::SceneNode::AddNode(const UUID& id)
{
	auto node = std::make_shared<SceneNode>(id);
	node->Parent = this;
	Children.push_back(node);
}

void Fracture::SceneNode::RemoveNode(const 	std::shared_ptr<SceneNode>& node)
{
	//TODO Find Entity and Components and Remove From Scene
	Children.erase(std::remove(Children.begin(), Children.end(), node), Children.end());
}
