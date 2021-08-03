#pragma once

#ifndef	GRENADEMOLOTOV_H
#define	GRENADEMOLOTOV_H

#include "CGrenade.h"

class CGrenade_Molotov : public CE_Grenade
{
public:
	DECLARE_CLASS(CGrenade_Molotov, CE_Grenade);
	DECLARE_DATADESC();

	virtual void	Spawn(void);
	virtual void	Precache(void);
	//virtual void	Detonate(void);
	void			MolotovTouch(CEntity* pOther);
	void			VPhysicsUpdate(IPhysicsObject* pPhysics);
	//void			MolotovThink(void);

	//SmokeTrail* m_pFireTrail;

protected:
	bool	m_inSolid;
};

#endif	//GRENADEMOLOTOV_H