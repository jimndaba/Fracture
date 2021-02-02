#include "PhysicsDebugDraw.h"
#include "Rendering/Renderer.h"

Fracture::PhysicsDebugDraw::PhysicsDebugDraw()
{
}

Fracture::PhysicsDebugDraw::~PhysicsDebugDraw()
{
}

void Fracture::PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	Renderer::DrawDebugLine(glm::vec3(from.getX(), from.getY(), from.getZ()), glm::vec3(to.getX(),to.getY(), to.getZ()),glm::vec4(0.9f,0.9f,0.9f,1.0f));

}

void Fracture::PhysicsDebugDraw::reportErrorWarning(const char* warningString)
{
}

void Fracture::PhysicsDebugDraw::draw3dText(const btVector3& location, const char* textString)
{
}

void Fracture::PhysicsDebugDraw::drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&)
{
}

void Fracture::PhysicsDebugDraw::setDebugMode(int debugMode)
{
	m_debug = (DebugDrawModes)debugMode;
}

int Fracture::PhysicsDebugDraw::getDebugMode() const
{
	return m_debug;
}

void Fracture::PhysicsDebugDraw::clearLines()
{
}

void Fracture::PhysicsDebugDraw::flushLines()
{
}
