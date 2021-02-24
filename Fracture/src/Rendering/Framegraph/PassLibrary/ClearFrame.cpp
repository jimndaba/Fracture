#include "ClearFrame.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"

Fracture::ClearFrame::ClearFrame(const std::string& Name):FrameNode(Name)
{
	std::shared_ptr<InputSocket> m_Input =std::make_shared<InputSocket>("buffer");
	std::shared_ptr<OutputSocket> m_output =std::make_shared<OutputSocket>("buffer");
	
	//Sockets
	AddInputSocket(m_Input);
	AddOutputSocket(m_output);

	//Link Sockets to Resources
	AddInputResource(m_Input, resource);
	AddOutputResource(m_output, resource);
}

void Fracture::ClearFrame::execute(Renderer& renderer)
{
	ProfilerTimer timer("Clear Frame");
	for (auto& res : resources)
	{
		res.second->bind();
		renderer.clearColor(Color.x, Color.y, Color.z);
		renderer.clear();
		res.second->Unbind();
	}
}
