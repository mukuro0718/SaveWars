#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "SphereColliderData.h"
#include "Character.h"
#include "Player.h"
#include "Camera.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "InputManager.h"
#include "Debug.h"
#include "SceneChanger.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera()
	: collider	(nullptr)
	, nextTarget(Gori::ORIGIN)
	, nowTarget	(Gori::ORIGIN)
	, direction	(Gori::ORIGIN)
	, fov		(0.0f)
	, length	(0.0f)
	, yow		(0.0f)
	, pitch		(0.0f)
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���C�_�[�f�[�^�̍쐬*/
	this->collider = new SphereColliderData(ColliderData::Priority::LOW, GameObjectTag::CAMERA);
	this->collider->rigidbody.SetUseGravity(false);
	auto& collider = dynamic_cast<SphereColliderData&>(*this->collider);
	collider.radius = json.GetJson(JsonManager::FileType::CAMERA)["RADIUS"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{
	DeleteMemberInstance(this->collider);
}

/// <summary>
/// ������
/// </summary>
void Camera::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json   = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy	 = Singleton<EnemyManager>::GetInstance();

	/*�g�p����json�f�[�^��萔�^�ɑ��*/
	const  float  NEAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["NEAR"];				  //��O�N���b�v����
	const  float  FAR_CLIP		  = json.GetJson(JsonManager::FileType::CAMERA)["FAR"];					  //���N���b�v����
	const  float  FOV			  = json.GetJson(JsonManager::FileType::CAMERA)["FOV"];					  //field of view
	const  float  FIRST_LENGTH	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_LENGTH"];					  //���N���b�v����
	const  float  FIRST_ANGLE	  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_ANGLE"];//�J�������W�I�t�Z�b�g
	vector<float> FIRST_DIRECTION = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"];//�J�������W�I�t�Z�b�g
	vector<float> TARGET_OFFSET   = json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"];//�J�������W�I�t�Z�b�g
	vector<float> FIRST_POSITION  = json.GetJson(JsonManager::FileType::CAMERA)["FIRST_POSITION"];//�J�������W�I�t�Z�b�g


	/*�����o�ϐ��̏�����*/
	this->nowTarget			 = enemy.GetRigidbody().GetPosition() + Gori::Convert(TARGET_OFFSET);	//�����_
	this->nextTarget		 = this->nowTarget;												//�����_
	this->direction			 = Gori::Convert(FIRST_DIRECTION);									//�J�����̌���
	this->length			 = FIRST_LENGTH;												//�����_����̋���
	this->fov				 = FOV;															//field of view
	this->yow				 = 0.0f;														//���[
	this->pitch				 = 0.0f;														//�s�b�`
	this->collider->rigidbody.SetPosition(this->nowTarget + (this->direction * this->length));	//�J�������W
	this->collider->rigidbody.SetPosition(Gori::Convert(FIRST_POSITION));

	/*�J�����̎�O�N���b�v�����Ɖ��N���b�v������ݒ肷��*/
	SetCameraNearFar(NEAR_CLIP, FAR_CLIP);

	/*fov��ݒ肷��*/
	SetupCamera_Perspective(DegToRad(this->fov));
}

/// <summary>
/// �X�V
/// </summary>
void Camera::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json	 = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�����_�̍X�V*/
	UpdateTarget();

	/*�����̍X�V*/
	UpdateLength();

	/*�ړ��x�N�g���̍X�V*/
	UpdateVelocity();

	/*FoV�̍X�V*/
	UpdateFoV();

	/*�J�����̌����̍X�V*/
	UpdateDirection();		
}

/// <summary>
/// �`��
/// </summary>
const void Camera::Draw()const
{
	/*�J�����̍��W�A�����_��ݒ肷��i������͂x������Z�o�j*/
	SetCameraPositionAndTarget_UpVecY(this->collider->rigidbody.GetPosition(), this->nowTarget);

	auto& debug = Singleton<Debug>::GetInstance();
	if (debug.IsShowDebugInfo(Debug::ItemType::CAMERA))
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		printfDx("CAMERA_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		printfDx("CAMERA_TARGET X:%f,Y:%f,Z:%f\n", this->nowTarget.x, this->nowTarget.y, this->nowTarget.z);
		printfDx("CAMERA_DIRECTION X:%f,Y:%f,Z:%f\n", this->direction.x, this->direction.y, this->direction.z);
		printfDx("CAMERA_LENGTH:%f\n", this->length);
	}
}

/// <summary>
/// �����_�̍X�V
/// </summary>
void Camera::UpdateTarget()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& sceneChanger = Singleton<SceneChanger>::GetInstance();

	/*�V�[���̏�ԂŎ��̒����_��ς���*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		this->nextTarget = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TITLE_TARGET"]);
		break;
	case SceneChanger::SceneType::GAME:
		if (player.GetHP() < 0 || !player.GetIsLockOn())
		{
			//�����_�I�t�Z�b�g
			const VECTOR TARGET_OFFSET = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TARGET_OFFSET"]);
			this->nextTarget = VAdd(player.GetRigidbody().GetPosition(), TARGET_OFFSET);
		}
		else
		{
			this->nextTarget = enemy.GetRigidbody().GetPosition();
		}
		break;
	case SceneChanger::SceneType::GAME_OVER:
		this->nextTarget = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TITLE_TARGET"]);
		break;
	case SceneChanger::SceneType::GAME_CLEAR:
		this->nextTarget = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["TITLE_TARGET"]);
		break;
	}

	/*���݂̒����_���o��*/
	const VECTOR LERP_PERCENT = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_TARGET"]);	//���`�⊮�̃p�[�Z���g
	this->nowTarget = Lerp(this->nowTarget, this->nextTarget, LERP_PERCENT);
}

/// <summary>
/// �J�������W�̍X�V
/// </summary>
void Camera::UpdateAngle()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input  = Singleton<InputManager>	::GetInstance();//�C���v�b�g�}�l�[�W���[
	auto& json   = Singleton<JsonManager>	::GetInstance();//json
	auto& player = Singleton<PlayerManager>::GetInstance();//json

	/*�ϐ��̏���*/
	const int	X_BUF		= input.GetRStickState().XBuf;								//�E�X�e�B�b�N����
	const int	Y_BUF		= input.GetRStickState().YBuf;								//�E�X�e�B�b�N����
	const float ADD_ANGLE	= json.GetJson(JsonManager::FileType::CAMERA)["ADD_ANGLE"];	//�A���O��������
	const float MAX_YOW		= json.GetJson(JsonManager::FileType::CAMERA)["MAX_YOW"];	//�A���O��������
	const float MIN_YOW		= json.GetJson(JsonManager::FileType::CAMERA)["MIN_YOW"];	//�A���O��������
	const float MAX_PITCH	= json.GetJson(JsonManager::FileType::CAMERA)["MAX_PITCH"];	//�A���O��������
	const float MIN_PITCH	= json.GetJson(JsonManager::FileType::CAMERA)["MIN_PITCH"];	//�A���O��������

	/*�A���O���̍X�V Y+:down Y-:up X+:right X-:left*/
	//�X�e�B�b�N���͂��L�[���͂��Ȃ����
	if (Y_BUF != 0 || X_BUF != 0 || CheckHitKeyAll(DX_CHECKINPUT_KEY) != 0)
	{
		//���E�̃X�e�B�b�N���͂������
		if (X_BUF != 0)
		{
			if (X_BUF < 0)
			{
				this->yow += ADD_ANGLE;
			}
			else if (X_BUF > 0)
			{
				this->yow -= ADD_ANGLE;
			}
		}
		//�㉺�̃X�e�B�b�N���͂������
		if (Y_BUF != 0)
		{
			if (Y_BUF < 0)
			{
				this->pitch -= ADD_ANGLE;
			}
			else if (Y_BUF > 0)
			{
				this->pitch += ADD_ANGLE;
			}
		}
	}
	
	/*�A���O�����͈͂��o�Ȃ��悤�ɂ���*/
	if (this->pitch < MIN_PITCH)
	{
		this->pitch = MIN_PITCH;
	}
	else if (this->pitch > MAX_PITCH)
	{
		this->pitch = MAX_PITCH;
	}

	if (this->yow < MIN_YOW)
	{
		this->yow += MAX_YOW;
	}
	else if (this->yow > MAX_YOW)
	{
		this->yow -= MIN_YOW;
	}
}

/// <summary>
/// �����̍X�V
/// </summary>
void Camera::UpdateLength()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json			= Singleton<JsonManager>::GetInstance();
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>::GetInstance();

	float maxLength = 0.0f;
	float minLength = 0.0f;

	/*�V�[���̏�Ԃŏ�����ς���*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		maxLength = json.GetJson(JsonManager::FileType::CAMERA)["TITLE_MAX_LENGTH"];
		minLength = json.GetJson(JsonManager::FileType::CAMERA)["TITLE_MIN_LENGTH"];
		break;
	case SceneChanger::SceneType::GAME:
		if (player.GetHP() < 0 || enemy.GetHP() < 0)
		{
			maxLength = json.GetJson(JsonManager::FileType::CAMERA)["DEATH_MAX_LENGTH"];
			minLength = json.GetJson(JsonManager::FileType::CAMERA)["DEATH_MIN_LENGTH"];
		}
		else
		{
			maxLength = json.GetJson(JsonManager::FileType::CAMERA)["MAX_LENGTH"];
			minLength = json.GetJson(JsonManager::FileType::CAMERA)["MIN_LENGTH"];
		}
		break;
	default:
		maxLength = json.GetJson(JsonManager::FileType::CAMERA)["MAX_LENGTH"];
		minLength = json.GetJson(JsonManager::FileType::CAMERA)["MIN_LENGTH"];
		break;
	}
	const float LERP_VALUE = json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_LENGTH"];

	/*�����̕␳*/
	if (this->length >= maxLength)
	{
		this->length = Lerp(this->length, maxLength, LERP_VALUE);
	}
	else if (this->length <= minLength)
	{
		this->length = Lerp(this->length, minLength, LERP_VALUE);
	}
}

/// <summary>
/// �ړ��x�N�g���̕␳
/// </summary>
void Camera::UpdateVelocity()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json			= Singleton<JsonManager>::GetInstance();
	auto& player		= Singleton<PlayerManager>::GetInstance();
	auto& enemy			= Singleton<EnemyManager>::GetInstance();
	auto& sceneChanger	= Singleton<SceneChanger>::GetInstance();

	/*���̍��W���o��*/
	VECTOR nextPosition = Gori::ORIGIN;

	/*�V�[���̏�Ԃŏ�����ς���*/
	switch (sceneChanger.GetNowSceneType())
	{
	case SceneChanger::SceneType::TITLE:
		const VECTOR NOW_POSITION = this->collider->rigidbody.GetPosition();
		const VECTOR CAMERA_TO_TARGET = VNorm(VSub(this->nowTarget, NOW_POSITION));
		const VECTOR MOVE_VELOCITY = VNorm(VCross(CAMERA_TO_TARGET, Gori::UP_VEC));
		nextPosition = VNorm(VAdd(NOW_POSITION, MOVE_VELOCITY));
		nextPosition = VScale(nextPosition, this->length);
		nextPosition.y = json.GetJson(JsonManager::FileType::CAMERA)["TITLE_POSITION_OFFSET"];

		break;
	case SceneChanger::SceneType::GAME:
		if (player.GetIsLockOn())
		{
			const VECTOR ENEMY_TO_PLAYER = VNorm(VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition()));
			const VECTOR POSITION_OFFSET = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["POSITION_OFFSET"]);
			nextPosition = VScale(ENEMY_TO_PLAYER, this->length);
			nextPosition = VAdd(player.GetRigidbody().GetPosition(), nextPosition);
			nextPosition = VAdd(nextPosition, POSITION_OFFSET);
		}
		else
		{
			/*�A���O���̍X�V*/
			UpdateAngle();
			const VECTOR FIRST_DIRECTION = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["FIRST_DIRECTION"]);
			VECTOR direction = VTransform(FIRST_DIRECTION, MGetRotY(this->yow));
			VECTOR axis = VCross(direction, Gori::UP_VEC);
			direction = VTransform(direction, MGetRotAxis(axis, this->pitch));
			nextPosition = VAdd(player.GetRigidbody().GetPosition(), VScale(direction, this->length));
		}
		break;
	default:
		const VECTOR ENEMY_TO_PLAYER = VNorm(VSub(player.GetRigidbody().GetPosition(), enemy.GetRigidbody().GetPosition()));
		const VECTOR POSITION_OFFSET = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["POSITION_OFFSET"]);
		nextPosition = VScale(ENEMY_TO_PLAYER, this->length);
		nextPosition = VAdd(player.GetRigidbody().GetPosition(), nextPosition);
		nextPosition = VAdd(nextPosition, POSITION_OFFSET);
		break;
	}

	/*�␳�l*/
	const VECTOR LERP_VALUE = Gori::Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_VELOCITY"]);

	/*���̍��W�ƍ��̍��W�̍����ړ��x�N�g���ɂ���*/
	VECTOR newPosition = Lerp(this->collider->rigidbody.GetPosition(), nextPosition, LERP_VALUE);
	VECTOR newVelocity = VSub(newPosition,this->collider->rigidbody.GetPosition());

	this->collider->rigidbody.SetVelocity(newVelocity);
}

/// <summary>
/// �J�����̌����̍X�V
/// </summary>
void Camera::UpdateDirection()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();//json

	/*�J�����������_���J�����̌����ɂȂ�i���K���j*/
	this->direction = VSub(this->nowTarget, this->collider->rigidbody.GetPosition());
	this->direction = VNorm(this->direction);
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
	VECTOR out = (Gori::ORIGIN);
	out.x = Lerp(_start.x, _end.x, _percent.x);
	out.y = Lerp(_start.y, _end.y, _percent.y);
	out.z = Lerp(_start.z, _end.z, _percent.z);
	return out;
}

/// <summary>
/// ���݂̍��W�̎擾
/// </summary>
const VECTOR Camera::GetNowPosition()const
{
	return this->collider->rigidbody.GetPosition();
}