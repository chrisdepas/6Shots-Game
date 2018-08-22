#ifndef __C6SPACKET_H__
#define __C6SPACKET_H__

/*
		Christopher De Pasquale
Unknown date, notice added 12 December 2016
*/

class C6SPacket : public sf::Packet {
	int m_iPacketID;
public: 
	enum EPacketID {
		ID_NONE, /* NO ID */
		ID_NOTSET, /* ID WASN'T SET */
		ID_CLIENT_CONNECTION_REQUEST,
		ID_SERVER_CONNECTION_RESPONSE,
		ID_CLIENT_DISCONNECT_MESSAGE,	
		ID_SERVER_KICK,					/* Server kicks a player */

		ID_CLIENT_CHAT_MESSAGE,			/* Client sends chat message to server */

		ID_SERVER_CHAT_MESSAGE,			/* Server informs client of chat message */
		ID_SERVER_PLAYER_LOBBY_UPDATE,	/* Server informs client that lobby has changed */
		ID_SERVER_LAUNCH_GAME,			/* Server informs client that game has launched from lobby */
		ID_SERVER_ROUND_START,			/* Server informs client that round has begun */
		ID_SERVER_ROUND_RESTART,		/* Server instructs client to reset game state */

		ID_CLIENT_GAME_READY,			/* Client has loaded into game */
		ID_CLIENT_TICK,					/* Client sends tick (player state) */

		ID_SERVER_PLAYER_UPDATE,		/* Server informs client of a change in player state */

		ID_CLIENT_PLAYER_INPUT_EVENT,	/* Client informs server of input */

		ID_SERVER_PLAYER_EVENT,			/* Server informs client of a player event (e.g. shooting, weapon pickup) */
		ID_SERVER_ENTITY_CREATE,		/* Server informs client of a new entity */

		ID_CLIENT_CONCMD,				/* Client informs server of a console command request (e.g. /kill) */

		ID_SERVER_MESSAGE,				/* Server sends a broadcast message */
	
	};

	C6SPacket() {
		m_iPacketID = ID_NOTSET;
	}
	void SetID(int id) {
		m_iPacketID = id;
	}
	int GetID() {
		return m_iPacketID;
	}
	virtual const void* onSend(std::size_t &size);
	virtual void onReceive(const void *data, std::size_t size);
};

#endif