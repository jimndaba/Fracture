#pragma once
#ifndef COMMAND_H
#define COMMAND_H

namespace Fracture
{
	struct SortKey
	{
		uint16_t ViewportIndex = 0 ;
		uint16_t ScissorIndex = 0;
		uint16_t ShaderIndex = 0;
		uint16_t MaterialIndex = 0;
		uint16_t MeshIndex = 0;
		float Depth = 0.0f;

		bool operator < (const SortKey& r) const;
		bool operator > (const SortKey& r) const;
	};

	struct Command
	{
		SortKey Key;
		std::function<void()> fnc;
		std::string DEBUG;
	};




}

#endif