#include "stdafx.h"
#include "Renderer.h"
#include "CGameObject.h"
#include <Windows.h>
#include <timeapi.h>

void CGameObject::Update(float elapsedTime)
{
	if (!equal(m_Velocity.magnitude(), 0.0f))
	{
		// 중력(Gravity) = mg (g: 중력가속도(9.8m/s²)
		Vector Gravity = Vector(1.0f, 1.0f, 1.0f) * m_Mass * GravityAccelarationFactor;

		// 마찰력(Friction) = uN (u: 마찰계수, N: 수직항력(-mg))
		Vector NormalForce = -Gravity;
		Vector Friction = m_FrictionFactor * NormalForce;
		Vector Direction = unit(m_Velocity);
		Friction.i *= Direction.i;
		Friction.j *= Direction.j;
		//Friction.k *= Direction.k;
		
		// 외력(ExternalForce)
		Vector ExternalForce = /*Gravity + */Friction;
		ExternalForce.k = -Gravity.k;

		CGameObject::ApplyForce(ExternalForce, elapsedTime);

		// Calculation Position
		// 새로운 위치 = 이전 위치 + 속도 * 시간
		m_Position += m_Velocity * elapsedTime;
	}

	// Set State
	if (m_Position.z > 0.0f)
	{
		m_State = OBJECT_STATE_AIR;
	}
	else
	{
		m_State = OBJECT_STATE_GROUND;
		m_Position.z = 0.0f;
		m_Velocity.k = 0.0f;
	}
}

void CGameObject::ApplyForce(Vector Force, float elapsedTime)
{
	// Calculation Acceleration
	// 가속도 = 힘(외력)/질량
	// a = F/m (m/s²)
	m_Acceleration = Force / m_Mass;
	if (equal(m_Acceleration.magnitude(), 0.0f)) m_Acceleration = { 0.0f, 0.0f, 0.0f };

	// Calculation Velocity
	// 새로운 속도 = 이전 속도 + 가속도 * 시간
	// v = a * s = (m/s²) * s
	Vector AfterVelocity = m_Velocity + m_Acceleration * elapsedTime;

	// Calculation Maximum Static Friction
	// 정지마찰계산 (현재속도와 계산후속도의 방향이 반대이면 정지 (단, x,y에 대해서만))
	if (AfterVelocity.i * m_Velocity.i < 0.0f || AfterVelocity.j * m_Velocity.j < 0.0f)
	{
		if (AfterVelocity.i * m_Velocity.i < 0.0f)
			m_Velocity.i = 0.0f;
		if (AfterVelocity.j * m_Velocity.j < 0.0f)
			m_Velocity.j = 0.0f;
	}
	else
		m_Velocity = AfterVelocity;
	if (equal(m_Velocity.magnitude(), 0.0f)) m_Velocity = { 0.0f, 0.0f, 0.0f };
}

bool CGameObject::DealayComplete(int DealayKind)
{
	// Dealay 타입에 따른 StartDealayTime 메모리확보
	if (m_StartDelayTimes.size() <= (size_t)DealayKind)
	{
		for (int i = DealayKind - m_StartDelayTimes.size(); i >= 0; --i)
			m_StartDelayTimes.emplace_back();
		m_StartDelayTimes[DealayKind] = timeGetTime();
	}
	return true;
}

bool CGameObject::DealayInit(int DealayKind)
{
	CGameObject::DealayComplete(DealayKind);
	m_StartDelayTimes[DealayKind] = timeGetTime();
	return true;
}

void CHeroHead::ProcessSpecialKey(bool* KeyBuffer, float elapsedTime)
{
	if (KeyBuffer[0x0065] == true) // 0x0065 : GLUT_KEY_UP
	{
		// Animation Frame Set(UpView-Sequance)
		if (4 > (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 4.0f;
		m_TextureSequenceCurrentPos.x += 1.0f;
		if (5 < (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 4.0f;
	}
	if (KeyBuffer[0x0064] == true) // 0x0064 : GLUT_KEY_LEFT
	{
		// Animation Frame Set(LeftView-Sequance)
		if (6 > (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 6.0f;
		m_TextureSequenceCurrentPos.x += 1.0f;
		if (7 < (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 6.0f;
	}
	if (KeyBuffer[0x0067] == true) // 0x0067 : GLUT_KEY_DOWN
	{
		// Animation Frame Set(DownView-Sequance)
		if (0 > (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 0.0f;
		m_TextureSequenceCurrentPos.x += 1.0f;
		if (1 < (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 0.0f;
	}
	if (KeyBuffer[0x0066] == true) // 0x0066 : GLUT_KEY_RIGHT
	{
		// Animation Frame Set(RightView-Sequance)
		if (2 > (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 2.0f;
		m_TextureSequenceCurrentPos.x += 1.0f;
		if (3 < (int)m_TextureSequenceCurrentPos.x) m_TextureSequenceCurrentPos.x = 2.0f;
	}
}

bool CHeroHead::DealayComplete(int DealayKind)
{
	CGameObject::DealayComplete(DealayKind);

	// 눈물 발사 경과시간(CurrentTime - StartTime) 체크
	if (DealayKind == KIND_HERO_DEALAY_SHOOT && timeGetTime() - m_StartDelayTimes[DealayKind] >= HERO_SHOOT_DEALAY)
		return true;
	return false;
}

// elapsedTime동안 1N의 힘을 오브젝트에 가한다.
void CHeroBody::ProcessKey(bool* KeyBuffer, float elapsedTime)
{
	Vector newForce;
	if (m_Velocity.magnitude() < HERO_MAX_SPEED)
	{
		if (KeyBuffer['w'] == true)
			newForce.j += HERO_SPEED;
		if (KeyBuffer['a'] == true)
			newForce.i -= HERO_SPEED;
		if (KeyBuffer['s'] == true)
			newForce.j -= HERO_SPEED;
		if (KeyBuffer['d'] == true)
			newForce.i += HERO_SPEED;
	}

	if (KeyBuffer[32] == true && m_State == OBJECT_STATE_GROUND)
		newForce.k += HERO_JUMP_SPEED;

	// 힘의 크기가 0이 아니라면 오브젝트에 힘을 가한다.
	if (!equal(newForce.magnitude(), 0.0f))
		CGameObject::ApplyForce(newForce, elapsedTime);
}

bool CHeroBody::DealayComplete(int DealayKind)
{
	CGameObject::DealayComplete(DealayKind);

	if (DealayKind == KIND_HERO_DEALAY_HIT)
	{
		// 플레이어 무적상태 경과시간 체크
		if (timeGetTime() - m_StartDelayTimes[DealayKind] >= HERO_HIT_DEALAY)
		{
			m_Color.g = 1.0f;
			m_Color.b = 1.0f;
			return true;
		}
	}
	else if (DealayKind == KIND_HERO_DEALAY_HIT_COLORCHANGE)
	{
		// 플레이어 무적상태일 경우에는 플레이어에게 Hitting Peedback을 Hero 캐릭터 색상으로 알림
			// 이때, 색상은 PLAYER_HIT_COLORCHANGE_DEALAY마다 바뀜
		if (timeGetTime() - m_StartDelayTimes[DealayKind] >= HERO_HIT_COLORCHANGE_DEALAY)
		{
			m_Color.g = (equal(m_Color.g, 0.0f)) ? 1.0f : 0.0f;
			m_Color.b = (equal(m_Color.b, 0.0f)) ? 1.0f : 0.0f;
			return true;
		}
	}
	return false;
}

void CHeroBody::Update(float elapsedTime)
{
	CGameObject::Update(elapsedTime);
	// Animation Frame Update
	// Sprite Sequence 계산 (속도에 비례 / Sprite 상 캐릭터 보폭을 반영해서 Sequence 전환)
	Vector MoveDistance = m_Velocity * elapsedTime;

	if (abs(m_Velocity.i) < 0.5f) // Move Up/Down Or Stop
	{
		if ((int)m_TextureSequenceCurrentPos.y != 0)
		{
			m_TextureSequenceCurrentPos.y = 0.0f;
			m_TextureSequenceCurrentPos.x = 0.0f;
		}
		if (abs(MoveDistance.j) > 0.0f)
			m_FootStep += abs(MoveDistance.j) / (HERO_FOOTSTEP_DIST_ONE_SEQUENCE);
	}
	else if (m_Velocity.i < 0.0f) // Move Left
	{
		if ((int)m_TextureSequenceCurrentPos.y != 1)
		{
			m_TextureSequenceCurrentPos.y = 1.0f;
			m_TextureSequenceCurrentPos.x = 0.0f;
		}
		m_FootStep += abs(MoveDistance.i) / (HERO_FOOTSTEP_DIST_ONE_SEQUENCE);
	}
	else if (m_Velocity.i > 0.0f) // Move Right
	{
		if ((int)m_TextureSequenceCurrentPos.y != 2)
		{
			m_TextureSequenceCurrentPos.y = 2.0f;
			m_TextureSequenceCurrentPos.x = 0.0f;
		}
		m_FootStep += abs(MoveDistance.i) / (HERO_FOOTSTEP_DIST_ONE_SEQUENCE);
	}
	// MoveDist = 이동 거리 : 이동 속도 * 게임 경과시간
	// FootStep = 1보폭 : Isaac_Body.png상에서의 한 행 (10 Sequence)
	// FootStepDist = 1보폭 간격 : 17pixel(0.17m)
	// FootStepDist_OneSeq = 1Sequence 당 보폭 간격 : 17pixel / 10 Sequence = 1.7Pixel(0.017m)
	// MoveDist > FootStepDist_OneSeq 일 경우
	// 현재 SequenceNumber = 이전 SequenceNumber + (MoveDist / FootStepDist_OneSeq)
	while (m_FootStep - 1.0f > 0.0f)
	{
		m_FootStep = (m_FootStep - 1.0f < 0.0f) ? 0.0f : m_FootStep - 1.0f;
		m_TextureSequenceCurrentPos.x = (m_Velocity.magnitude() > 0.5f) ? ((int)m_TextureSequenceCurrentPos.x + 1) % (int)m_TextureSequenceMaxPos.x : 0.0f;
	}
}

bool CHeroDeadBody::DealayComplete(int DealayKind)
{
	CGameObject::DealayComplete(DealayKind);
	if (DealayKind == KIND_HERO_DEALAY_DEADSEQUENCE_CHANGE && timeGetTime() - m_StartDelayTimes[DealayKind] >= HERO_DEADSEQUENCE_CHANGE_DEALAY)
		return true;
	return false;
}

void CHeroDeadBody::Update(float elapsedTime)
{
	if (CHeroDeadBody::DealayComplete(KIND_HERO_DEALAY_DEADSEQUENCE_CHANGE) == true)
	{
		if ((int)m_TextureSequenceCurrentPos.x < MAX_HERO_DEAD_ANIMATION_SEQUENCE_X - 1)
			m_TextureSequenceCurrentPos.x += 1.0f;
	}
}
