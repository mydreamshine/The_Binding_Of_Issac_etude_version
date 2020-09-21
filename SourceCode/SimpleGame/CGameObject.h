#pragma once
#include <vector>
/*
게임 상에서의 기본 물리 단위를 정의해야 한다.
길이, 속도, 시간주기 등등.

현재 이 테스트 게임에선
1pixel을 1cm로 가정한다.
기본속도는 1m/s로 가정한다.

갱신될 위치 = 이전 위치 + 가속도 * 시간
갱신될 속도 = 이전 속도 + 가속도 * 시간

가속도 = 힘 / 질량
속도 = 가속도 * 시간
마찰력 = 힘 * 마찰계수



속도 : m/s
가속도 : m/s²
힘 : N
질량 : kg
*/

class CGameObject;
class CHeroHead;
class CBoss;
class CBullet;



class CGameObject
{
protected:
	// Phisical Option
	Point          m_Position;
	fSIZE          m_Size = { 0.0f, 0.0f, 0.0f };
	fSIZE          m_BoundingBoxSize = { 0.0f, 0.0f, 0.0f };
	Vector         m_Velocity;
	Vector         m_Acceleration;
	float          m_Mass = 0.0f;
	float          m_FrictionFactor = 0.0f;

	// Render Option
	int            m_TextureID = 0;
	Point          m_TextureSequenceMaxPos;
	Point          m_TextureSequenceCurrentPos;
	COLOR          m_Color = { 1.0f, 0.0f, 0.0f, 1.0f };
	
	// Game Option
	int            m_HP = 0;
	int            m_Damage = 0;
	int            m_Kind = KIND_NULL;
	int            m_State = OBJECT_STATE_GROUND;

	std::vector<unsigned long> m_StartDelayTimes;

	bool           m_bInitialized = true;

public:
	CGameObject()  = default;
	~CGameObject() = default;

	// Phisical Option
	virtual bool SetPosition(Point newPosition)             { m_Position = newPosition; return true; }
	virtual bool SetSize(fSIZE newSize)                     { m_Size = newSize; return true; }
	virtual bool SetBoudingBoxSize(fSIZE newSize)           { m_BoundingBoxSize = newSize; return true; }
	virtual bool SetVelocity(Vector newVelocity)            { m_Velocity = newVelocity; return true; }
	virtual bool SetAcceleration(Vector newAcceleration)    { m_Acceleration = newAcceleration; return true; }
	virtual bool SetMass(float fmass)                       { m_Mass = fmass; return true; }
	virtual bool SetFrictionFactor(float fFrictionFactor)   { m_FrictionFactor = fFrictionFactor; return true;}

	// Render Option
	virtual bool SetTextureID(int newTextureID)             { m_TextureID = newTextureID; return true; }
	virtual bool SetTextureSequenceMaxPos(Point newPos)     { m_TextureSequenceMaxPos = newPos; return true; }
	virtual bool SetTextureSequenceCurrentPos(Point newPos) { m_TextureSequenceCurrentPos = newPos; return true; }
	virtual bool SetColor(COLOR newColor)                   { m_Color = newColor; return true;}

	// Game Option
	virtual bool SetKind(int newkind)                       { m_Kind = newkind; return true; }
	virtual bool SetHP(int newHP)                           { m_HP = newHP; return true; }
	virtual bool SetDamage(int newDamage)                   { m_Damage = newDamage; return true; }
	virtual bool SetState(int newState)                     { m_State = newState; return true; }

	// Phisical Option
	virtual Point  GetPosition()                            { return m_Position; }
	virtual fSIZE  GetSize()                                { return m_Size; }
	virtual fSIZE  GetBoundingBoxSize()                     { return m_BoundingBoxSize; }
	virtual fBOX   GetBoudingBox()                          { return fBOX{ m_Position.x - m_BoundingBoxSize.Width / 2,	   // Left(minX)
		                                                                   m_Position.y - m_BoundingBoxSize.Height / 2,	   // Bottom(minY)
		                                                                   m_Position.z - m_BoundingBoxSize.Depth / 2,	   // Back(minZ)
		                                                                   m_Position.x + m_BoundingBoxSize.Width / 2,	   // Right(maxX)
	                                                                       m_Position.y + m_BoundingBoxSize.Height / 2,	   // Top(maxY)
	                                                                       m_Position.z + m_BoundingBoxSize.Depth / 2 }; } // Front(maxZ)
	virtual Vector GetVelocity()                            { return m_Velocity; }
	virtual Vector GetAcceleration()                        { return m_Acceleration; }
	virtual float  GetMass()                                { return m_Mass; }
	virtual float  GetFrictionFactor()                      { return m_FrictionFactor; }

	// Render Option
	virtual int   GetTextureID()                            { return m_TextureID; }
	virtual Point GetTextureSequenceMaxPos()                { return m_TextureSequenceMaxPos; }
	virtual Point GetTextureSequenceCurrentPos()            { return m_TextureSequenceCurrentPos; }
	virtual COLOR GetColor()                                { return m_Color; }

	// Game Option
	virtual int GetKind()                                   { return m_Kind; }
	virtual int GetHP()                                     { return m_HP; }
	virtual int GetDamage()                                 { return m_Damage; }
	virtual int GetState()                                  { return m_State; }


	bool IsInitialized()                                    { return m_bInitialized; }


	//////////////////////// Input Processing ////////////////////////
	virtual void ProcessMouse(int button, int state, int x, int y, float elapsedTime) {}
	virtual void ProcessKey(bool* KeyBuffer, float elapsedTime) {}
	virtual void ProcessSpecialKey(bool* KeyBuffer, float elapsedTime) {}


	//////////////////////// Object Action ////////////////////////
	virtual void Update(float elapsedTime);
	virtual void ApplyForce(Vector Force, float elapsedTime);

	//////////////////////// Object Dealay ////////////////////////
	virtual bool DealayComplete(int DealayKind);
	virtual bool DealayInit(int DealayKind);
};




class CHeroHead : public CGameObject
{
public:
	virtual void ProcessSpecialKey(bool* KeyBuffer, float elapsedTime);
	virtual bool DealayComplete(int DealayKind);
};



class CHeroBody : public CGameObject
{
private:
	float m_FootStep = 0.0f;
public:
	virtual void ProcessKey(bool* KeyBuffer, float elapsedTime);
	virtual bool DealayComplete(int DealayKind);
	virtual void Update(float elapsedTime);
};

class CHeroDeadBody : public CGameObject
{
private:
	virtual bool DealayComplete(int DealayKind);
public:
	virtual void Update(float elapsedTime);
};


class CBoss : public CGameObject
{
private:
	// UnderOrdered_Offset_Y: 2차원 평면에서의 깊이감을 나타내기 위해 BoundingRect를 실제 크기보다 작게 하고 이를
	// 실제 Bottom과 BoudingRect의 Bottom을 같게 아래쪽 맞춤으로 정렬함
	float m_UnderOrdered_Offset_Y = -((BOSS_HEIGHT * 0.5f) - BOSS_BOUNDINGBOX_HEIGHT);
public:
	virtual fBOX GetBoudingBox() {
		return fBOX{
			m_Position.x - m_BoundingBoxSize.Width / 2,                             // Left(minX)
			m_Position.y - m_BoundingBoxSize.Height / 2 + m_UnderOrdered_Offset_Y,  // Bottom(minY)
			m_Position.z - m_BoundingBoxSize.Depth / 2,                             // Back(minZ)
			m_Position.x + m_BoundingBoxSize.Width / 2,                             // Right(maxX)
			m_Position.y + m_BoundingBoxSize.Height / 2 + m_UnderOrdered_Offset_Y,  // Top(maxY)
			m_Position.z + m_BoundingBoxSize.Depth / 2                              // Front(maxZ)
		};
	}
};



class CBullet : public CGameObject
{

};

class CPressurePlate : public CGameObject
{

};