#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerRunOutOfStamina.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerRunOutOfStamina::PlayerRunOutOfStamina()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerRunOutOfStamina::~PlayerRunOutOfStamina()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerRunOutOfStamina::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerRunOutOfStamina::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerRunOutOfStamina::Update(Player& _player)
{
	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	auto& json = Singleton<JsonManager>::GetInstance();
	MoveData data;
	data.Set(Gori::ORIGIN, json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"], false, true);
	Move(_player, data);

	/*�X�^�~�i�̌v�Z*/
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]);
	
	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::RUN_FRONT);
	float playTime = 0.3f;
	_player.PlayAnimation(nextAnimation, playTime);
}