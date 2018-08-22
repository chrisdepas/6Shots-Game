#include "stdafx.h"
#include "CGameStateHandler.h"
#include "CGameState.h"
#include "CGame.h"

CGameStateHandler::CGameStateHandler()
{
	m_State = 0;
}
void CGameStateHandler::Cleanup(CGame* pGame)
{
	if (m_State)
		m_State->Cleanup(pGame);
}
CGameState* CGameStateHandler::GetCurrentState()
{
	return m_State;
}
void CGameStateHandler::ChangeState(CGameState* pState, CGame* pGame)
{
	if (pState)
	{
		m_State = pState;
		m_State->Init(pGame);
		m_State->OnInitialised();
	}
}
void CGameStateHandler::Draw(CGame* pGame)
{
	if (!m_State)
		return;
	m_State->Draw(pGame);
}
void CGameStateHandler::Update(CGame* pGame, float fFrameTime)
{
	if (!m_State)
		return;
	m_State->Update(pGame, fFrameTime);
 }
void CGameStateHandler::HandleInput(CGame* pGame)
{
	if (!m_State)
		return;
	m_State->HandleInput(pGame);
}
bool CGameStateHandler::IsCurrentState(CGameState* pGameState) {
	return m_State == pGameState;
}