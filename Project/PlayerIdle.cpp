#include <DxLib.h>
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "PlayerIdle.h"
#include "UseJson.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerIdle::PlayerIdle()
	: PlayerAction()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerIdle::~PlayerIdle()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerIdle::Initialize()
{

}

/// <summary>
/// �㏈��
/// </summary>
void PlayerIdle::Finalize()
{

}

/// <summary>
/// �X�V
/// </summary>
void PlayerIdle::Update(Player& _player)
{
	/*�A�j���[�V�����̍Đ�*/
	auto& json = Singleton<JsonManager>::GetInstance();
	int nextAnimation = static_cast<int>(Player::AnimationType::IDLE);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}