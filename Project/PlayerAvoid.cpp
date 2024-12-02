#include <DxLib.h>
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerAvoid.h"
#include "UseJson.h"
#include "CharacterData.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerAvoid::PlayerAvoid()
	: PlayerAction	()
	, isPlay		(false)
	, frameCount	(0)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerAvoid::~PlayerAvoid()
{

}

/// <summary>
/// ������
/// </summary>
void PlayerAvoid::Initialize()
{
	this->isPlay = false;
	this->isChangeAction = false;
	this->isEndAction = false;
	this->frameCount = 0;
}

/// <summary>
/// �㏈��
/// </summary>
void PlayerAvoid::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
void PlayerAvoid::Update(Player& _player)
{
	/*�J�n����̏���*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (this->frameCount == 0)
	{
		//���G�t���O�𗧂Ă�
		_player.GetPlayerData().isInvinvible = true;
		//�X�^�~�i�̌v�Z
		_player.CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]);
		//����������
		_player.SetSpeed(json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"]);
	}
	else
	{
		//�A�j���[�V�������I�����Ă��邩
		if (_player.GetIsChangeAnimation())
		{
			this->isEndAction = true;
		}
	}

	/*�t���[���̌v��*/
	bool isRotate = true;
	//�t���[���J�E���g�̑���
	this->frameCount++;
	//�A�N�V�����L�����Z�����\��������
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"])
	{
		isRotate = false;
	}
	//���G����
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"])
	{
		_player.GetPlayerData().isInvinvible = false;
	}

	/*�ړ������i�ړ������Ȃ��ꍇ�ł��A���x�̌���������̂ŏ������s���j*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, isRotate, false);
	Move(_player, data);

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::AVOID);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}