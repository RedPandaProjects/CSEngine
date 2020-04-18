#include "precompiled.h"

LINK_ENTITY_TO_CLASS(weapon_m3, CM3, CCSM3)

void CM3::Spawn()
{
	Precache();

	m_iId = WEAPON_M3;
	SET_MODEL(edict(), "models/w_m3.mdl");

	m_iDefaultAmmo = M3_DEFAULT_GIVE;

	// Get ready to fall down
	FallInit();

	// extend
	CBasePlayerWeapon::Spawn();
}

void CM3::Precache()
{
	PRECACHE_MODEL("models/v_m3.mdl");
	PRECACHE_MODEL("models/w_m3.mdl");

	m_iShellId = m_iShell = PRECACHE_MODEL("models/shotgunshell.mdl");

	PRECACHE_SOUND("weapons/m3-1.wav");
	PRECACHE_SOUND("weapons/m3_insertshell.wav");
	PRECACHE_SOUND("weapons/m3_pump.wav");
	PRECACHE_SOUND("weapons/reload1.wav");
	PRECACHE_SOUND("weapons/reload3.wav");

	m_usFireM3 = PRECACHE_EVENT(1, "events/m3.sc");
}

int CM3::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "buckshot";
	p->iMaxAmmo1 = MAX_AMMO_BUCKSHOT;
	p->pszAmmo2 = nullptr;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = M3_MAX_CLIP;
	p->iSlot = 0;
	p->iPosition = 5;
	p->iId = m_iId = WEAPON_M3;
	p->iFlags = 0;
	p->iWeight = M3_WEIGHT;

	return 1;
}

BOOL CM3::Deploy()
{
	return DefaultDeploy("models/v_m3.mdl", "models/p_m3.mdl", M3_DRAW, "shotgun", UseDecrement() != FALSE);
}

void CM3::PrimaryAttack()
{
	Vector vecAiming, vecSrc, vecDir;
	int flag;

	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound();
		m_flNextPrimaryAttack = GetNextAttackDelay(0.15);
		return;
	}

	if (m_iClip <= 0)
	{
		Reload();

		if (!m_iClip)
		{
			PlayEmptySound();
		}

		if (TheBots)
		{
			TheBots->OnEvent(EVENT_WEAPON_FIRED_ON_EMPTY, m_pPlayer);
		}

		m_flNextPrimaryAttack = GetNextAttackDelay(1);
		return;
	}

	m_iClip--;
	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

	m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
	// player "shoot" animation
	m_pPlayer->SetAnimation(PLAYER_ATTACK1);

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);

	vecSrc = m_pPlayer->GetGunPosition();
	vecAiming = gpGlobals->v_forward;

	m_pPlayer->FireBullets(9, vecSrc, vecAiming, M3_CONE_VECTOR, 3000, BULLET_PLAYER_BUCKSHOT, 0);

#ifdef CLIENT_WEAPONS
	flag = FEV_NOTHOST;
#else
	flag = 0;
#endif

	PLAYBACK_EVENT_FULL(flag, m_pPlayer->edict(), m_usFireM3, 0, (float *)&g_vecZero, (float *)&g_vecZero, 0, 0, 0, 0, FALSE, FALSE);

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
	{
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", SUIT_SENTENCE, SUIT_REPEAT_OK);
	}

#ifndef REGAMEDLL_FIXES
	if (m_iClip != 0)
		m_flPumpTime = UTIL_WeaponTimeBase() + 0.5f;
#endif

	m_flNextPrimaryAttack = GetNextAttackDelay(0.875);
	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.875f;

	if (m_iClip != 0)
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.5f;
	else
		m_flTimeWeaponIdle = 0.875f;

	m_fInSpecialReload = 0;

	if (m_pPlayer->pev->flags & FL_ONGROUND)
		m_pPlayer->pev->punchangle.x -= UTIL_SharedRandomLong(m_pPlayer->random_seed + 1, 4, 6);
	else
		m_pPlayer->pev->punchangle.x -= UTIL_SharedRandomLong(m_pPlayer->random_seed + 1, 8, 11);

	m_pPlayer->m_flEjectBrass = gpGlobals->time + 0.45f;
}

void CM3::Reload()
{
	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == iMaxClip())
		return;

	// don't reload until recoil is done
	if (m_flNextPrimaryAttack > UTIL_WeaponTimeBase())
		return;

	// check to see if we're ready to reload
	if (m_fInSpecialReload == 0)
	{
		m_pPlayer->SetAnimation(PLAYER_RELOAD);
		SendWeaponAnim(M3_START_RELOAD, UseDecrement() != FALSE);

		m_fInSpecialReload = 1;
		m_flNextSecondaryAttack = m_flTimeWeaponIdle = m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.55f;
		m_flNextPrimaryAttack = GetNextAttackDelay(0.55);
	}
	else if (m_fInSpecialReload == 1)
	{
		if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
			return;

		// was waiting for gun to move to side
		m_fInSpecialReload = 2;
		SendWeaponAnim(M3_RELOAD, UseDecrement());

		m_flTimeWeaponIdle = m_flNextReload = UTIL_WeaponTimeBase() + 0.45f;
	}
	else
	{
		m_iClip++;

#ifdef REGAMEDLL_ADD
		if (refill_bpammo_weapons.value < 3.0f)
#endif
		{
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
			m_pPlayer->ammo_buckshot--;
		}

		m_fInSpecialReload = 1;
	}
}

void CM3::WeaponIdle()
{
	ResetEmptySound();
	m_pPlayer->GetAutoaimVector(AUTOAIM_5DEGREES);

#ifndef REGAMEDLL_FIXES
	if (m_flPumpTime && m_flPumpTime < UTIL_WeaponTimeBase())
	{
		m_flPumpTime = 0;
	}
#endif

	if (m_flTimeWeaponIdle < UTIL_WeaponTimeBase())
	{
		if (m_iClip == 0 && m_fInSpecialReload == 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
		{
			Reload();
		}
		else if (m_fInSpecialReload != 0)
		{
			if (m_iClip != iMaxClip() && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
			{
				Reload();
			}
			else
			{
				// reload debounce has timed out
				SendWeaponAnim(M3_PUMP, UseDecrement() != FALSE);

				m_fInSpecialReload = 0;
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5f;
			}
		}
		else
		{
			SendWeaponAnim(M3_IDLE, UseDecrement() != FALSE);
		}
	}
}
