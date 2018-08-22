#include "stdafx.h"
#include "CEntityManager.h"
#include "C6SMap.h"
#include "CGlobal.h"
#include "CBaseNPC.h"
#include "CWorldPhysics.h"

void CEntityManager::AddNPC(CBaseNPC* pEnt) {
	if (pEnt)
		m_NPCs.push_back(pEnt);
}
int CEntityManager::GetNextEntityID() {
	return (int)m_Weapons.size() + 1;
}
bool CEntityManager::EntityIDExists(int iEntID) {
	for (unsigned int i = 0; i < m_Weapons.size(); i++) {
		if (m_Weapons[i]->GetEntityID() == iEntID)
			return true;
	}
	return false;
}

void CEntityManager::AddWeapon(C6SBaseNetworkedWeapon* pWep) {
	m_Weapons.push_back(pWep); 
}

void CEntityManager::Update(float fElapsedTime, CGame* pGame, CWorldPhysics* pPhysics) {
	/* Weapon physics */
	for (unsigned int i = 0; i < m_Weapons.size(); i++) {
		m_Weapons[i]->UpdatePhysicsPosition(); 
	} 

	/* Spent casings physics */
	for (unsigned int i = 0; i < m_Casings.size(); i++) {
		m_Casings[i]->Update(fElapsedTime, pPhysics);
	}
	
	/* Remove expired casings */
	for (std::vector<CSpentCasingEffect*>::iterator it = m_Casings.begin(); it != m_Casings.end(); ) {
		if ((*it)->ShouldRemove()) {
			delete * it;
			it = m_Casings.erase(it);
		}
		else {
			it++;
		}
	}

	/* NPC */
	for (unsigned int i = 0; i < m_NPCs.size(); i++) {
		m_NPCs[i]->Update(pGame, fElapsedTime);
	}
	 
}

void CEntityManager::Draw(CGame* pGame) {
	for (unsigned int i = 0; i < m_NPCs.size(); i++) {
		m_NPCs[i]->Draw(pGame);
	}

	/* Draw weapons */
	for (unsigned int i = 0; i < m_Weapons.size(); i++) {
		m_Weapons[i]->DefaultDraw(pGame);
	}

	/* Draw casings */
	for (unsigned int i = 0; i < m_Casings.size(); i++) {
		m_Casings[i]->Draw(pGame);
	}
}

void CEntityManager::DestroyAllEntities() {
	/* Wipe weapons */
	for (unsigned int i = 0; i < m_Weapons.size(); i++) {
		if (m_Weapons[i])
			delete m_Weapons[i];
	}
	m_Weapons.clear();

	/* Wipe Casings */
	for (unsigned int i = 0; i < m_Casings.size(); i++) {
		if (m_Casings[i])
			delete m_Casings[i];
	}
	m_Casings.clear();
}

C6SBaseNetworkedWeapon* CEntityManager::ClosestWeapon(sf::Vector2i pos, float maxDist) {
	float smallestDist = maxDist + 1.0f;
	C6SBaseNetworkedWeapon* closestWep = 0;
	for (unsigned int i = 0; i < m_Weapons.size(); i++) {
		float dist = vec::Length(m_Weapons[i]->GetPosition2i() - pos);
		if (dist < maxDist && dist < smallestDist) {
			smallestDist = dist;
			closestWep = m_Weapons[i];
		}
	}
	return closestWep;
}
C6SBaseNetworkedWeapon* CEntityManager::ClosestWeapon(sf::Vector2f pos, float maxDist) {
	return ClosestWeapon(sf::Vector2i(pos), maxDist);
}
C6SBaseNetworkedWeapon* CEntityManager::GetWeaponByID(int iWeaponID) {
	for (unsigned int i = 0; i < m_Weapons.size(); i++) {
		if (m_Weapons[i]->GetEntityID() == iWeaponID)
			return m_Weapons[i];
	}
	return NULL;  
}  

void CEntityManager::RemoveWeapon(C6SBaseNetworkedWeapon* pWep) {
	m_Weapons.erase(std::remove(m_Weapons.begin(), m_Weapons.end(), pWep), m_Weapons.end());
}

void CEntityManager::AddCasingEffect(CSpentCasingEffect* effect) {
	m_Casings.push_back(effect);
}