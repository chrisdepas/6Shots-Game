#include "stdafx.h"
#include "CChatList.h"
#include "CGame.h"

CChatList::~CChatList() {
	for (int i = 0; i < m_iNumMessages; i++)
	{
		free(m_pszMessageBuffer[i]); 
	}
}
void CChatList::Draw(CGame* pGame)
{
	if (m_fTimeSinceLastShown >= CHATLIST_END_FADE_TIME)
		return;

	int alpha = 255;
	if (m_fTimeSinceLastShown > CHATLIST_BEGIN_FADE_TIME) {
		float fFadeTime = CHATLIST_END_FADE_TIME - CHATLIST_BEGIN_FADE_TIME;
		alpha = (int)(255.0f * (1.0f - ((m_fTimeSinceLastShown - CHATLIST_BEGIN_FADE_TIME) / fFadeTime)));
	}
	int yoffset = 0;
	for (int i = 0; i < m_iNumMessages; i++)
	{
		pGame->m_Drawing.DrawText(&pGame->m_WindowManager, m_pszMessageBuffer[i], -2, yoffset + 1, m_iFontSize, 0, 0, 0, alpha);
		pGame->m_Drawing.DrawText(&pGame->m_WindowManager, m_pszMessageBuffer[i], 0, yoffset, m_iFontSize, 255, 255, 255, alpha);
		yoffset += m_iFontSize;
	}
} 

void CChatList::AddMessage(char* szMessage)
{
	if (!szMessage)
		return;

	m_fTimeSinceLastShown = 0.0f;

	/* We have room, add message to queue */
	if (m_iNumMessages < iChatlistBufferSize)
	{
		int slen = strlen(szMessage);
		m_pszMessageBuffer[m_iNumMessages] = (char*)malloc(slen + 1);
		memcpy(m_pszMessageBuffer[m_iNumMessages], szMessage, slen);
		m_pszMessageBuffer[m_iNumMessages][slen] = 0;
		m_iNumMessages++;
		return;
	}

	/* Already have max number of messages, delete oldest one */
	free(m_pszMessageBuffer[0]);

	/* Shift down */
	for (int i = 0; i < m_iNumMessages - 1; i++)
	{
		m_pszMessageBuffer[i] = m_pszMessageBuffer[i + 1];
	}

	/* Decrement amount of messages - one has been removed */
	m_iNumMessages--;

	/* Add message to queue */
	AddMessage(szMessage);
}

void CChatList::Update(float fFrameTime) {
	if (m_fTimeSinceLastShown < CHATLIST_END_FADE_TIME)
		m_fTimeSinceLastShown += fFrameTime;
}
void CChatList::Show() {
	m_fTimeSinceLastShown = 0.0f;
}