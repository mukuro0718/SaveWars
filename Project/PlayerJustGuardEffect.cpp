#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "PlayerJustGuardEffect.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerJustGuardEffect::PlayerJustGuardEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// �G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�
/// </summary>
void PlayerJustGuardEffect::SetInfoToPlayTheEffect()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	VECTOR position = player.GetRigidbody().GetPosition();
	position.y += json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_JUST_GUARD_Y_OFFSET"];
	VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_JUST_GUARD_SCALE"]);
	VECTOR rotation = player.GetRigidbody().GetRotation();

	this->transform->SetPosition(position);
	this->transform->SetScale(scale);
	this->transform->SetRotationOfRadian(rotation);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_JUST_GUARD_START_FRAME"];
	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_JUST_GUARD_END_FRAME"];
}


