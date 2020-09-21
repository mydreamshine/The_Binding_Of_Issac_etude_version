#include "stdafx.h"
#include "Renderer.h"
#include "CGameObject.h"
#include <Windows.h>
#include <timeapi.h>

void CGameObject::Update(float elapsedTime)
{
	if (!equal(m_Velocity.magnitude(), 0.0f))
	{
		// �߷�(Gravity) = mg (g: �߷°��ӵ�(9.8m/s��)
		Vector Gravity = Vector(1.0f, 1.0f, 1.0f) * m_Mass * GravityAccelarationFactor;

		// ������(Friction) = uN (u: �������, N: �����׷�(-mg))
		Vector NormalForce = -Gravity;
		Vector Friction = m_FrictionFactor * NormalForce;
		Vector Direction = unit(m_Velocity);
		Friction.i *= Direction.i;
		Friction.j *= Direction.j;
		//Friction.k *= Direction.k;
		
		// �ܷ�(ExternalForce)
		Vector ExternalForce = /*Gravity + */Friction;
		ExternalForce.k = -Gravity.k;

		CGameObject::ApplyForce(ExternalForce, elapsedTime);

		// Calculation Position
		// ���ο� ��ġ = ���� ��ġ + �ӵ� * �ð�
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
	// ���ӵ� = ��(�ܷ�)/����
	// a = F/m (m/s��)
	m_Acceleration = Force / m_Mass;
	if (equal(m_Acceleration.magnitude(), 0.0f)) m_Acceleration = { 0.0f, 0.0f, 0.0f };

	// Calculation Velocity
	// ���ο� �ӵ� = ���� �ӵ� + ���ӵ� * �ð�
	// v = a * s = (m/s��) * s
	Vector AfterVelocity = m_Velocity + m_Acceleration * elapsedTime;

	// Calculation Maximum Static Friction
	// ����������� (����ӵ��� ����ļӵ��� ������ �ݴ��̸� ���� (��, x,y�� ���ؼ���))
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
	// Dealay Ÿ�Կ� ���� StartDealayTime �޸�Ȯ��
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

	// ���� �߻� ����ð�(CurrentTime - StartTime) üũ
	if (DealayKind == KIND_HERO_DEALAY_SHOOT && timeGetTime() - m_StartDelayTimes[DealayKind] >= HERO_SHOOT_DEALAY)
		return true;
	return false;
}

// elapsedTime���� 1N�� ���� ������Ʈ�� ���Ѵ�.
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

	// ���� ũ�Ⱑ 0�� �ƴ϶�� ������Ʈ�� ���� ���Ѵ�.
	if (!equal(newForce.magnitude(), 0.0f))
		CGameObject::ApplyForce(newForce, elapsedTime);
}

bool CHeroBody::DealayComplete(int DealayKind)
{
	CGameObject::DealayComplete(DealayKind);

	if (DealayKind == KIND_HERO_DEALAY_HIT)
	{
		// �÷��̾� �������� ����ð� üũ
		if (timeGetTime() - m_StartDelayTimes[DealayKind] >= HERO_HIT_DEALAY)
		{
			m_Color.g = 1.0f;
			m_Color.b = 1.0f;
			return true;
		}
	}
	else if (DealayKind == KIND_HERO_DEALAY_HIT_COLORCHANGE)
	{
		// �÷��̾� ���������� ��쿡�� �÷��̾�� Hitting Peedback�� Hero ĳ���� �������� �˸�
			// �̶�, ������ PLAYER_HIT_COLORCHANGE_DEALAY���� �ٲ�
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
	// Sprite Sequence ��� (�ӵ��� ��� / Sprite �� ĳ���� ������ �ݿ��ؼ� Sequence ��ȯ)
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
	// MoveDist = �̵� �Ÿ� : �̵� �ӵ� * ���� ����ð�
	// FootStep = 1���� : Isaac_Body.png�󿡼��� �� �� (10 Sequence)
	// FootStepDist = 1���� ���� : 17pixel(0.17m)
	// FootStepDist_OneSeq = 1Sequence �� ���� ���� : 17pixel / 10 Sequence = 1.7Pixel(0.017m)
	// MoveDist > FootStepDist_OneSeq �� ���
	// ���� SequenceNumber = ���� SequenceNumber + (MoveDist / FootStepDist_OneSeq)
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
