#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "Camera.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "InputManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera()
	: nextPosition	 { 0.0f,0.0f,0.0f }
	, firstDirection { 0.0f,0.0f,0.0f }
	, nowPosition	 { 0.0f,0.0f,0.0f }
	, nextTarget	 { 0.0f,0.0f,0.0f }
	, nowTarget		 { 0.0f,0.0f,0.0f }
	, direction		 { 0.0f,0.0f,0.0f }
	, fov			 (0.0f)
	, length		 (0.0f)
	, horizonAngle	 (0.0f)
	, verticalAngle	 (0.0f)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{

}

/// <summary>
/// ������
/// </summary>
void Camera::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�g�p����json�f�[�^��萔�^�ɑ��*/
	const  float  NEAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["NEAR"];				  //��O�N���b�v����
	const  float  FAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["FAR"];					  //���N���b�v����
	const  float  FOV			  = json.GetJson(JsonManager::FileType::CAMERA)["FOV"];					  //field of view
	const  float  FIRST_LENGTH	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_LENGTH"];					  //���N���b�v����
	const  float  FIRST_ANGLE	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_ANGLE"];//�J�������W�I�t�Z�b�g
	vector<float> FIRST_DIRECTION = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"];//�J�������W�I�t�Z�b�g
	vector<float> TARGET_OFFSET   = json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"];//�J�������W�I�t�Z�b�g

	/*�����o�ϐ��̏�����*/
	this->nowTarget		  = player.GetRigidbody().GetPosition() + Convert(TARGET_OFFSET);		//�����_
	this->nextTarget	  = this->nowTarget;										//�����_
	this->direction		  = Convert(FIRST_DIRECTION);							//�J�����̌���
	this->length		  = FIRST_LENGTH;											//�����_����̋���
	this->firstDirection  = this->direction * -1.0f;								//���]������direction
	this->nowPosition	  = this->nowTarget + (this->firstDirection * this->length);//�J�������W
	this->nextPosition	  = this->nowPosition;										//�J�������W
	this->fov			  = FOV;													//field of view
	this->horizonAngle	  = FIRST_ANGLE;											//����������]�l
	this->verticalAngle	  = FIRST_ANGLE;											//����������]�l

	/*�J�����̎�O�N���b�v�����Ɖ��N���b�v������ݒ肷��*/
	SetCameraNearFar(NEAR_CLIP, FAR_CLIP);

	/*fov��ݒ肷��*/
	SetupCamera_Perspective(this->DegToRad(this->fov));
}

/// <summary>
/// �X�V
/// </summary>
void Camera::Update()
{

	/*�����_�̍X�V*/
	UpdateTarget();

	/*��]�l�̍X�V*/
	UpdateAngle();

	FixAngle();

	/*�����i�����j�̍X�V*/
	UpdateLength();

	/*FoV�̍X�V*/
	UpdateFoV();

	/*�J�����̌����̍X�V*/
	UpdateDirection();
	
	/*���W�̍X�V*/
	this->nowPosition = this->nowTarget + (this->direction * this->length);
	
	/*�J�����̍��W�A�����_��ݒ肷��i������͂x������Z�o�j*/
	SetCameraPositionAndTarget_UpVecY(this->nowPosition, this->nowTarget);
}

/// <summary>
/// �J�������W�̍X�V
/// </summary>
void Camera::UpdatePosition()
{

}

/// <summary>
/// �`��
/// </summary>
const void Camera::Draw()const
{
	//printfDx("CAMERA_POSITION X:%f,Y:%f,Z:%f\n", this->nowPosition.x, this->nowPosition.y, this->nowPosition.z);
	//printfDx("CAMERA_TARGET X:%f,Y:%f,Z:%f\n", this->nowTarget.x, this->nowTarget.y, this->nowTarget.z);
}

/// <summary>
/// �����_�̍X�V
/// </summary>
void Camera::UpdateTarget()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�v���C���[�̍��W�ɕ␳�l�𑫂��Ē����_�Ƃ���*/
	this->nextTarget = player.GetRigidbody().GetPosition() + Convert(json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"]);

	this->nowTarget = Lerp(this->nowTarget, this->nextTarget, VGet(0.3f, 0.3f, 0.3f));

}

/// <summary>
/// �J������]�l�̍X�V
/// </summary>
void Camera::UpdateAngle()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager>::GetInstance();//�C���v�b�g�}�l�[�W���[
	auto& json  = Singleton<JsonManager>::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�ϐ��̏���*/
	const int	X_BUF			= input.GetRStickState().XBuf;										  //�E�X�e�B�b�N����
	const int	Y_BUF			= input.GetRStickState().YBuf;										  //�E�X�e�B�b�N����
	const float KEY_BUF			= json.GetJson(JsonManager::FileType::CAMERA)["KEY_INPUT_BUF"]; //�L�[����
	const float ANGLE_MULTIPLY	= json.GetJson(JsonManager::FileType::CAMERA)["ANGLE_MULTIPLY"];//�A���O���{��
		  float horizonAngle	= 0.0f;																  //������]�l
		  float verticalAngle	= 0.0f;																  //������]�l
		  float	addAngle		= 0.0f;
		addAngle = json.GetJson(JsonManager::FileType::CAMERA)["ADD_ANGLE"];	//�A���O��������
	/*�����X�e�B�b�N���͂��L�[���͂��Ȃ����return��Ԃ�*/
	if (Y_BUF == 0 && X_BUF == 0 && CheckHitKeyAll(DX_CHECKINPUT_KEY) == 0)return;
	
	/*Y+:down Y-:up X+:right X-:left*/
	/*���E�̃X�e�B�b�N���͂܂��͂������*/
	if (X_BUF != 0 || CheckHitKey(KEY_INPUT_RIGHT) != 0 || CheckHitKey(KEY_INPUT_LEFT) != 0)
	{
		//�X�e�B�b�N���͂��Ȃ���Β萔������
		if (X_BUF == 0)
		{
			horizonAngle = KEY_BUF;
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				horizonAngle *= -1.0f;
			}
		}
		//�X�e�B�b�N���͂�����΂��̒l������
		else
		{
			horizonAngle = static_cast<float>(X_BUF);
		}
		this->horizonAngle += horizonAngle * ANGLE_MULTIPLY;
	}

	/*�㉺�̓��͂������*/
	if (Y_BUF != 0 || CheckHitKey(KEY_INPUT_DOWN) != 0 || CheckHitKey(KEY_INPUT_UP) != 0)
	{
		//�X�e�B�b�N���͂��Ȃ���Β萔������
		if (Y_BUF == 0)
		{
			verticalAngle = KEY_BUF;
			if (CheckHitKey(KEY_INPUT_UP))
			{
				verticalAngle *= -1.0f;
			}
		}
		//�X�e�B�b�N���͂�����΂��̒l������
		else
		{
			verticalAngle = static_cast<float>(Y_BUF);
		}
		this->verticalAngle += verticalAngle * ANGLE_MULTIPLY;
	}
}

/// <summary>
/// ��]�l�̕␳
/// </summary>
void Camera::FixAngle()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();//json

	/*�g�p����ϐ��̏���*/
	const float MAX_HORIZON_ANGLE  = json.GetJson(JsonManager::FileType::CAMERA)["MAX_HORIZON_ANGLE"]; //�����_�܂ł̍ő咷��
	const float MIN_HORIZON_ANGLE  = json.GetJson(JsonManager::FileType::CAMERA)["MIX_HORIZON_ANGLE"]; //�����_�܂ł̍ŏ�����
	const float MAX_VERTICAL_ANGLE = json.GetJson(JsonManager::FileType::CAMERA)["MAX_VERTICAL_ANGLE"];//�����_�܂ł̍ő咷��
	const float MIN_VERTICAL_ANGLE = json.GetJson(JsonManager::FileType::CAMERA)["MIX_VERTICAL_ANGLE"];//�����_�܂ł̍ŏ�����

	/*�A���O����␳����*/
	if (this->horizonAngle > MAX_HORIZON_ANGLE || this->horizonAngle < MIN_HORIZON_ANGLE)
	{
		this->horizonAngle = 0.0f;
	}
	if (this->verticalAngle > MAX_VERTICAL_ANGLE)
	{
		this->verticalAngle = MAX_VERTICAL_ANGLE;
	}
	else if (this->verticalAngle < MIN_VERTICAL_ANGLE)
	{
		this->verticalAngle = MIN_VERTICAL_ANGLE;
	}
}

/// <summary>
/// �����_����J�������W�܂ł̒����̍X�V
/// </summary>
void Camera::UpdateLength()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�g�p����ϐ��̏���*/
	float maxLength = 0.0f;
		maxLength = json.GetJson(JsonManager::FileType::CAMERA)["MAX_LENGTH"];	//�A���O��������

	/*�����̕␳*/
	this->length = Lerp(this->length, maxLength, 0.05f);
}

/// <summary>
/// �J�����̌����̍X�V
/// </summary>
void Camera::UpdateDirection()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();//json

	/*�e��]�l�����Ƃɂ�����]�s����o��*/
	MATRIX horizonRotationMatrix = MGetRotY(this->horizonAngle);
	MATRIX verticalRotationMatrix = MGetRotZ(this->verticalAngle);
	VECTOR firstDirection = Convert(json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"]);

	/*�ŏ��̌����x�N�g������]�������݂̌����x�N�g�����o��*/
	this->direction = VTransform(firstDirection, MMult(verticalRotationMatrix, horizonRotationMatrix));
}

/// <summary>
/// FoV�̍X�V
/// </summary>
void Camera::UpdateFoV()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();//player

	/*�萔�^�ɑ��*/
	const float FOV		  = json.GetJson(JsonManager::FileType::CAMERA)["FOV"];
	const float MOVED_FOV = json.GetJson(JsonManager::FileType::CAMERA)["MOVED_FOV"];
	const float PERCENT   = json.GetJson(JsonManager::FileType::CAMERA)["FOV_PERCENT"];

	/*�v���C���[���ړ���Ԃ�������*/
	//if (player.IsRun())
	//{
	//	this->fov = Lerp(this->fov, MOVED_FOV, PERCENT);
	//}
	///*�ړ����Ă��Ȃ�������*/
	//else
	//{
		this->fov = Lerp(this->fov, FOV, PERCENT);
	//}

	float fov = this->fov * (DX_PI_F / 180.0f);

	/*fov��ݒ肷��*/
	SetupCamera_Perspective(fov);
}

float Camera::Lerp(const float _start,const float _end,const float _percent)
{
	return _start + _percent * (_end - _start);
}
VECTOR Camera::Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
{
	VECTOR out = { 0.0f,0.0f,0.0f };
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}