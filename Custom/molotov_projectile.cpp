
#include "molotov_projectile.h"
#include "CGrenade.h"
#include "CSprite.h"
#include "CSpriteTrail.h"
#include "player_pickup.h"
#include "CCombatCharacter.h"
#include "CPlayer.h"
//#include "fire.cpp"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define MOLOTOV_EXPLOSION_VOLUME	1024

BEGIN_DATADESC(CGrenade_Molotov)

	//DEFINE_FIELD(m_pFireTrail, FIELD_CLASSPTR),

	// Function Pointers
	DEFINE_FUNCTION(MolotovTouch),
	//DEFINE_FUNCTION(MolotovThink),

END_DATADESC()

LINK_ENTITY_TO_CUSTOM_CLASS(grenade_molotov, smokegrenade_projectile, CGrenade_Molotov);


void CGrenade_Molotov::Precache(void)
{
	BaseClass::Precache();
	PrecacheModel("models/props_junk/garbage_glassbottle003a.mdl");
	g_helpfunc.UTIL_PrecacheOther("_firesmoke");

	//PrecacheScriptSound("Grenade_Molotov.Detonate");
}

void CGrenade_Molotov::Spawn(void)
{
	Precache();

	this->SetModel("models/props_junk/garbage_glassbottle003a.mdl");

	UTIL_SetSize(BaseEntity(), -Vector(4, 4, 4), Vector(4, 4, 4));
	SetCollisionGroup(COLLISION_GROUP_WEAPON);
	CreateVPhysics();

	AddSolidFlags(FSOLID_NOT_STANDABLE);

	SetTouch(&CGrenade_Molotov::MolotovTouch);

	BaseClass::Spawn();
}

void CGrenade_Molotov::MolotovTouch(CEntity* pOther) {
	/*if(pOther != this->GetOwnerEntity())
		UTIL_Remove(this);*/
	/*Vector vector = this->GetAbsOrigin();

	printf("<POSICION>\t | x=%f | y= %f | z=%f |\n",
		vector.x,
		vector.y,
		vector.z);

	if (vector.z == 0.0) {
		printf("CGrenade_Molotov::MolotovTouch(%s)\n", pOther->GetClassname());
		Detonate();
	}*/
}

// this will hit only things that are in newCollisionGroup, but NOT in collisionGroupAlreadyChecked
class CTraceFilterCollisionGroupDelta : public CTraceFilterEntitiesOnly
{
public:
	// It does have a base, but we'll never network anything below here..
	DECLARE_CLASS_NOBASE(CTraceFilterCollisionGroupDelta);

	CTraceFilterCollisionGroupDelta(const IHandleEntity* passentity, int collisionGroupAlreadyChecked, int newCollisionGroup)
		: m_pPassEnt(passentity), m_collisionGroupAlreadyChecked(collisionGroupAlreadyChecked), m_newCollisionGroup(newCollisionGroup)
	{
	}

	virtual bool ShouldHitEntity(IHandleEntity* pHandleEntity, int contentsMask)
	{
		if (!PassServerEntityFilter(pHandleEntity, m_pPassEnt))
			return false;
		CEntity* pEntity = CE_EntityFromEntityHandle(pHandleEntity);

		if (pEntity)
		{
			if (g_helpfunc.GameRules_ShouldCollide(m_collisionGroupAlreadyChecked, pEntity->GetCollisionGroup()))
				return false;
			if (g_helpfunc.GameRules_ShouldCollide(m_newCollisionGroup, pEntity->GetCollisionGroup()))
				return true;
		}

		return false;
	}

protected:
	const IHandleEntity* m_pPassEnt;
	int		m_collisionGroupAlreadyChecked;
	int		m_newCollisionGroup;
};

void CGrenade_Molotov::VPhysicsUpdate(IPhysicsObject* pPhysics)
{
	BaseClass::VPhysicsUpdate(pPhysics);
	Vector vel;
	AngularImpulse angVel;
	pPhysics->GetVelocity(&vel, &angVel);

	Vector start = GetAbsOrigin();
	// find all entities that my collision group wouldn't hit, but COLLISION_GROUP_NONE would and bounce off of them as a ray cast
	CTraceFilterCollisionGroupDelta filter(BaseEntity(), GetCollisionGroup(), COLLISION_GROUP_NONE);
	trace_t tr;

	// UNDONE: Hull won't work with hitboxes - hits outer hull.  But the whole point of this test is to hit hitboxes.
#if 0
	UTIL_TraceHull(start, start + vel * gpGlobals->frametime, CollisionProp()->OBBMins(), CollisionProp()->OBBMaxs(), CONTENTS_HITBOX | CONTENTS_MONSTER | CONTENTS_SOLID, &filter, &tr);
#else
	UTIL_TraceLine(start, start + vel * gpGlobals->frametime, CONTENTS_HITBOX | CONTENTS_MONSTER | CONTENTS_SOLID, &filter, &tr);
#endif
	if (tr.startsolid)
	{
		if (!m_inSolid)
		{
			// UNDONE: Do a better contact solution that uses relative velocity?
			vel *= -0.2f; // bounce backwards
			pPhysics->SetVelocity(&vel, NULL);
		}
		m_inSolid = true;
		return;
	}
	m_inSolid = false;
	if (tr.DidHit())
	{
		Vector dir = vel;
		VectorNormalize(dir);
		// send a tiny amount of damage so the character will react to getting bonked
		CBaseEntity* pBaseThrower = GetThrower() ? GetThrower()->BaseEntity() : NULL;
		CTakeDamageInfo info(BaseEntity(), pBaseThrower, pPhysics->GetMass() * vel, GetAbsOrigin(), 0.1f, DMG_CRUSH);
		CEntity::Instance(tr.m_pEnt)->TakeDamage(info);

		// reflect velocity around normal
		vel = -2.0f * tr.plane.normal * DotProduct(vel, tr.plane.normal) + vel;

		// absorb 80% in impact
		vel *= 1.0f;
		angVel *= -1.0f;
		pPhysics->SetVelocity(&vel, &angVel);
	}
}