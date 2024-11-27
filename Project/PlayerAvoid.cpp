#include <DxLib.h>
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
	: PlayerAction()
	, isPlay (false)
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

	/*�t���[���̌v��*/
	//�t���[���J�E���g�̑���
	this->frameCount++;
	//�A�j���[�V�������I�����Ă��邩����L�����Z�����\��������
	if (_player.GetIsChangeAnimation() || this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"])
	{
		this->isChangeAction = true;
	}
	//���G����
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"])
	{
		_player.GetPlayerData().isInvinvible = false;
	}

	/*��]�����o��*/
	VECTOR nextRotation = _player.GetNextRotation();
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(nextRotation, nowRotation);
	_player.SetRotation(nowRotation, nextRotation);

	/*�ړ����x���o��*/
	float maxSpeed = 0.0f;
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, maxSpeed, nowRotation, nextRotation);
	_player.SetSpeed(nowSpeed);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelcity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelcity, nowSpeed, false);
	_player.SetVelocity(newVelocity);

	/*�A�j���[�V�����̍Đ�*/
	int nextAnimation = static_cast<int>(Player::AnimationType::RUN_FRONT);
	int playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	_player.PlayAnimation(nextAnimation, playTime);
}