#include "weapon_molotov.h"
#include "CEntity.h"
#include "CPlayer.h"
#include "CCombatWeapon.h"
#include "ammodef.h"
#include "tier0/memdbgon.h"
#include "molotov_projectile.h"

BEGIN_DATADESC(CWeaponMolotov)
/*
DEFINE_FIELD(m_nNumAmmoTypes, FIELD_INTEGER),
DEFINE_FIELD(m_bNeedDraw, FIELD_BOOLEAN),
DEFINE_FIELD(m_iThrowBits, FIELD_INTEGER),
DEFINE_FIELD(m_fNextThrowCheck, FIELD_TIME),
DEFINE_FIELD(m_vecTossVelocity, FIELD_VECTOR),
*/
END_DATADESC()

BEGIN_PREDICTION_DATA(CWeaponMolotov)
END_PREDICTION_DATA()

//CE_LINK_ENTITY_TO_CLASS(weapon_smokegrenade, CWeaponMolotov);

LINK_ENTITY_TO_CUSTOM_CLASS(weapon_molotov, weapon_smokegrenade ,CWeaponMolotov);

acttable_t	CWeaponMolotov::m_acttable[] =
{
	{ ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_THROW, true },
};
IMPLEMENT_ACTTABLE(CWeaponMolotov);

void CWeaponMolotov::Precache(void)
{
	BaseClass::Precache();
	PrecacheModel("models/props_junk/garbage_glassbottle003a.mdl");	//<<TEMP>> need real model
	printf("CWeaponMolotov::Precache(void)\n");
}

void CWeaponMolotov::Spawn(void)
{
	printf("CWeaponMolotov::Spawn(void)\n");
	// Call base class first
	BaseClass::Spawn();

	//m_bNeedDraw = true;
	//			(	this, ...									  );
	UTIL_SetSize(BaseEntity(), Vector(-6, -6, -2), Vector(6, 6, 2));
}

void CWeaponMolotov::ItemPostFrame() {
	
	CPlayer* jugador = ToBasePlayer(GetOwnerEntity());
	if (jugador->m_afButtonPressed & IN_ATTACK)
	{
		PrimaryAttack(jugador);
	}
	else {
		jugador->m_nButtons &= ~IN_ATTACK;
	}
	//remplazo el proyectil no quiero la smoke
	BaseClass::ItemPostFrame();
}
void CWeaponMolotov::PrimaryAttack(CPlayer* jugador) {
	//proyectil va aca->

	Vector origen = GetAbsOrigin();
	origen.z += 70.0;

	jugador->EyeAngles();
	/*printf("<POSICION>\t | x=%f | y= %f | z=%f |\n<Ojos>\t | x=%f | y= %f | z=%f |\n", 
		origen.x, 
		origen.y, 
		origen.z,
		jugador->EyeAngles().x,
		jugador->EyeAngles().y,
		jugador->EyeAngles().z);*/

	CGrenade_Molotov* pMolotov = (CGrenade_Molotov*)CEntity::Create("grenade_molotov", origen, GetAbsAngles(), BaseEntity());
	Vector vecForward;
	AngleVectors(jugador->EyeAngles(), &vecForward);

	pMolotov->SetAbsVelocity(vecForward * 550);

	QAngle vecAngVelocity(enginerandom->RandomFloat(-100, -500), 0, 0);
	SetLocalAngularVelocity(vecAngVelocity);
	pMolotov->SetGravity(1.0);
	pMolotov->SetFriction(1.0);
	pMolotov->SetOwnerEntity(*jugador);
	
	jugador->Weapon_Drop(*this);
	UTIL_Remove(this);
	
}