#ifndef	WEAPON_MOLOTOV_H
#define	WEAPON_MOLOTOV_H

#include "CGrenade.h"
#include "CHLMachineGun.h"
#include "CCombatWeapon.h"
//#include "CGrenadeWeapon.h"

//CBaseHLCombatWeapon == CLASE SIMILAR ==> CCombatWeapon

class CWeaponMolotov : public CCombatWeapon
{
/*private:
	int					m_nNumAmmoTypes;
	bool				m_bNeedDraw;
	int					m_iThrowBits;				// Save the current throw bits state
	float				m_fNextThrowCheck;			// When to check throw ability next
	Vector				m_vecTossVelocity;*/
public:
	CE_DECLARE_CLASS(CWeaponMolotov, CCombatWeapon);
	CWeaponMolotov() 
	{
		printf("CWeaponMolotov()__\n");
	}
	//DECLARE_SERVERCLASS();
	void				Precache(void);
	//virtual	void	Spawn();
	void				Spawn(void);
	//virtual	void	ItemPostFrame();
	void				ItemPostFrame();
	void				PrimaryAttack(CPlayer* jugador);
	//virtual void Touch(CEntity* pOther);
	//void		Touch(CEntity* pOther);
	DECLARE_ACTTABLE();
	DECLARE_DATADESC();

	DECLARE_PREDICTABLE();
};

#endif	//WEAPON_MOLOTOV_H
