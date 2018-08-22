#ifndef __CBASEENTITY_H__
#define __CBASEENTITY_H__


class CGame;

class CBaseEntity {
	
protected:
	bool m_bFacingRight;
	bool m_bAffectedByGravity;
	sf::Vector2f m_vVelocity;
	sf::Vector2f m_vLocation;
	sf::Vector2i m_vSize;
	float m_fScale;
	sf::Texture m_texture;
	int m_Flags;
public:

	enum EBaseFlags {
		FL_FACING_LEFT =	(1 << 0), 
		FL_WALKING =		(1 << 1),
	};

	sf::Vector2f GetVelocity();
	sf::Vector2f GetPosition2f();
	sf::Vector2i GetPosition2i();
	CBaseEntity();

	void SetBaseFlag(EBaseFlags f, bool b);
	bool GetBaseFlag(EBaseFlags f);

	void SetVelocity(sf::Vector2f v);
	void SetPosition(sf::Vector2f p);
	void SetPosition(sf::Vector2i p);
	void DefaultDraw(CGame* pGame);

	virtual void Draw(CGame* pGame){}
};
#endif