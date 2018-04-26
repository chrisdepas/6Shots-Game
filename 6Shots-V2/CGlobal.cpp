#include "stdafx.h"
#include "CGlobal.h"

void CGlobal::Init(float gravity, float terminal) {
	m_fGravity = gravity;
	m_fTerminalVelocity = terminal;
}

float CGlobal::GetGravity() { 
	return m_fGravity;
}

void CGlobal::SetGravity(float gravity) {
	m_fGravity = gravity;
}

float CGlobal::GetTerminalVelocity() {
	return m_fTerminalVelocity;
}