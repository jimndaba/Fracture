#pragma once
#ifndef COLLISIONFILTER_H
#define COLLISIONFILTER_H

namespace Fracture
{

	enum class CollisionGroup
	{
		COLGROUP_NONE = 0, 
		COLGROUP_Bullet = 1 << 0, 
		COLGROUP_Player = 1 << 1, 
		COLGROUP_Enemy = 1 << 2
	};

	enum class CollisionMask
	{
		None = (int)CollisionGroup::COLGROUP_NONE,
		Everything = (int)CollisionGroup::COLGROUP_NONE|
		(int)CollisionGroup::COLGROUP_Bullet|
		(int)CollisionGroup::COLGROUP_Enemy|
		(int)CollisionGroup::COLGROUP_Player,
		BulletMask = (int)CollisionGroup::COLGROUP_Enemy,	
		PlayerMask = (int)CollisionGroup::COLGROUP_Enemy,
		EnemyMask = (int)CollisionGroup::COLGROUP_Player|(int)CollisionGroup::COLGROUP_Bullet,
	};

}

#endif
