#pragma once
#ifndef PHYSICSDEBUG_H
#define PHYSICSDEBUG_H
#include "Bullet/include/btBulletDynamicsCommon.h"

namespace Fracture
{

	class PhysicsDebugDraw : public btIDebugDraw
	{
	public:
		PhysicsDebugDraw();
		~PhysicsDebugDraw();

		virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		virtual void reportErrorWarning(const char* warningString);
		virtual void draw3dText(const btVector3& location, const char* textString);
		virtual void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&);
		virtual void setDebugMode(int debugMode);
		virtual int getDebugMode() const;
		virtual void clearLines();		
		virtual void flushLines();

	private:
		DebugDrawModes m_debug;
	};
}


#endif