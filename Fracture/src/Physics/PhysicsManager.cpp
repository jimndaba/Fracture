#include "PhysicsManager.h"
#include "Component/RigidBodyComponent.h"
#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Profiling/Profiler.h"
#include <iostream>

btDefaultCollisionConfiguration* Fracture::PhysicsManager::collisionConfiguration;
btCollisionDispatcher* Fracture::PhysicsManager::dispatcher;
btBroadphaseInterface* Fracture::PhysicsManager::overlappingPairCache;
btSequentialImpulseConstraintSolver* Fracture::PhysicsManager::solver;
btDiscreteDynamicsWorld* Fracture::PhysicsManager::dynamicsWorld;
btAlignedObjectArray<btCollisionShape*> Fracture::PhysicsManager::collisionShapes;

Fracture::PhysicsManager::PhysicsManager()
{

	
}

Fracture::PhysicsManager::~PhysicsManager()
{
	collisionShapes.clear();

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	delete collisionConfiguration;
	delete dispatcher;
	delete overlappingPairCache;
	delete solver;
	delete dynamicsWorld;
}

void Fracture::PhysicsManager::Init()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	///-----initialization_end----
}

void Fracture::PhysicsManager::AddCollider(btCollisionShape* collider)
{
	collisionShapes.push_back(collider);
}

void Fracture::PhysicsManager::RemoveCollider(btCollisionShape* collider)
{
	collisionShapes.remove(collider);
}

void Fracture::PhysicsManager::AddRigidBody(btRigidBody* body)
{
	dynamicsWorld->addRigidBody(body);
}

void Fracture::PhysicsManager::RemoveRigidBody(btRigidBody* body)
{
	dynamicsWorld->removeRigidBody(body);
}

void Fracture::PhysicsManager::onUpdate(float dt)
{
	ProfilerTimer timer("Physics Update");
	if (dynamicsWorld)
	{
		dynamicsWorld->stepSimulation(1.0f / 60.f);
	}

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--) {
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		btTransform trans;

		if (body && body->getMotionState()) {
			
			body->getMotionState()->getWorldTransform(trans);			
			
			void* userPointer = body->getUserPointer();
			if (userPointer) {
				RigidBodyComponent* component = static_cast<RigidBodyComponent*>(userPointer);			

				btQuaternion orientation = trans.getRotation();

				std::shared_ptr<TransformComponent> transcomponent = ComponentManager::GetComponent<TransformComponent>(component->EntityID);

				transcomponent->Position = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
				transcomponent->Rotation = glm::vec3(orientation.getX(), orientation.getY(), orientation.getZ());
				
			}
		}
		
	}
	
}
