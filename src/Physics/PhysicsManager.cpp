#include "FracturePCH.h"
#include "PhysicsManager.h"
#include "EventSystem/Eventbus.h"
#include "PhysicsEvents.h"
#include "World/SceneManager.h"
#include "World/Components.h"

Fracture::PhysicsManager::PhysicsManager(const PhysicsConfiguration& config):Config(config)
{
}

void Fracture::PhysicsManager::OnInit()
{
	_broadphase = new btDbvtBroadphase();
	_collisionConfig = new btDefaultCollisionConfiguration();
	_collisionDispatcher = new btCollisionDispatcher(_collisionConfig);
	_solver = new btSequentialImpulseConstraintSolver();
	_world = new btDiscreteDynamicsWorld(_collisionDispatcher,_broadphase,_solver,_collisionConfig);
	_world->setGravity(btVector3(0, -10, 0));
	
	FRACTURE_INFO("Physics Initialised");
}

void Fracture::PhysicsManager::OnShutdown()
{
	mRigidBodies.clear();
	mCollisionShapes.clear();

	delete _world;
	delete _solver;
	delete _collisionConfig;
	delete _collisionDispatcher;
	delete _broadphase;
}

void Fracture::PhysicsManager::OnClearPhysicsWorld()
{
	for(const auto& rigid : mRigidBodies)
	{
		_world->removeRigidBody(rigid.second.get());
	}

	mRigidBodies.clear();
	mCollisionShapes.clear();
}

void Fracture::PhysicsManager::OnPhysicsUpdate(float dt)
{
	_world->stepSimulation(dt,10);


	// Feedback physics engine state to system:
	for (int i = 0; i < _world->getCollisionObjectArray().size(); ++i)
	{
		btCollisionObject* collisionobject = _world->getCollisionObjectArray()[i];
		auto rigidbody = (RigidbodyComponent*)collisionobject->getUserPointer();
		btRigidBody* mbody = btRigidBody::upcast(collisionobject);

		if (mbody != nullptr)
		{
			if (!rigidbody->IsDynamic)
			{
				continue;
			}

			const auto& transform = SceneManager::GetComponent<TransformComponent>(rigidbody->GetID());
			btTransform trans = mbody->getWorldTransform();
			transform->Position = { trans.getOrigin().getX(),trans.getOrigin().getY() ,trans.getOrigin().getZ() };
			transform->Rotation = { trans.getRotation().getX(),trans.getRotation().getY() ,trans.getRotation().getZ() ,trans.getRotation().getW() };
			transform->IsDirty = true;
		}
	}

	/*
	for (const auto& body : mRigidBodies)
	{
		btTransform trans;
		if (body.second && body.second->getMotionState())
		{
			body.second->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = body.second->getWorldTransform();
		}

	
		const auto& entity = (RigidbodyComponent*)body.second->getUserPointer();
		if (SceneManager::HasComponent<RigidbodyComponent>(entity->GetID()))
		{
			const auto& rigidbody = SceneManager::GetComponent<RigidbodyComponent>(entity->GetID());
			if (!rigidbody->IsDynamic)
			{
				continue;
			}

			const auto& transform = SceneManager::GetComponent<TransformComponent>(entity->GetID());
			transform->Position = { trans.getOrigin().getX(),trans.getOrigin().getY() ,trans.getOrigin().getZ() };
			transform->Rotation = { trans.getRotation().getX(),trans.getRotation().getY() ,trans.getRotation().getZ() ,trans.getRotation().getW() };
			transform->IsDirty = true;
		}
		
	}
	*/
}

void Fracture::PhysicsManager::OnCollisionUpdate()
{
	int numManifolds = _world->getDispatcher()->getNumManifolds();
	for (int i = 0; i < numManifolds; i++)
	{
		const auto& contactManifold = _world->getDispatcher()->getManifoldByIndexInternal(i);
		
		int numContacts = contactManifold->getNumContacts();
		if (numContacts > 0)
		{
			//5
			const auto& obA = contactManifold->getBody0();
			const auto& obB = contactManifold->getBody1();

			//6
			const auto& pnA = (Fracture::UUID)obA->getUserPointer();
			CollisionContext a;
			a.entity = pnA;
			
			const auto& pnB = (Fracture::UUID)obB->getUserPointer();
			CollisionContext b;
			b.entity = pnB;

			Eventbus::Publish<OnCollisionEvent>(a,b);

		}

	}
}

void Fracture::PhysicsManager::SetGravity(const glm::vec3& g)
{
	_world->setGravity(btVector3(g.x, g.y, g.z));
}

void Fracture::PhysicsManager::SetMovementConstraints(const Fracture::UUID& entity,const bool c[3])
{
	if (mRigidBodies.find(entity) != mRigidBodies.end() && mRigidBodies[entity])
		mRigidBodies[entity]->setLinearFactor(btVector3(c[0], c[1], c[2]));

}

void Fracture::PhysicsManager::SetRotationConstraints(const Fracture::UUID& entity, const bool c[3])
{
	if(mRigidBodies.find(entity) != mRigidBodies.end() && mRigidBodies[entity])
		mRigidBodies[entity]->setAngularFactor(btVector3(c[0], c[1], c[2]));
}

void Fracture::PhysicsManager::AddRigidBodyToWorld(Fracture::UUID entity)
{
	if (!SceneManager::HasComponent<RigidbodyComponent>(entity))
		return;

	if (mCollisionShapes.find(entity) == mCollisionShapes.end())
	{
		FRACTURE_ERROR("No Collider Assigned to entity: {}", entity);
		FRACTURE_ERROR("Attach COllider before Rigidbody");
		return;
	}

	const auto& transform = SceneManager::GetComponent<TransformComponent>(entity);
	const auto& rigidbody = SceneManager::GetComponent<RigidbodyComponent>(entity);
	
	btQuaternion rotation = {transform->Rotation.x,transform->Rotation.y,transform->Rotation.z ,transform->Rotation.w};
	btVector3 position = btVector3(transform->Position.x, transform->Position.y, transform->Position.z);

	btScalar bodyMass = rigidbody->Mass;
	btVector3 bodyInertia(0, 0, 0);
	
	if(rigidbody->IsDynamic)
		mCollisionShapes[entity]->calculateLocalInertia(bodyMass, bodyInertia);


	btVector3 S(transform->Scale.x, transform->Scale.y, transform->Scale.z);
	mCollisionShapes[entity]->setLocalScaling(S);
	
	btTransform mbtTransform;
	mbtTransform.setIdentity();
	mbtTransform.setOrigin(position);
	mbtTransform.setRotation(rotation);
	rigidbody->motionState = new btDefaultMotionState(mbtTransform);

	btRigidBody::btRigidBodyConstructionInfo bodyCI = btRigidBody::btRigidBodyConstructionInfo(bodyMass, rigidbody->motionState, mCollisionShapes[entity].get(), bodyInertia);
	bodyCI.m_restitution = rigidbody->Bouncyness;
	bodyCI.m_friction = rigidbody->Friction;
	mRigidBodies[entity] = std::make_unique<btRigidBody>(bodyCI);
	mRigidBodies[entity]->setLinearFactor(btVector3(rigidbody->LinearConstraints[0], rigidbody->LinearConstraints[1], rigidbody->LinearConstraints[2]));
	mRigidBodies[entity]->setAngularFactor(btVector3(rigidbody->AngularConstraints[0], rigidbody->AngularConstraints[1], rigidbody->AngularConstraints[2]));
	mRigidBodies[entity]->setUserPointer((void*)rigidbody.get());
	mRigidBodies[entity]->setWorldTransform(btTransform(rotation, position));
	mRigidBodies[entity]->forceActivationState(DISABLE_DEACTIVATION);
	mRigidBodies[entity]->setCollisionFlags(btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_STATIC_OBJECT);

	_world->addRigidBody(mRigidBodies[entity].get());	
	FRACTURE_INFO("Added RigidBody {}", entity);
}

void Fracture::PhysicsManager::AttachCollisionShape(const Fracture::UUID& entity)
{
	if (!SceneManager::HasComponent<ColliderComponent>(entity))
		return;

	if (mCollisionShapes[entity])
	{
		return;
	}

	const auto& colliderComp = SceneManager::GetComponent<ColliderComponent>(entity);

	switch (colliderComp->Shape)
	{
		case ColliderType::Sphere:
		{
			mCollisionShapes[entity] = std::make_unique<btSphereShape>(colliderComp->Radius);
			break;
		}
		case ColliderType::Box:
		{
			mCollisionShapes[entity] = std::make_unique<btBoxShape>(btVector3(colliderComp->Size.x / 2, colliderComp->Size.y / 2, colliderComp->Size.z / 2));
			break;
		}
		case ColliderType::Capsule:
		{
			mCollisionShapes[entity] = std::make_unique<btCapsuleShape>(colliderComp->Radius, colliderComp->Height);
			break;
		}
		case ColliderType::Cone:
		{
			mCollisionShapes[entity] = std::make_unique<btConeShape>(colliderComp->Radius, colliderComp->Height);
			break;
		}
		case ColliderType::Cylinder:
		{
			mCollisionShapes[entity] = std::make_unique<btCylinderShape>(btVector3(colliderComp->Size.x / 2, colliderComp->Size.y / 2, colliderComp->Size.z / 2));
			break;
		}
	}
}

void Fracture::PhysicsManager::RemoveCollider(const Fracture::UUID& entity)
{

}

void Fracture::PhysicsManager::RemoveRigidBody(const Fracture::UUID& entity)
{
	if (mRigidBodies.find(entity) != mRigidBodies.end())
	{
		_world->removeRigidBody(mRigidBodies[entity].get());
		mRigidBodies.erase(entity);
	}
}
