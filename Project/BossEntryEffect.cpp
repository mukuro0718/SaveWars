#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BossEntryEffect.h"
#include "EnemyManager.h"

BossEntryEffect::BossEntryEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// �G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�
/// </summary>
void BossEntryEffect::SetInfoToPlayTheEffect()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	//this->transform->SetPosition(enemy.GetRigidbody().GetPosition());
	this->transform->SetScale(Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_SCALE"]));
	this->transform->SetRotation(enemy.GetRigidbody().GetRotation());

	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_START_FRAME"];
	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_ENTRY_END_FRAME"];
}

