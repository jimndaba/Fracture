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

	private:
		void DoIrradiance(RenderContext* Context, UUID component);
		void DoPrefitler(RenderContext* Context, UUID component);
		void DoBRDF(RenderContext* Context, UUID component);

	};



}

#endif