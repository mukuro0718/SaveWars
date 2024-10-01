#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "PlayerData.h"
#include "AttackData.h"
#include "PlayerAttackData.h"
#include "CharacterColliderData.h"
#include "HitStop.h"

CharacterColliderData::CharacterColliderData(const ColliderData::Priority _priority, const GameObjectTag _tag, CharacterData* _data)
	: ColliderData(ColliderData::Kind::CHARACTER_CAPSULE, _priority, _tag)
	, radius(0.0f)
	, topPositon(VGet(0.0f, 0.0f, 0.0f))
	, data(_data)
{
	//�����Ȃ�
}

const CharacterData& CharacterColliderData::GetCharacterData()const
{
	return *data;
}

void CharacterColliderData::OnHit(const AttackData& _data, const VECTOR _attackPosition)
{
	auto& hitStop = Singleton<HitStop>::GetInstance();

	if (!this->data->isCutDamage)
	{
		this->data->hp -= _data.damage;
		hitStop.SetHitStopTime(_data.hitStopTime);
	}
	this->data->isHit = true;
}

void CharacterColliderData::SetPlayerReaction(const int _type)
{
	auto& playerData = dynamic_cast<PlayerData&>(*this->data);

	playerData.playerReaction = _type;
}