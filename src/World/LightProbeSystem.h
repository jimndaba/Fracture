#pragma once
#ifndef LIGHTPROBESYSTEM_H
#define LIGHTPROBESYSTEM_H



namespace Fracture
{
	struct LightProbeComponent;
	struct RenderContext;

	struct LightProbeSystem
	{
		LightProbeSystem();
		void Bake(RenderContext* Context ,UUID component);
	};



}

#endif