#include "stdafx.h"
#include "CBaseCombatEntity.h"

int CBaseCombatEntity::GetHP() { return m_iHP; }

void CBaseCombatEntity::SetMaxHP(int iMaxHP) {
	m_iMaxHP = iMaxHP;
}
void CBaseCombatEntity::SetHP(int hp) {
	m_iHP = hp <= m_iMaxHP ? hp : m_iMaxHP;
}
void CBaseCombatEntity::Heal(int heal) {
	m_iHP += heal;
	if (m_iHP > m_iMaxHP) {
		m_iHP = m_iMaxHP;
	}
}
void CBaseCombatEntity::TakeDamage(int damage) {
	m_iHP -= damage;
	if (m_iHP < 0) {
		m_iHP = 0;
	}
}
void CBaseCombatEntity::Kill() {
	m_iHP = 0;
}
bool CBaseCombatEntity::IsDead() {
	return m_iHP <= 0;
}
void CBaseCombatEntity::AddScore(int iScoreAmt) {
	m_iScore += iScoreAmt;
}
int CBaseCombatEntity::GetScore() {
	return m_iScore;
}