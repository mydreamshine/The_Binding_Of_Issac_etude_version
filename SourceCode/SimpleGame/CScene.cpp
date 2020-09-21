#include "stdafx.h"
#include "CScene.h"
#include "CollisionFunc.h"

int GameScene::FindEmptyObjectSlot()
{
	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		if (m_Objects[i] == NULL && i != BOSS_ID && i != HERO_BODY_ID && i != HERO_HEAD_ID)
			return i;
	}
	return -1;
}

void GameScene::AddObject(int kind, int State, int HP, int Damage, Point Position, fSIZE Size, fSIZE BoundingBoxSize, float mass, float frictionFactor, Vector Velocity)
{
	int index = FindEmptyObjectSlot();
	if (kind == KIND_BOSS)
	{
		index = BOSS_ID;
		if (m_Objects[index] != NULL)
		{
			delete m_Objects[index];
			m_Objects[index] = NULL;
		}
	}
	if (kind == KIND_HERO_BODY)
	{
		index = HERO_BODY_ID;
		if (m_Objects[index] != NULL)
		{
			delete m_Objects[index];
			m_Objects[index] = NULL;
		}
	}
	if (kind == KIND_HERO_HEAD)
	{
		index = HERO_HEAD_ID;
		if (m_Objects[index] != NULL)
		{
			delete m_Objects[index];
			m_Objects[index] = NULL;
		}
	}
	if (index < 0)
	{
		std::cout << "Not Find Object Empty Slot!" << std::endl;
		return;
	}

	if (kind == KIND_HERO_BODY)
	{
		m_Objects[index] = new CHeroBody();
		m_Objects[index]->SetTextureID(m_TextureIDs[KIND_HERO_BODY]);
		m_Objects[index]->SetTextureSequenceMaxPos({ MAX_HERO_BODY_ANIMATION_SEQUENCE_X, MAX_HERO_BODY_ANIMATION_SEQUENCE_Y });
	}
	else if (kind == KIND_HERO_HEAD)
	{
		m_Objects[index] = new CHeroHead();
		m_Objects[index]->SetTextureID(m_TextureIDs[KIND_HERO_HEAD]);
		m_Objects[index]->SetTextureSequenceMaxPos({ MAX_HERO_HEAD_ANIMATION_SEQUENCE_X, MAX_HERO_HEAD_ANIMATION_SEQUENCE_Y });
	}
	else if (kind == KIND_HERO_DEADBODY)
	{
		m_Objects[index] = new CHeroDeadBody();
		m_Objects[index]->SetTextureID(m_TextureIDs[KIND_HERO_DEADBODY]);
		m_Objects[index]->SetTextureSequenceMaxPos({ MAX_HERO_DEAD_ANIMATION_SEQUENCE_X, MAX_HERO_DEAD_ANIMATION_SEQUENCE_Y });
	}
	else if (kind == KIND_BOSS)
	{
		m_Objects[index] = new CBoss();
		m_Objects[index]->SetTextureID(m_TextureIDs[KIND_BOSS]);
		m_Objects[index]->SetTextureSequenceMaxPos({ MAX_BOSS_ANIMATION_SEQUENCE_X, MAX_BOSS_ANIMATION_SEQUENCE_Y });
	}
	else if (kind == KIND_BULLET_NORMAL || kind == KIND_BULLET_BLOOD)
	{
		m_Objects[index] = new CBullet();
		m_Objects[index]->SetTextureID((kind == KIND_BULLET_NORMAL) ? m_TextureIDs[KIND_BULLET_NORMAL] : m_TextureIDs[KIND_BULLET_BLOOD]);
		m_Objects[index]->SetTextureSequenceMaxPos({ 1,1 });
	}
	else if (kind == KIND_PRESSSURE_PLATE)
	{
		m_Objects[index] = new CPressurePlate();
		m_Objects[index]->SetTextureID(m_TextureIDs[KIND_PRESSSURE_PLATE]);
		m_Objects[index]->SetTextureSequenceMaxPos({ MAX_PRESSURE_PLATE_ANIMATION_SEQUENCE_X, MAX_PRESSURE_PLATE_ANIMATION_SEQUENCE_Y });
	}
	m_Objects[index]->SetKind(kind);
	m_Objects[index]->SetState(State);
	m_Objects[index]->SetHP(HP);
	m_Objects[index]->SetDamage(Damage);
	m_Objects[index]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	m_Objects[index]->SetPosition(Position);
	m_Objects[index]->SetSize(Size);
	m_Objects[index]->SetBoudingBoxSize(BoundingBoxSize);
	m_Objects[index]->SetMass(mass);
	m_Objects[index]->SetFrictionFactor(frictionFactor);
	m_Objects[index]->SetVelocity(Velocity);
}

void GameScene::DeleteObject(int id)
{
	if (id < 0)
	{
		std::cout << "You Try Underflow Accesss into ObjectArray!" << std::endl;
		std::cout << "Accessing index: " << id << std::endl;
		return;
	}
	else if (id >= MAX_OBJECTS)
	{
		std::cout << "You Try Overflow Accesss into ObjectArray!" << std::endl;
		std::cout << "Accessing index: " << id << std::endl;
		return;
	}
	else if (m_Objects[id] == NULL)
	{
		std::cout << "Objects[" << id << "] these NULL!" << std::endl;
		return;
	}
	if (m_Objects[id] != NULL)
	{
		delete m_Objects[id];
		m_Objects[id] = NULL;
	}
}

void GameScene::DoGarbageCollect()
{
	int    Kind;
	int    HP;
	Point  Position;
	Vector Velocity;
	fBOX   BoudingBox;
	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		if (m_Objects[i] == NULL)
			continue;

		Kind = m_Objects[i]->GetKind();
		Position = m_Objects[i]->GetPosition();
		BoudingBox = m_Objects[i]->GetBoudingBox();
		Velocity = m_Objects[i]->GetVelocity();
		HP = m_Objects[i]->GetHP();
		
		// Check HP
		if (HP <= 0)
		{
			if (Kind == KIND_HERO_BODY)
			{
				fSIZE Size = m_Objects[i]->GetSize(), BoundingBoxSize = m_Objects[i]->GetBoundingBoxSize();
				float Mass = m_Objects[i]->GetMass(), FrcitionFactor = m_Objects[i]->GetFrictionFactor();
				GameScene::AddObject(KIND_HERO_DEADBODY, OBJECT_STATE_GROUND, 1, 0, Position, Size, BoundingBoxSize, Mass, FrcitionFactor, Velocity);
				DeleteObject(HERO_HEAD_ID);

				m_pSound->PlaySound(m_SoundIDs[SOUND_HERO_DIES], false, 0.7f);
			}
			DeleteObject(i);
			continue;
		}
		// Check Velocity
		if (equal(Velocity.magnitude(), 0.0f) == true && (Kind == KIND_BULLET_NORMAL || Kind == KIND_BULLET_BLOOD))
		{
			DeleteObject(i);

			m_pSound->PlaySound(m_SoundIDs[SOUND_TEAR_BLOCK], false, 1.0f);
			continue;
		}
		// Check Boundary
		if ((Kind == KIND_BULLET_NORMAL || Kind == KIND_BULLET_BLOOD)
			&& Check_BoundaryOut(WORLD_BOUNDARY_LEFT, WORLD_BOUNDARY_BOTTOM, WORLD_BOUNDARY_RIGHT, WORLD_BOUNDARY_TOP,
				BoudingBox.Left, BoudingBox.Bottom, BoudingBox.Right, BoudingBox.Top))
		{
			DeleteObject(i);

			m_pSound->PlaySound(m_SoundIDs[SOUND_TEAR_BLOCK], false, 1.0f);
			continue;
		}
	}
}

void GameScene::BulletShoot(int BulletKind, int ShootID)
{
	if (ShootID == SHOOT_NONE)
		return;

	Point  newPoint = (BulletKind == KIND_BULLET_NORMAL) ? m_Objects[HERO_HEAD_ID]->GetPosition() : m_Objects[BOSS_ID]->GetPosition();
	fSIZE  newSize = { BULLET_WIDTH, BULLET_HEIGHT, 0.0f };
	fSIZE  newBoundingBoxSize = { BULLET_BOUNDINGBOX_WIDTH, BULLET_BOUNDINGBOX_HEIGHT, 0.0f };
	float  newMass = BULLET_MASS;
	float  newFrictionFactor = BULLET_FRICTION_FACTOR;
	Vector newVelocity = (BulletKind == KIND_BULLET_NORMAL) ? m_Objects[HERO_HEAD_ID]->GetVelocity() : Vector(0.0f, 0.0f, 0.0f);
	float newAcceleration_Magnitude = (BulletKind == KIND_BULLET_NORMAL) ? BULLET_SPEED_FOR_HERO : BULLET_SPEED_FOR_BOSS;

	// Hero가 발사한 눈물
	if (BulletKind == KIND_BULLET_NORMAL)
	{
		switch (ShootID)
		{
		case SHOOT_LEFT:
			newVelocity.i -= newAcceleration_Magnitude;
			break;
		case SHOOT_RIGHT:
			newVelocity.i += newAcceleration_Magnitude;
			break;
		case SHOOT_UP:
			newVelocity.j += newAcceleration_Magnitude;
			break;
		case SHOOT_DOWN:
			newVelocity.j -= newAcceleration_Magnitude;
			break;
		}
		GameScene::AddObject(BulletKind, OBJECT_STATE_AIR, 1, BULLET_DAMAGE, newPoint, newSize, newBoundingBoxSize, newMass, newFrictionFactor, newVelocity);
		// Play Sound (Bullet Shot)
		m_pSound->PlaySound(m_SoundIDs[SOUND_TEAR_FIRE], false, 1.0f);
	}
	// Boss가 발사한 눈물
	else if (BulletKind == KIND_BULLET_BLOOD)
	{
		int bossShootNum;
		float currentShoot;
		float SizeFactor;
		switch (ShootID)
		{
		case BOSS_SHOOT_PATTERN_1:
			bossShootNum = 7 + rand() % 4; // 랜덤 발사 눈물 갯수
			currentShoot = 0;
			while (currentShoot < bossShootNum)
			{
				// 오른쪽 시야각으로 랜덤방향
				newVelocity.i = cos(Radian((float)(rand() % 45)));
				if (!(rand() % 2))
					newVelocity.j = sin(Radian((float)(rand() % 45)));
				else
					newVelocity.j = -sin(Radian((float)(rand() % 45)));
				newVelocity = newVelocity * newAcceleration_Magnitude;

				// 랜덤 크기
				SizeFactor = 1.0f + rand() / (float)RAND_MAX;
				newSize = { BULLET_WIDTH * SizeFactor, BULLET_HEIGHT * SizeFactor, 0.0f };
				newBoundingBoxSize = { BULLET_BOUNDINGBOX_WIDTH * SizeFactor, BULLET_BOUNDINGBOX_HEIGHT * SizeFactor, 0.0f };

				GameScene::AddObject(BulletKind, OBJECT_STATE_AIR, 1, BULLET_DAMAGE, newPoint, newSize, newBoundingBoxSize, newMass, newFrictionFactor, newVelocity);
				currentShoot++;
			}
			// Play Sound (Bullet Shot)
			m_pSound->PlaySound(m_SoundIDs[SOUND_BOSS_SHOT], false, 0.7f);
			m_pSound->PlaySound(m_SoundIDs[SOUND_TEAR_FIRE], false, 1.0f);
			break;
		case BOSS_SHOOT_PATTERN_2:
			bossShootNum = 7 + rand() % 4; // 랜덤 발사 눈물 갯수
			currentShoot = 0;
			while (currentShoot < bossShootNum)
			{
				// 왼쪽 시야각으로 랜덤방향
				newVelocity.i = -cos(Radian((float)(rand() % 45)));
				if (!(rand() % 2))
					newVelocity.j = sin(Radian((float)(rand() % 45)));
				else
					newVelocity.j = -sin(Radian((float)(rand() % 45)));
				newVelocity = newVelocity * newAcceleration_Magnitude;

				// 랜덤 크기
				SizeFactor = 1.0f + rand() / (float)RAND_MAX;
				newSize = { BULLET_WIDTH * SizeFactor, BULLET_HEIGHT * SizeFactor, 0.0f };
				newBoundingBoxSize = { BULLET_BOUNDINGBOX_WIDTH * SizeFactor, BULLET_BOUNDINGBOX_HEIGHT * SizeFactor, 0.0f };

				GameScene::AddObject(BulletKind, OBJECT_STATE_AIR, 1, BULLET_DAMAGE, newPoint, newSize, newBoundingBoxSize, newMass, newFrictionFactor, newVelocity);
				currentShoot++;
			}
			// Play Sound (Bullet Shot)
			m_pSound->PlaySound(m_SoundIDs[SOUND_BOSS_SHOT], false, 0.7f);
			m_pSound->PlaySound(m_SoundIDs[SOUND_TEAR_FIRE], false, 1.0f);
			break;

		case BOSS_SHOOT_PATTERN_3:
			bossShootNum = 8;
			currentShoot = 0;
			while (currentShoot < bossShootNum)
			{
				// 8방향 방사진
				newVelocity.i = cos(Radian((float)(currentShoot * 45)));
				newVelocity.j = sin(Radian((float)(currentShoot * 45)));
				newVelocity = newVelocity * newAcceleration_Magnitude;
				GameScene::AddObject(BulletKind, OBJECT_STATE_AIR, 1, BULLET_DAMAGE, newPoint, newSize, newBoundingBoxSize, newMass, newFrictionFactor, newVelocity);
				// Play Sound (Bullet Shot)
				m_pSound->PlaySound(m_SoundIDs[SOUND_TEAR_FIRE], false, 0.7f);
				currentShoot++;
			}
			break;
		}
	}
}

void GameScene::ProcessCollision(int i, int j)
{
	if (i < 0 || j < 0)
	{
		std::cout << "You Try Underflow Accesss into ObjectArray!" << std::endl;
		if (i < 0) std::cout << "Accessing index: " << i << std::endl;
		if (j < 0) std::cout << "Accessing index: " << i << std::endl;
		return;
	}
	else if (i >= MAX_OBJECTS || j >= MAX_OBJECTS)
	{
		std::cout << "You Try Overflow Accesss into ObjectArray!" << std::endl;
		if (i >=MAX_OBJECTS) std::cout << "Accessing index: " << i << std::endl;
		if (j >=MAX_OBJECTS) std::cout << "Accessing index: " << i << std::endl;
		return;
	}
	else if (m_Objects[i] == NULL || m_Objects[j] == NULL)
	{
		std::cout << "Objects[" << i << "] OR Objects[" << j << "], one of these NULL!" << std::endl;
		return;
	}

	int i_kind = m_Objects[i]->GetKind();
	int j_kind = m_Objects[j]->GetKind();
	// 히어로와 보스의 충돌처리
	if ((i_kind == KIND_HERO_BODY && j_kind == KIND_BOSS) || (i_kind == KIND_BOSS && j_kind == KIND_HERO_BODY))
	{
		fBOX  BBox_Hero = m_Objects[HERO_BODY_ID]->GetBoudingBox(), BBox_Boss = m_Objects[BOSS_ID]->GetBoudingBox();
		fRECT BoundingRect_Hero = { BBox_Hero.Left, BBox_Hero.Top, BBox_Hero.Right, BBox_Hero.Bottom }, BoundingRect_Boss = { BBox_Boss.Left, BBox_Boss.Top, BBox_Boss.Right, BBox_Boss.Bottom };
		fRECT IntersectRECT;
		IntersectRect(&IntersectRECT, BoundingRect_Hero, BoundingRect_Boss);
		// 교차 영역 크기
		float nInterWidth = abs(IntersectRECT.Right - IntersectRECT.Left);
		float nInterHeight = abs(IntersectRECT.Top - IntersectRECT.Bottom);

		// Moved_BoundingRect: PlayerBoundRect
		// Hold_BoundingRect: BossBoundRect

		Vector newVelocity = m_Objects[HERO_BODY_ID]->GetVelocity();
		Point  newPosition = m_Objects[HERO_BODY_ID]->GetPosition();

		// 교차 너비 > 교차 높이: 위/아래에서부터 충돌이 이루어짐
		if (nInterWidth > nInterHeight)
		{
			// Hold_BoundingRect의 윗부분 충돌
			if (IntersectRECT.Top == BoundingRect_Boss.Top)
				newPosition.y += nInterHeight;
			// Hold_BoundingRect의 아랫부분 충돌
			else if (IntersectRECT.Bottom == BoundingRect_Boss.Bottom)
				newPosition.y -= nInterHeight;

			// y축 방향 속도값 0으로 초기화
			newVelocity.j = 0;
		}
		// 교차 너비 < 교차 높이: 좌/우로부터 충돌이 이루어짐
		else
		{
			// Hold_BoundingRect의 왼쪽부분 충돌
			if (IntersectRECT.Left == BoundingRect_Boss.Left)
				newPosition.x -= nInterWidth;
			// Hold_BoundingRect의 오른쪽부분 충돌
			else if (IntersectRECT.Right == BoundingRect_Boss.Right)
				newPosition.x += nInterWidth;

			// x축 방향 속도값 0으로 초기화
			newVelocity.i = 0;
		}

		m_Objects[HERO_BODY_ID]->SetPosition(newPosition);
		m_Objects[HERO_BODY_ID]->SetVelocity(newVelocity);

		// 피격당하는 딜레이 확인 후 체력 감소
		if (m_Objects[HERO_BODY_ID]->DealayComplete(KIND_HERO_DEALAY_HIT) == true)
		{
			int HP = m_Objects[HERO_BODY_ID]->GetHP();
			int Damage = m_Objects[BOSS_ID]->GetDamage();

			int newHP = (HP - Damage > 0) ? HP - Damage : 0;
			m_Objects[HERO_BODY_ID]->SetHP(newHP);

			m_Objects[HERO_BODY_ID]->DealayInit(KIND_HERO_DEALAY_HIT);

			int RandomHurtSoundIndex = SOUND_HERO_HURT_1 + rand() % (SOUND_HERO_HURT_3 - SOUND_HERO_HURT_1 + 1);
			m_pSound->PlaySound(m_SoundIDs[RandomHurtSoundIndex], false, 0.7f);
		}
	}
	// 히어로가 발사한 눈물과 보스와의 충돌처리
	else if ((i_kind == KIND_BULLET_NORMAL && j_kind == KIND_BOSS) || (i_kind == KIND_BOSS && j_kind == KIND_BULLET_NORMAL))
	{
		int HP = m_Objects[BOSS_ID]->GetHP();
		int Damage = (j_kind == KIND_BULLET_NORMAL) ? m_Objects[j]->GetDamage() : m_Objects[i]->GetDamage();
		Vector Velocity;
		if (j_kind == KIND_BULLET_NORMAL)
		{
			Velocity = m_Objects[j]->GetVelocity();
			m_Objects[j]->SetHP(0);
		}
		else
		{
			Velocity = m_Objects[i]->GetVelocity();
			m_Objects[i]->SetHP(0);
		}

		int newHP = (HP - Damage > 0) ? HP - Damage : 0;
		m_Objects[BOSS_ID]->SetHP(newHP);
		// 넉백
		Point newPosition = m_Objects[BOSS_ID]->GetPosition() + Velocity / RENDER_TRANSLATION_SCALE;
		m_Objects[BOSS_ID]->SetPosition(newPosition);
	}
	// 보스가 발사한 눈물과 히어로와의 충돌처리
	else if (i_kind == KIND_BULLET_BLOOD && j_kind == KIND_HERO_BODY || (i_kind == KIND_HERO_BODY && j_kind == KIND_BULLET_BLOOD))
	{
		// 피격당하는 딜레이 확인 후 체력 감소
		if (m_Objects[HERO_BODY_ID]->DealayComplete(KIND_HERO_DEALAY_HIT) == true)
		{
			int HP = m_Objects[HERO_BODY_ID]->GetHP();
			int Damage = (j_kind == KIND_BULLET_BLOOD) ? m_Objects[j]->GetDamage() : m_Objects[i]->GetDamage();
			Vector Velocity;
			if (j_kind == KIND_BULLET_BLOOD) Velocity = m_Objects[j]->GetVelocity();
			else Velocity = m_Objects[i]->GetVelocity();

			int newHP = (HP - Damage > 0) ? HP - Damage : 0;
			m_Objects[HERO_BODY_ID]->SetHP(newHP);

			// 넉백
			Point newPosition = m_Objects[HERO_BODY_ID]->GetPosition() + (Velocity / RENDER_TRANSLATION_SCALE) * 6;
			m_Objects[HERO_BODY_ID]->SetPosition(newPosition);

			m_Objects[HERO_BODY_ID]->DealayInit(KIND_HERO_DEALAY_HIT);

			int RandomHurtSoundIndex = SOUND_HERO_HURT_1 + rand() % (SOUND_HERO_HURT_3 - SOUND_HERO_HURT_1 + 1);
			m_pSound->PlaySound(m_SoundIDs[RandomHurtSoundIndex], false, 0.7f);
		}

		if (j_kind == KIND_BULLET_BLOOD) m_Objects[j]->SetHP(0);
		else m_Objects[i]->SetHP(0);
		// std::cout << "Hero(HP:" << newHP << ")" << std::endl;
	}
	// 보스레이드 시작 발판과 히어로와의 충돌처리
	else if (((i_kind == KIND_PRESSSURE_PLATE && j_kind == KIND_HERO_BODY) || (i_kind == KIND_HERO_BODY && j_kind == KIND_PRESSSURE_PLATE)) && m_StartBossRaid == false)
	{
		Point newTexturePos = m_Objects[i]->GetTextureSequenceCurrentPos();
		newTexturePos.x += 1.0f;
		if(i_kind == KIND_PRESSSURE_PLATE) m_Objects[i]->SetTextureSequenceCurrentPos(newTexturePos);
		else m_Objects[j]->SetTextureSequenceCurrentPos(newTexturePos);
		GameScene::CreateBoss();
		m_StartBossRaid = true;

		m_pSound->PlaySound(m_SoundIDs[SOUND_PRESSUREPALTE_PRESS], false, 1.0f);
	}
}

void GameScene::ProcessCollision()
{
	fBOX BoundingBox_i, BoundingBox_j;

	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		if (m_Objects[i] != NULL)
		{
			// Process Boundary Out
			if (i == HERO_BODY_ID || i == BOSS_ID)
				ProcessCollision_BoundaryOut(i);

			// Process Collision with other objects
			for (int j = i + 1; j < MAX_OBJECTS; ++j)
			{
				if (m_Objects[j] != NULL)
				{
					BoundingBox_i = m_Objects[i]->GetBoudingBox();
					BoundingBox_j = m_Objects[j]->GetBoudingBox();

					if (Collide_Box_AABB( BoundingBox_i.Left, BoundingBox_i.Bottom, BoundingBox_i.Back, BoundingBox_i.Right, BoundingBox_i.Top, BoundingBox_i.Front,
						                  BoundingBox_j.Left, BoundingBox_j.Bottom, BoundingBox_j.Back, BoundingBox_j.Right, BoundingBox_j.Top, BoundingBox_j.Front))
						ProcessCollision(i, j);
				}
			}
		}
	}
}

void GameScene::ProcessCollision_BoundaryOut(int id)
{
	if (id < 0)
	{
		std::cout << "You Try Underflow Accesss into ObjectArray!" << std::endl;
		std::cout << "Accessing index: " << id << std::endl;
		return;
	}
	else if (id >= MAX_OBJECTS)
	{
		std::cout << "You Try Overflow Accesss into ObjectArray!" << std::endl;
		std::cout << "Accessing index: " << id << std::endl;
		return;
	}
	else if (m_Objects[id] == NULL)
	{
		std::cout << "Objects[" << id << "] these NULL!" << std::endl;
		return;
	}

	// Check Boundary
	Point  Position = m_Objects[id]->GetPosition();
	fBOX   BoundingBox = m_Objects[id]->GetBoudingBox();
	fSIZE  BoundingBoxSize = m_Objects[id]->GetBoundingBoxSize();
	Vector Velocity = m_Objects[id]->GetVelocity();
	if (WORLD_BOUNDARY_LEFT > BoundingBox.Left)
	{
		Position.x = WORLD_BOUNDARY_LEFT + BoundingBoxSize.Width / 2;
		Velocity.i = 0.0f;
	}
	if (WORLD_BOUNDARY_RIGHT < BoundingBox.Right)
	{
		Position.x = WORLD_BOUNDARY_RIGHT - BoundingBoxSize.Width / 2;
		Velocity.i = 0.0f;
	}
	if (WORLD_BOUNDARY_TOP < BoundingBox.Top)
	{
		Position.y = WORLD_BOUNDARY_TOP - BoundingBoxSize.Height / 2;
		Velocity.j = 0.0f;
	}
	if (WORLD_BOUNDARY_BOTTOM > BoundingBox.Bottom)
	{
		Position.y += WORLD_BOUNDARY_BOTTOM - BoundingBox.Bottom;
		Velocity.j = 0.0f;
	}

	m_Objects[id]->SetPosition(Position);
	m_Objects[id]->SetVelocity(Velocity);
}

void GameScene::CreateBoss()
{
	m_pSound->StopAllSounds();
	m_pSound->PlaySound(m_SoundIDs[SOUND_BOSSRAID_BGM], true, 0.7f);
	// Initialize Boss Character Option
	Point  newPoint = Point(2.0f, 0.0f, 0.0f);
	fSIZE  newSize = { BOSS_WIDTH, BOSS_HEIGHT, 0.0f };
	fSIZE  newBoudingBoxSize = { BOSS_BOUNDINGBOX_WIDTH, BOSS_BOUNDINGBOX_HEIGHT, 0.0f };
	float  newMass = BOSS_MASS;
	float  nweFrictionFactor = BOSS_FRICTION_FACTOR;
	Vector newVelocity = { 0.0f, 0.0f, 0.0f };
	GameScene::AddObject(KIND_BOSS, OBJECT_STATE_GROUND, BOSS_INIT_HP, BULLET_DAMAGE, newPoint, newSize, newBoudingBoxSize, newMass, nweFrictionFactor, newVelocity);
}

void GameScene::BossPattern(float ElapsedTime)
{
	if (m_Objects[BOSS_ID] == NULL) return;
	else if(m_Objects[BOSS_ID]->GetHP() > 0)
	{
		switch (m_BossPattern)
		{
		case BOSS_PATTERN_JUMP:
			BossJump(ElapsedTime);
			break;
		case BOSS_PATTERN_HIGHJUMP:
			BossHighJump(ElapsedTime);
			break;
		case BOSS_PATTERN_SHOOT:
			BossShoot(ElapsedTime);
			break;
		}
	}
}

void GameScene::BossJump(float ElapsedTime)
{
	if (m_Objects[BOSS_ID] == NULL) return;
	Point  newPosition = m_Objects[BOSS_ID]->GetPosition();
	static Vector newVelocity;
	Point CurrnetSequencePos = m_Objects[BOSS_ID]->GetTextureSequenceCurrentPos();

	m_BossETime += ElapsedTime;

	if (m_BossPatternInit == true)
	{
		newVelocity = GameScene::BossGetTargetDirectirion() * BOSS_SPEED;
		m_BossPatternInit = false;
	}

	if (m_BossETime >= 0.0f && m_BossETime < 1.0f)
	{		
		CurrnetSequencePos.x = 7.0f;
		if (newVelocity.i <= 0)
			CurrnetSequencePos.y = 0.0f;
		else
			CurrnetSequencePos.y = 1.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);

		newPosition.z = -BOSS_JUMP_SPEED * (pow(m_BossETime,2) - m_BossETime + 0.25f) + 2;
		if (newPosition.z >= 0)
		{
			newVelocity.k = -BOSS_JUMP_SPEED * (2 * m_BossETime - 1);
			m_Objects[BOSS_ID]->SetVelocity(newVelocity);
			newPosition.x += (newVelocity.i * ElapsedTime * BOSS_SPEED);
			newPosition.y += (newVelocity.j * ElapsedTime * BOSS_SPEED);
		}
		else
		{
			m_Objects[BOSS_ID]->SetVelocity(Vector(0, 0, 0));
			newPosition.z = 0;
		}
	}
	else if (m_BossETime >= 1.0f && m_BossETime < 1.5f)
	{
		m_Objects[BOSS_ID]->SetVelocity(Vector(0, 0, 0));
		CurrnetSequencePos.x = 8.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);
		newPosition.z = 0;
	}
	else if (m_BossETime >= 1.5f && m_BossETime < 2.5f)
	{
		CurrnetSequencePos.x = 2.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);
		newPosition.z = 0;
	}
	else
	{
		m_BossETime = 0;
		m_BossPatternInit = true;
		m_BossPattern = rand() % KIND_BOSS_PATTERN;
	}

	m_Objects[BOSS_ID]->SetPosition(newPosition);
}

void GameScene::BossHighJump(float ElapsedTime)
{
	if (m_Objects[BOSS_ID] == NULL) return;
	Point  newPosition = m_Objects[BOSS_ID]->GetPosition();
	static Vector newVelocity;
	Point CurrnetSequencePos = m_Objects[BOSS_ID]->GetTextureSequenceCurrentPos();

	m_BossETime += ElapsedTime;

	if (m_BossETime >= 0.f && m_BossETime < 0.75f)
	{
		CurrnetSequencePos.x = 4.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);

		newVelocity = m_Objects[BOSS_ID]->GetVelocity();
		newVelocity.k = -BOSS_HIGHJUMP_SPEED * (2 * m_BossETime - 1.5f);

		m_Objects[BOSS_ID]->SetVelocity(newVelocity);
		newPosition.z = -BOSS_HIGHJUMP_SPEED * (m_BossETime * (m_BossETime - 1.5f));

		if (newPosition.z < 0)
		{
			m_Objects[BOSS_ID]->SetVelocity(Vector(0, 0, 0));
			newPosition.z = 0;
		}

		m_BossPatternInit = true;
	}
	else if (m_BossETime >= 0.75f && m_BossETime < 1.5f)
	{
		CurrnetSequencePos.x = 6.0f;

		newVelocity = m_Objects[BOSS_ID]->GetVelocity();
		newVelocity.k = -BOSS_HIGHJUMP_SPEED * (2 * m_BossETime - 1.5f);

		m_Objects[BOSS_ID]->SetVelocity(newVelocity);
		newPosition.z = -BOSS_HIGHJUMP_SPEED * (m_BossETime * (m_BossETime - 1.5f));

		if (m_BossPatternInit == true)
		{
			Point newTargetPosition = GameScene::BossGetTargetPosition();
			if (newTargetPosition.x < newPosition.x)
				CurrnetSequencePos.y = 0.0f;
			else
				CurrnetSequencePos.y = 1.0f;

			newPosition.x = newTargetPosition.x;
			newPosition.y = newTargetPosition.y;
			m_BossPatternInit = false;
		}

		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);
		if (newPosition.z < 0)
		{
			m_Objects[BOSS_ID]->SetVelocity(Vector(0, 0, 0));
			newPosition.z = 0;
		}
	}
	else if (m_BossETime >= 1.5f && m_BossETime < 1.7f)
	{
		CurrnetSequencePos.x = 5.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);
		m_Objects[BOSS_ID]->SetVelocity(Vector(0, 0, 0));
		newPosition.z = 0;

		m_BossPatternInit = true;
	}
	else if (m_BossETime >= 1.7f && m_BossETime < 2.2f)
	{
		CurrnetSequencePos.x = 8.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);
		newPosition.z = 0;

		if (m_BossPatternInit == true)
		{
			BulletShoot(KIND_BULLET_BLOOD, BOSS_SHOOT_PATTERN_3);
			m_BossPatternInit = false;
		}
	}
	else if (m_BossETime >= 2.2f && m_BossETime < 3.2f)
	{
		CurrnetSequencePos.x = 2.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);
		newPosition.z = 0;
	}
	else
	{
		m_BossPatternInit = true;
		m_BossETime = 0.0f;
		m_BossPattern = rand() % KIND_BOSS_PATTERN;
	}

	m_Objects[BOSS_ID]->SetPosition(newPosition);
}

void GameScene::BossShoot(float ElapsedTime)
{
	if (m_Objects[BOSS_ID] == NULL) return;
	Point CurrnetSequencePos = m_Objects[BOSS_ID]->GetTextureSequenceCurrentPos();

	m_BossETime += ElapsedTime;

	Point newTargetPosition = GameScene::BossGetTargetPosition();

	if (m_BossETime >= 0.f && m_BossETime < 0.25f)
	{
		CurrnetSequencePos.x = 1.0f;
		if (m_Objects[BOSS_ID]->GetPosition().x > newTargetPosition.x)
			CurrnetSequencePos.y = 0.0f;
		else
			CurrnetSequencePos.y = 1.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);

		m_BossPatternInit = true;
	}
	else if (m_BossETime >= 0.25f && m_BossETime < 0.75f)
	{
		CurrnetSequencePos.x = 3.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);

		if (m_BossETime >= 0.3f) {
			if (m_BossPatternInit == true) {
				if ((int)CurrnetSequencePos.y == 0)
					BulletShoot(KIND_BULLET_BLOOD, BOSS_SHOOT_PATTERN_2);
				if ((int)CurrnetSequencePos.y == 1)
					BulletShoot(KIND_BULLET_BLOOD, BOSS_SHOOT_PATTERN_1);
				m_BossPatternInit = false;
			}
		}
	}
	else if (m_BossETime >= 0.75f && m_BossETime < 1.5f)
	{
		CurrnetSequencePos.x = 2.0f;
		m_Objects[BOSS_ID]->SetTextureSequenceCurrentPos(CurrnetSequencePos);
	}
	else 
	{
		m_BossPatternInit = true;
		m_BossETime = 0.0f;
		m_BossPattern = rand() % KIND_BOSS_PATTERN;
	}
}

Vector GameScene::BossGetTargetDirectirion()
{
	Vector min_direction = { 100, 100, 100 };

	Point BossPosition = m_Objects[BOSS_ID]->GetPosition();
	BossPosition.z = 0;

	if (m_Objects[HERO_BODY_ID] != NULL)
	{
		Point TargetPosition = GameScene::BossGetTargetPosition();
		if (BossPosition.distanceTo(TargetPosition) < min_direction.magnitude())
			min_direction = TargetPosition - BossPosition;
	}

	return unit(min_direction);
}

Point GameScene::BossGetTargetPosition()
{
	Point TargetPosition;
	if (m_Objects[HERO_BODY_ID] != NULL) TargetPosition = m_Objects[HERO_BODY_ID]->GetPosition();
	return TargetPosition;
}

void GameScene::RenderUnderUI()
{
	if (m_StartBossRaid == false)
	{
		m_pRenderer->DrawTextureRect(0.0f, -100.0f, 0.0f, 653.0f, 122.0f, 1.0f, 1.0f, 1.0f, 1.0f, m_TextureIDs[KIND_RAID_START_INFO]);
	}
	else
	{
		if(m_Objects[BOSS_ID] != NULL && m_Objects[HERO_BODY_ID] == NULL)
			m_pRenderer->DrawTextureRect(0.0f, 0.0f, 0.0f, 545, 253, 1.0f, 1.0f, 1.0f, 1.0f, m_TextureIDs[KIND_GAME_FAIL]);
		else if(m_Objects[BOSS_ID] == NULL && m_Objects[HERO_BODY_ID] != NULL)
			m_pRenderer->DrawTextureRect(0.0f, 0.0f, 0.0f, 599, 259, 1.0f, 1.0f, 1.0f, 1.0f, m_TextureIDs[KIND_GAME_CLEAR]);
	}
}

void GameScene::RenderUI()
{
	Point Pos;
	fSIZE Size = { 0.0f,0.0f,0.0f };
	Point Pos_InTexture;

	// Draw BossHP
	if (m_StartBossRaid == true)
	{
		// Background HP Rect
		Pos.x = UI_BOSS_HP_RECT_POS_X;
		Pos.y = UI_BOSS_HP_RECT_POS_Y;
		Size.Width = UI_BOSS_HP_RECT_WIDTH;
		Size.Height = UI_BOSS_HP_RECT_HEIGHT;
		m_pRenderer->DrawSolidRect(
			Pos.x, Pos.y, 0.0f,
			Size.Width, Size.Height,
			69.0f / 255, 56.0f / 255, 83.0f / 255, 1.0f);

		int BossHP = (m_Objects[BOSS_ID] != NULL) ? m_Objects[BOSS_ID]->GetHP() : 0;
		Pos.x -= (UI_BOSS_HP_RECT_WIDTH / 2)* ((float)(BOSS_INIT_HP - BossHP) / BOSS_INIT_HP);
		Size.Width = UI_BOSS_HP_RECT_WIDTH * ((float)BossHP / BOSS_INIT_HP);

		static Point destPos = { UI_BOSS_HP_RECT_POS_X, UI_BOSS_HP_RECT_POS_Y };
		static fSIZE destSize = { UI_BOSS_HP_RECT_WIDTH, UI_BOSS_HP_RECT_HEIGHT, 0.0f };
		destPos.x += (Pos.x - destPos.x) * 0.01f;
		destPos.y = Pos.y;
		destSize.Width += (Size.Width - destSize.Width) * 0.01f;

		// Decrease HP Rect
		m_pRenderer->DrawSolidRect(
			destPos.x, destPos.y, 0.0f,
			destSize.Width, destSize.Height,
			236.0f / 255, 180.0f / 255, 84.0f / 255, 1.0f);

		// ForeGround HP Rect
		m_pRenderer->DrawSolidRect(
			Pos.x, Pos.y, 0.0f,
			Size.Width, Size.Height,
			241.0f / 255, 99.0f / 255, 83.0f / 255, 1.0f);

		// Boss Name
		m_pRenderer->DrawTextureRect(
			UI_BOSS_HP_RECT_POS_X, UI_BOSS_HP_RECT_POS_Y, 0.0f,
			UI_BOSS_HP_RECT_WIDTH * 0.3f, UI_BOSS_HP_RECT_HEIGHT,
			1.0f, 1.0f, 1.0f, 1.0f, m_TextureIDs[KIND_BOSS_NAME]);
	}

	// Draw HeroHP
	int HeroHP = (m_Objects[HERO_BODY_ID] != NULL) ? m_Objects[HERO_BODY_ID]->GetHP() : 0;
	float HeroHP_Cnt = (float)HeroHP / 2;
	for (int i = 0; i < (int)(HERO_INIT_HP / 2.0f + 0.5f); ++i)
	{
		Pos.x = UI_HERO_HEART_POS_X + (i * UI_HERO_HEART_WIDTH * m_TranslationScale);
		Pos.y = UI_HERO_HEART_POS_Y;
		Pos.z = 0;

		if (HeroHP_Cnt > 0.5f)
			Pos_InTexture.x = 0;
		else if (equal(HeroHP_Cnt, 0.5f))
			Pos_InTexture.x = 1;
		else
			Pos_InTexture.x = 2;
		--HeroHP_Cnt;

		m_pRenderer->DrawTextureRectHeightSeqXY
		(
			Pos.x, Pos.y, Pos.z,
			UI_HERO_HEART_WIDTH * m_TranslationScale, UI_HERO_HEART_HEIGHT * m_TranslationScale, // Size In World Coordination
			1.0f, 1.0f, 1.0f, 1.0f, // Render Color
			m_TextureIDs[KIND_HERO_HEART], // Texture ID
			(int)Pos_InTexture.x, 0, // Position In Texture Coordination
			MAX_HERO_HEART_ANIMATION_SEQUENCE_X, MAX_HERO_HEART_ANIMATION_SEQUENCE_Y // Max Sequence Count In Texture(Row, Column)
		);
	}
}

GameScene::GameScene()
{
	for (int i = 0; i < MAX_OBJECTS; ++i)
		m_Objects[i] = NULL;
	for (int i = 0; i < MAX_TEXTURE; ++i)
		m_TextureIDs[i] = -1;
	for (int i = 0; i < MAX_SOUNDS; ++i)
		m_SoundIDs[i] = -1;
}

GameScene::~GameScene()
{
	if (m_pRenderer)
	{
		for (int i = 0; i < MAX_TEXTURE; ++i)
		{
			if (m_TextureIDs[i] > -1)
				m_pRenderer->DeleteTexture(m_TextureIDs[i]);
		}
		delete m_pRenderer;
	}
	if (m_pSound)
	{
		for (int i = 0; i < MAX_SOUNDS; ++i)
		{
			if (m_SoundIDs[i] > -1)
				m_pSound->DeleteSound(m_SoundIDs[i]);
		}
		delete m_pSound;
	}
	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		if (m_Objects[i] != NULL) delete m_Objects[i];
	}
}

bool GameScene::InitialRenderer(int windowSizeX, int windowSizeY, float TranslationScale)
{
	if (m_pRenderer) delete m_pRenderer;
	m_pRenderer = new Renderer(windowSizeX, windowSizeY);

	m_TranslationScale = TranslationScale;
	m_TextureIDs[KIND_LAND] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Background.png");
	m_TextureIDs[KIND_PRESSSURE_PLATE] = m_pRenderer->CreatePngTexture("./Resource/Graphic/PressurePlate.png");
	m_TextureIDs[KIND_HERO_BODY] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Isaac_Body.png");
	m_TextureIDs[KIND_HERO_HEAD] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Isaac_Head.png");
	m_TextureIDs[KIND_HERO_DEADBODY] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Isaac_Dead.png");
	m_TextureIDs[KIND_BOSS] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Boss.png");
	m_TextureIDs[KIND_BULLET_NORMAL] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Tear.png");
	m_TextureIDs[KIND_BULLET_BLOOD] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Blood_Tear.png");
	m_TextureIDs[KIND_HERO_HEART] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Heart.png");
	m_TextureIDs[KIND_BOSS_NAME] = m_pRenderer->CreatePngTexture("./Resource/Graphic/Boss_Name.png");
	m_TextureIDs[KIND_GAME_FAIL] = m_pRenderer->CreatePngTexture("./Resource/Graphic/FAIL.png");
	m_TextureIDs[KIND_GAME_CLEAR] = m_pRenderer->CreatePngTexture("./Resource/Graphic/CLEAR.png");
	m_TextureIDs[KIND_RAID_START_INFO] = m_pRenderer->CreatePngTexture("./Resource/Graphic/RAID_START_INFO.png");

	return m_pRenderer->IsInitialized();
}

bool GameScene::InitialSound()
{
	if (m_pSound) delete m_pSound;
	m_pSound = new Sound();
	m_SoundIDs[SOUND_INTRO_BGM] = m_pSound->CreateSound("./Resource/Sound/BGM/TitleScreenIntro.mp3");
	m_SoundIDs[SOUND_BOSSRAID_BGM] = m_pSound->CreateSound("./Resource/Sound/BGM/BasicBossFight.mp3");
	m_SoundIDs[SOUND_HERO_HURT_1] = m_pSound->CreateSound("./Resource/Sound/SFX/Hero/HurtGrunt0.wav");
	m_SoundIDs[SOUND_HERO_HURT_2] = m_pSound->CreateSound("./Resource/Sound/SFX/Hero/HurtGrunt1.wav");
	m_SoundIDs[SOUND_HERO_HURT_3] = m_pSound->CreateSound("./Resource/Sound/SFX/Hero/HurtGrunt2.wav");
	m_SoundIDs[SOUND_HERO_DIES] = m_pSound->CreateSound("./Resource/Sound/SFX/Hero/IsaacDies.wav");
	m_SoundIDs[SOUND_BOSS_HIGHJUMP] = m_pSound->CreateSound("./Resource/Sound/SFX/Boss/BossHighJump.wav");
	m_SoundIDs[SOUND_BOSS_LANDING] = m_pSound->CreateSound("./Resource/Sound/SFX/Boss/BossLanding.wav");
	m_SoundIDs[SOUND_BOSS_SHOT] = m_pSound->CreateSound("./Resource/Sound/SFX/Boss/BossShot.wav");
	m_SoundIDs[SOUND_TEAR_FIRE] = m_pSound->CreateSound("./Resource/Sound/SFX/Tear/TearFire.wav");
	m_SoundIDs[SOUND_TEAR_BLOCK] = m_pSound->CreateSound("./Resource/Sound/SFX/Tear/TearBlock.wav");
	m_SoundIDs[SOUND_PRESSUREPALTE_PRESS] = m_pSound->CreateSound("./Resource/Sound/SFX/PressurePlate/ButtonPress.wav");

	m_pSound->PlaySound(m_SoundIDs[SOUND_INTRO_BGM], true, 0.7f);
	return true;
}

bool GameScene::InitialObjects()
{
	for (int i = 0; i < MAX_OBJECTS; ++i)
		m_Objects[i] = NULL;

	// Initialize PressurePlate Option
	Point  newPoint = Point(0.0f, -2.0f, 0.0f);
	fSIZE  newSize = { PRESSURE_PLATE_WIDTH, PRESSURE_PLATE_HEIGHT, 0.0f };
	fSIZE  newBoudingBoxSize = { PRESSURE_PLATE_BOUNDINGBOX_WIDTH, PRESSURE_PLATE_BOUNDINGBOX_HEIGHT, 0.0f };
	float  newMass = 0.0f;
	float  nweFrictionFactor = 0.0f;
	Vector newVelocity = { 0.0f, 0.0f, 0.0f };

	// Create PressurePlate
	GameScene::AddObject(KIND_PRESSSURE_PLATE, OBJECT_STATE_GROUND, 1, 0, newPoint, newSize, newBoudingBoxSize, newMass, nweFrictionFactor, newVelocity);

	// Initialize Player Character Option
	newPoint = Point(0.0f,0.0f,0.0f);
	newSize = { HERO_WIDTH, HERO_HEIGHT, 0.0f };
	newBoudingBoxSize = { HERO_BOUNDINGBOX_WIDTH, HERO_BOUNDINGBOX_HEIGHT, 0.0f };
	newMass = HERO_MASS;
	nweFrictionFactor = HERO_FRICTION_FACTOR;
	newVelocity = { 0.0f, 0.0f, 0.0f };

	// Createa Player Character
	GameScene::AddObject(KIND_HERO_BODY, OBJECT_STATE_GROUND, HERO_INIT_HP, 0, newPoint, newSize, newBoudingBoxSize, newMass, nweFrictionFactor, newVelocity);
	newPoint += Point(HERO_HEAD_OFFSET_X, HERO_HEAD_OFFSET_Y, 0.0f);
	GameScene::AddObject(KIND_HERO_HEAD, OBJECT_STATE_GROUND, HERO_INIT_HP, 0, newPoint, newSize, newBoudingBoxSize, newMass, nweFrictionFactor, newVelocity);

	return true;
}

void GameScene::Idle()
{
}

void GameScene::MouseInput(int button, int state, int x, int y)
{
	m_MouseButton = button;
	m_MouseState = state;
	m_MousePointX = x;
	m_MousePointY = y;
}

void GameScene::MouseOperation(float elapsedTime)
{
	switch (m_MouseButton)
	{
	case 0x0000: //GLUT_LEFT_BUTTON
		switch (m_MouseState)
		{
		case 0x0000://GLUT_DOWN
			break;
		case 0x0001://GLUT_UP
			break;
		}
		break;
	case 0x0001: //GLUT_MIDDLE_BUTTON
		switch (m_MouseState)
		{
		case 0x0000://GLUT_DOWN
			break;
		case 0x0001://GLUT_UP
			break;
		}
		break;
	case 0x0002: //GLUT_RIGHT_BUTTON
		switch (m_MouseState)
		{
		case 0x0000://GLUT_DOWN
			break;
		case 0x0001://GLUT_UP
			break;
		}
		break;
	}
}

void GameScene::KeyPressed(unsigned char key, int x, int y)
{
	m_KeyState[key] = true;
}

void GameScene::KeyUp(unsigned char key, int x, int y)
{
	m_KeyState[key] = false;
}

void GameScene::SpecialKeyPressed(int key, int x, int y)
{
	m_SpecialKeyState[key] = true;
}

void GameScene::SpecialKeyUp(int key, int x, int y)
{
	m_SpecialKeyState[key] = false;
}

void GameScene::KeyOperation(float elapsedTime)
{
	if (m_KeyState[27] == true && m_Objects[HERO_BODY_ID] == NULL)
		exit(1);
	if(m_Objects[HERO_BODY_ID] != NULL)
		m_Objects[HERO_BODY_ID]->ProcessKey(m_KeyState, elapsedTime);
}

void GameScene::SpecialKeyOperation(float elapsedTime)
{
	if (m_Objects[HERO_HEAD_ID] != NULL)
	{
		if (m_Objects[HERO_HEAD_ID]->DealayComplete(KIND_HERO_DEALAY_SHOOT) == true)
		{
			// 4방향으로 동시에 눈물발사되는 것을 막기 위한 방향키 스택 관리
			auto FindKey = [](std::list<int>* ArrowkeyStack, int SearchKey)
			{
				for (auto Arrowkey = ArrowkeyStack->begin(); Arrowkey != ArrowkeyStack->end(); ++Arrowkey)
				{
					if (*Arrowkey == SearchKey) return Arrowkey;
				}
				return ArrowkeyStack->end();
			};

			static bool SpecialKeyState[246];
			memcpy_s(SpecialKeyState, sizeof(bool) * 246, m_SpecialKeyState, sizeof(bool) * 246);
			// 0x0064 ~ 0x0067: GLUT_KEY_LEFT ~ GLUT_KEY_DOWN
			for (int i = 0x0064; i <= 0x0067; ++i)
			{
				auto SearchKey = FindKey(&m_KeyStack, i);
				if (m_SpecialKeyState[i] == true && SearchKey == m_KeyStack.end())
					m_KeyStack.emplace_back(i);
				else if (m_SpecialKeyState[i] != true && SearchKey != m_KeyStack.end())
					m_KeyStack.erase(SearchKey);

				SpecialKeyState[i] = false;
			}
			int ArrowKey = m_KeyStack.size() > 0 ? *(--m_KeyStack.end()) : -1;
			SpecialKeyState[ArrowKey] = true;

			m_Objects[HERO_HEAD_ID]->ProcessSpecialKey(SpecialKeyState, elapsedTime);
			if (SpecialKeyState[0x0065] == true) // 0x0065 : GLUT_KEY_UP
				BulletShoot(KIND_BULLET_NORMAL, SHOOT_UP);
			if (SpecialKeyState[0x0064] == true) // 0x0064 : GLUT_KEY_LEFT
				BulletShoot(KIND_BULLET_NORMAL, SHOOT_LEFT);
			if (SpecialKeyState[0x0067] == true) // 0x0067 : GLUT_KEY_DOWN
				BulletShoot(KIND_BULLET_NORMAL, SHOOT_DOWN);
			if (SpecialKeyState[0x0066] == true) // 0x0066 : GLUT_KEY_RIGHT
				BulletShoot(KIND_BULLET_NORMAL, SHOOT_RIGHT);

			m_Objects[HERO_HEAD_ID]->DealayInit(KIND_HERO_DEALAY_SHOOT);
		}
	}
}

void GameScene::UpdateScene(float elapsedTime)
{
	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		if (m_Objects[i] != NULL)
		{
			if(i != BOSS_ID) m_Objects[i]->Update(elapsedTime);
		}
	}
	if (m_Objects[BOSS_ID] != NULL) GameScene::BossPattern(elapsedTime);

	if (m_Objects[HERO_BODY_ID] != NULL && m_Objects[HERO_HEAD_ID] != NULL)
	{
		if (m_Objects[HERO_BODY_ID]->DealayComplete(KIND_HERO_DEALAY_HIT) == false)
		{
			if(m_Objects[HERO_BODY_ID]->DealayComplete(KIND_HERO_DEALAY_HIT_COLORCHANGE) == true)
				m_Objects[HERO_BODY_ID]->DealayInit(KIND_HERO_DEALAY_HIT_COLORCHANGE);
		}
		Point newPosition = m_Objects[HERO_BODY_ID]->GetPosition();
		COLOR newColor = m_Objects[HERO_BODY_ID]->GetColor();
		newPosition += Point(HERO_HEAD_OFFSET_X, HERO_HEAD_OFFSET_Y, 0.0f);
		m_Objects[HERO_HEAD_ID]->SetPosition(newPosition);
		m_Objects[HERO_HEAD_ID]->SetColor(newColor);
	}

	// 충돌 처리
	GameScene::ProcessCollision();

	// 가비지 컬렉트
	GameScene::DoGarbageCollect();
}

void GameScene::RendrScene()
{
	// Draw Background
	m_pRenderer->DrawTextureRect(0.0f, 0.0f, 0.0f, WND_WIDTH, WND_HEIGHT, 1.0f, 1.0f, 1.0f, 1.0f, m_TextureIDs[KIND_LAND]);

	// Draw UnderUI
	RenderUnderUI();

	Point  Position;
	fSIZE  Size;
	COLOR  Color;
	GLuint TextureID;
	Point  TextureSquanceMaxPos;
	Point  TextureSquanceCurrentPos;
	for (int i = 0; i < MAX_OBJECTS; ++i)
	{
		if (m_Objects[i] != NULL)
		{
			Position = m_Objects[i]->GetPosition();
			Size = m_Objects[i]->GetSize();
			Color = m_Objects[i]->GetColor();
			TextureID = m_Objects[i]->GetTextureID();
			TextureSquanceMaxPos = m_Objects[i]->GetTextureSequenceMaxPos();
			TextureSquanceCurrentPos = m_Objects[i]->GetTextureSequenceCurrentPos();

			// 월드 좌표계로 변환
			Position *= m_TranslationScale;
			Size.Width *= m_TranslationScale;
			Size.Height *= m_TranslationScale;
			Size.Depth *= m_TranslationScale;

			// Renderer
			m_pRenderer->DrawTextureRectHeightSeqXY
			(
				Position.x, Position.y, Position.z,
				Size.Width, Size.Height,
				Color.r, Color.g, Color.b, Color.a,
				TextureID,
				(int)TextureSquanceCurrentPos.x, (int)TextureSquanceCurrentPos.y,
				(int)TextureSquanceMaxPos.x, (int)TextureSquanceMaxPos.y
			);
		}
	}

	GameScene::RenderUI();
}

