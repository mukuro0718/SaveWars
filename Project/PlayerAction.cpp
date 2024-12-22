#include <DxLib.h>
#include <math.h>
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "UseJson.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "PlayerAction.h"
#include "InputManager.h"
#include "CameraManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerAction::PlayerAction()
	: isEndAction	(false)
	, isChangeAction(false)
{

}

/// <summary>
/// ��]���̍X�V
/// </summary>
void PlayerAction::UpdateRotation(const bool isSkip, VECTOR& _nextRotationation, VECTOR& _nowRotationation)
{
	if (!isSkip)
	{
		/*�X�e�B�b�N���͂��擾*/
		auto& input = Singleton<InputManager>::GetInstance();
		VECTOR lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
		//���K��
		lStick = VNorm(lStick);

		/*�X�e�B�b�N���͂��Ȃ���Α������^�[��*/
		if (lStick.x == 0.0f && lStick.z == 0.0f) return;

		/*�J�����̌������擾*/
		auto& camera = Singleton<CameraManager>::GetInstance();
		VECTOR cameraDirection = VNorm(camera.GetCameraDirection());

		/*�J�����̌����ƃX�e�B�b�N�̓��͂����Ƃɉ�]�����o��*/
		_nextRotationation = Gori::ORIGIN;
		_nextRotationation.y = static_cast<float>(
			-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
			- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
		/*���݂̉�]�������[�v�ŕ⊮���ďo��*/
		auto& json = Singleton<JsonManager>::GetInstance();
		VECTOR lerpValue = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATION_LERP_VALUE"]);
		_nowRotationation = Gori::LerpAngle(_nowRotationation, _nextRotationation, lerpValue);
	}
}

/// <summary>
/// �ړ����x�̍X�V
/// </summary>
void PlayerAction::UpdateSpeed(float& _nowSpeed, const float _maxSpeed, const VECTOR _nowRotation, const VECTOR _nextRotation)
{
	/*�ő呬�x����*/
	float max_speed = _maxSpeed;

	/*���̉�]���ƍ��̉�]���̍����萔�ȏ�Ȃ猸��������*/
	//float nowDeg = (_nowRotation.y * 180.0f / DX_PI_F);
	//float nextDeg = (_nextRotation.y * 180.0f / DX_PI_F);
	//const float PI_2 = 360.0f;
	//if (nextDeg < 0.0f) { nextDeg += PI_2; }
	//float diff = nowDeg - nextDeg;
	//if (diff < 0.0f) { diff *= -1.0f; }
	//if (diff >= 180.0f)
	//{
	//	max_speed = 0.0f;
	//}

	/*�V�������x���o��*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (max_speed != 0)
	{
		_nowSpeed += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"]);
		//�ő呬�x�𒴂��Ȃ��悤�ɒ�������
		if (_nowSpeed >= max_speed)
		{
			_nowSpeed = max_speed;
		}
	}
	else
	{
		_nowSpeed += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["DECEL"]);
		//�O�ȉ��ɂȂ�Ȃ��悤�ɒ�������
		if (_nowSpeed <= 0)
		{
			_nowSpeed = 0;
		}
	}
}

/// <summary>
/// �ړ��x�N�g���̍X�V
/// </summary>
VECTOR PlayerAction::UpdateVelocity(const VECTOR _rotation, const VECTOR _prevVelocity, const float _speed, const bool _isLerp)
{
	/*�ړ�����������o��*/
	VECTOR direction = VGet(-sinf(_rotation.y), 0.0f, -cosf(_rotation.y));
	direction = VNorm(direction);

	/*�V�����ړ��x�N�g�����o��*/
	VECTOR ainVelocity = VScale(direction, _speed);
	VECTOR prevVelcity = _prevVelocity;
	VECTOR newVelocity = VGet(ainVelocity.x, prevVelcity.y, ainVelocity.z);

	/*�␳�t���O�������Ă�����␳����*/
	if (_isLerp)
	{
		auto& json = Singleton<JsonManager>::GetInstance();
		VECTOR lerpValue = Gori::Convert(json.GetJson(JsonManager::FileType::PLAYER)["VELOCITY_LERP_VALUE"]);
		newVelocity = Gori::LerpVECTOR(prevVelcity, ainVelocity, lerpValue);
	}
	return newVelocity;
}

/// <summary>
/// �ړ�����
/// </summary>
void PlayerAction::Move(Player& _player, MoveData& _set)
{
	/*��]���̍X�V*/
	VECTOR nowRotation = _player.GetRigidbody().GetRotation();
	UpdateRotation(_set.isSkip, _set.nextRotation, nowRotation);
	_player.SetRotation(nowRotation, _set.nextRotation);

	/*�ړ����x�̍X�V*/
	float nowSpeed = _player.GetSpeed();
	UpdateSpeed(nowSpeed, _set.maxSpeed, nowRotation, _set.nextRotation);
	_player.SetSpeed(nowSpeed);

	/*�ړ��x�N�g�����o��*/
	VECTOR nowVelocity = _player.GetRigidbody().GetVelocity();
	VECTOR newVelocity = UpdateVelocity(nowRotation, nowVelocity, nowSpeed, _set.isLerp);
	_player.SetVelocity(newVelocity);
}