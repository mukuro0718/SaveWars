#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerWalk.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerWalk::PlayerWalk()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerWalk::~PlayerWalk()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerWalk::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerWalk::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerWalk::Update(Player& _player)
{
	this->isEndAction = true;

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	auto& json = Singleton<JsonManager>::GetInstance();
	MoveData data;
	data.Set(Gori::ORIGIN, json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"], false, false);
	Move(_player, data);

	/*�X�^�~�i�̉�*/
	_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::WALK_FRONT);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}