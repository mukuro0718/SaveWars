#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "Animation.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "PlayerAttackManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
	: Collidable		(Collidable::Priority::HIGH, GameObjectTag::PLAYER, GoriLib::ColliderData::Kind::CAPSULE, false)
	, modelHandle		(-1)
	, isGround			(false)
	, speed				(0.0f)
	, attackComboCount  (0)
	, prevHitNum		(0)
	, jumpPower			(0.0f)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�C���X�^���X�̍쐬*/
	this->state = new BitFlag();
	this->animation = new Animation();

	/*vector�̒ǉ�*/
	for (int i = 0; i < this->COUNT_NUM; i++)
	{
		this->frameCount.emplace_back(0);
		this->isCount.emplace_back(false);
	}

	/*�A�j���[�V�����̐ݒ�*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);
	//�A�j���[�V�����}�b�v�̐ݒ�
	this->animationMap.emplace(this->IDLE, static_cast<int>(AnimationType::IDLE));
	this->animationMap.emplace(this->ROLL, static_cast<int>(AnimationType::ROLL));
	this->animationMap.emplace(this->DEATH, static_cast<int>(AnimationType::DEATH));
	this->animationMap.emplace(this->BLOCK, static_cast<int>(AnimationType::BLOCK));
	this->animationMap.emplace(this->JUMP, static_cast<int>(AnimationType::JUMP));
	this->animationMap.emplace(this->REACTION, static_cast<int>(AnimationType::REACTION));
	this->animationMap.emplace(this->BLOCK_REACTION, static_cast<int>(AnimationType::BLOCK_REACTION));
	this->animationMap.emplace(this->RUNNING, static_cast<int>(AnimationType::RUNNING));
	this->animationMap.emplace(this->WALK_BACK, static_cast<int>(AnimationType::WALK_BACK));
	this->animationMap.emplace(this->WALK_FRONT, static_cast<int>(AnimationType::WALK_FRONT));
	this->animationMap.emplace(this->WALK_LEFT, static_cast<int>(AnimationType::WALK_LEFT));
	this->animationMap.emplace(this->WALK_RIGHT, static_cast<int>(AnimationType::WALK_RIGHT));
	this->animationMap.emplace(this->SLASH, static_cast<int>(AnimationType::SLASH));


	/*�R���C�_�[�f�[�^�̍쐬*/
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	capsuleColiderData->radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];
	capsuleColiderData->height = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];
	capsuleColiderData->isCutDamage = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->state);
	this->frameCount.clear();
	this->isCount.clear();
}

/// <summary>
/// ������
/// </summary>
void Player::Initialize(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//��]��
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //�g�嗦

	/*�ϐ��̏�����*/
	this->direction				= VGet(0.0f, 0.0f, -1.0f);
	capsuleColiderData->hp		= json.GetJson(JsonManager::FileType::PLAYER)["HP"];	 //HP
	this->stamina				= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];	 //�X�^�~�i

	/*���f���̓ǂݍ���*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::PLAYER));

	/*�R���C�_�[�̏�����*/
	Collidable::Initialize(_physics);
	
	/*���������̏�����*/
	this->rigidbody.Initialize(true);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
	this->speed = 0.0f;

	this->state->SetFlag(this->IDLE);
	this->animation->Attach(&this->modelHandle);
	this->attackNumber = 0;
	this->jumpPower = 0.0f;
}

/// <summary>
/// �㏈��
/// </summary>
void Player::Finalize(GoriLib::Physics* _physics)
{
	/*�����o�^�̉���*/
	Collidable::Finalize(_physics);
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	//�n�ʂɂ��Ă��邩�𔻒�
	auto capsuleColliderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	VECTOR start = rigidbody.GetPosition();
	VECTOR end = VGet(start.x, start.y + capsuleColliderData->height, start.z);
	auto hitObjects = _physics->IsCollideLine(start, end);
	this->isGround = false;
	for (const auto& object : hitObjects)
	{
		if (object->GetTag() == GameObjectTag::GROUND)
		{
			this->isGround = true;
			break;
		}
	}
	/*�t���O�̏�����*/
	this->state->ClearFlag(this->MASK_ALL);

	/*�A�N�V����*/
	Death();
	Reaction();
	LockOn();
	Attack();
	Move();
	Rolling();
	Block();
	Jump();
	//���������A�N�V���������Ă��Ȃ�������Idle������
	if (DontAnyAction()) 
	{ 
		this->state->SetFlag(this->IDLE); 
	}

	//��Ԃ������܂��͑ҋ@�̎��̂݃X�^�~�i���񕜂���i���̑��X�^�~�i����͂��̏ꏊ�ōs���Ă���
	//�X�^�~�i�v�Z�̏ꏊ���U��΂��Ă���̂œ��ꂵ����
	if (!this->state->CheckFlag(this->MASK_CANT_RECOVERY_STAMINA))
	{
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);
	}

	/*�A�j���[�V�����̍X�V*/
	UpdateAnimation();
	float animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	VECTOR position = this->rigidbody.GetPosition();
	this->animation->Play(&this->modelHandle, position, this->nowAnimation, animationPlayTime);
}

/// <summary>
/// �`��
/// </summary>
const void Player::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();

	VECTOR position = this->rigidbody.GetPosition();
	VECTOR rotation = this->rigidbody.GetRotation();
#if _DEBUG
	printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
	printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
	printfDx("PLAYER_SPEED:%f					\n", this->speed);
	printfDx("%d:IDLE							\n", this->state->CheckFlag(this->IDLE));
	printfDx("%d:LOCK_ON						\n", this->isLockOn);
	printfDx("%d:ROLL							\n", this->state->CheckFlag(this->ROLL));
	printfDx("%d:DEATH						\n", this->state->CheckFlag(this->DEATH));
	printfDx("%d:BLOCK						\n", this->state->CheckFlag(this->BLOCK));
	printfDx("%d:JUMP							\n", this->state->CheckFlag(this->JUMP));
	printfDx("%d:REACTION						\n", this->state->CheckFlag(this->REACTION));
	printfDx("%d:BLOCK_REACTION				\n", this->state->CheckFlag(this->BLOCK_REACTION));
	printfDx("%d:RUNNING						\n", this->state->CheckFlag(this->RUNNING));
	printfDx("%d:WALK_BACK					\n", this->state->CheckFlag(this->WALK_BACK));
	printfDx("%d:WALK_FRONT					\n", this->state->CheckFlag(this->WALK_FRONT));
	printfDx("%d:WALK_LEFT					\n", this->state->CheckFlag(this->WALK_LEFT));
	printfDx("%d:WALK_RIGHT					\n", this->state->CheckFlag(this->WALK_RIGHT));
	printfDx("%d:SLASH						\n", this->state->CheckFlag(this->SLASH));
	printfDx("%d:HIT_NUM						\n", this->GetHitNumber());
#endif // _DEBUG

	MV1SetPosition(this->modelHandle, this->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->rigidbody.GetRotation());
	MV1SetScale(this->modelHandle, this->rigidbody.GetScale());
	MV1DrawModel(this->modelHandle);
}

/// <summary>
/// �ړ�
/// </summary>
void Player::Move()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*��]���̍X�V*/
	UpdateRotation();

	/*���x�̍X�V*/
	UpdateSpeed();

	/*�ړ��x�N�g���̍X�V*/
	UpdateMoveVector();

	/*�����̍X�V*/
	//���b�N�I�����Ă��Ȃ�������
	if (!this->isLockOn)
	{
		if (state->CheckFlag(this->MASK_MOVE))
		{
			this->direction = this->rigidbody.GetVelocity();
			this->direction = VNorm(this->direction);
		}
		this->direction.y = 0.0f;
	}
	else
	{
		this->direction = VSub(enemy.GetPosition(),this->rigidbody.GetPosition());
		this->direction = VNorm(this->direction);
	}
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateMoveVector()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�ړ��x�N�g��������������*/
	VECTOR direction = { 0.0f,0.0f,0.0f };

	/*�ړ����Ă���Ƃ���������Ă���Ƃ��͈ړ��x�N�g�����o��*/
	if (this->state->CheckFlag(this->MASK_CAN_VELOCITY))
	{
		/*��]�������ƂɈړ��x�N�g�����o��*/
		direction = VGet(-sinf(this->moveVectorRotation.y), 0.0f, -cosf(this->moveVectorRotation.y));
		/*�ړ��x�N�g���𐳋K��*/
		direction = VNorm(direction);
	}
	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = rigidbody.GetVelocity();
	float yVelocity = prevVelocity.y - this->jumpPower;

	/*�W�����v�͂̌v�Z*/
	if (this->state->CheckFlag(this->JUMP))
	{
		const float MIN_POWER = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"] * -1.0;
		this->jumpPower += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["JUMP_DECEL"]);
		if (this->jumpPower < MIN_POWER)
		{
			this->jumpPower = 0.0f;
		}
	}

	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y + this->jumpPower, aimVelocity.z);

	this->rigidbody.SetVelocity(newVelocity);
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateSpeed()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	float maxSpeed = 0.0f;
	
	/*�ړ����Ă�����*/
	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		//����
		if (this->state->CheckFlag(this->RUNNING))
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
		}
		//����
		else
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
		}
	}

	/*���x�̐ݒ�*/
	//�ő呬�x���O����Ȃ���΍ő呬�x�܂ŉ�������
	if (maxSpeed != 0)
	{
		this->speed += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"]);
		//�ő呬�x�𒴂��Ȃ��悤�ɒ�������
		if (this->speed >= maxSpeed)
		{
			this->speed = maxSpeed;
		}
	}
	else
	{
		this->speed += static_cast<float>(json.GetJson(JsonManager::FileType::PLAYER)["DECEL"]);
		//�ő呬�x�𒴂��Ȃ��悤�ɒ�������
		if (this->speed <= 0)
		{
			this->speed = 0;
		}
	}
}

/// <summary>
/// ���W��getter
/// </summary>
/// <returns></returns>
const VECTOR Player::GetPosition()const
{
	return this->rigidbody.GetPosition();
}

/// <summary>
/// ��]���̍X�V
/// </summary>
void Player::UpdateRotation()
{
	/*�ړ��ł��邩*/
	if (!CanRotation())return;

	/*������*/
	const float PI				  		= 180.0f;						//�ʓx�@�ł̃�
	VECTOR		rotation				= VGet(0.0f,0.0f,0.0f );	//��]��
	bool		isInputLStick			= false;						//L�X�e�B�b�N����
	VECTOR		cameraDirection			= VGet(0.0f,0.0f,0.0f );	//�J�����̌���
	VECTOR		playerToTargetDirection = VGet(0.0f,0.0f,0.0f );	//�J�����̌���
	VECTOR		wasd					= VGet(0.0f, 0.0f, 0.0f);	//wasd����
	VECTOR		lStick					= VGet(0.0f, 0.0f, 0.0f);	//lStick����(��:Z+ ��:Z- ��:x- �E:x+)
	this->state->ClearFlag(this->MASK_MOVE);

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input  = Singleton<InputManager> ::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager> ::GetInstance();
	auto& json	 = Singleton<JsonManager>  ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();
	//�X�e�B�b�N����
	lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));

	/*�ړ���Ԃ̐؂�ւ�*/
	//�X�e�B�b�N���͂����邩
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;
		//���b�N�I����Ԃ�
		if (this->isLockOn)
		{
			//��
			if (lStick.x < 0)
			{
				this->state->SetFlag(this->WALK_LEFT);
			}
			//�E
			else if (lStick.x > 0)
			{
				this->state->SetFlag(this->WALK_RIGHT);
			}
			//�O
			else if (lStick.z < 0)
			{
				this->state->SetFlag(this->WALK_FRONT);
			}
			//���
			else
			{
				this->state->SetFlag(this->WALK_BACK);
			}
		}
		//�O�ɑ���
		else
		{
			this->state->SetFlag(this->RUNNING);
		}
	}


	/*�J�����̌������o��*/
	//�J�������W����v���C���[�̍��W�ւ̃x�N�g�����o��
	cameraDirection = camera.GetCameraDirection();
	//���߂��x�N�g���𐳋K������
	cameraDirection = VNorm(cameraDirection);
	//���]���Ă����̂Ńx�N�g���𔽓]����
	cameraDirection = cameraDirection * -1.0f;

	/*�v���C���[����{�X�ւ̌������o��*/
	//�{�X���W����v���C���[�̍��W�ւ̃x�N�g�����o��
	playerToTargetDirection = VSub(enemy.GetPosition(), this->rigidbody.GetPosition());
	//���߂��x�N�g���𐳋K������
	playerToTargetDirection = VNorm(playerToTargetDirection);


	/*�������b�N�I�����Ă�����*/
	if (this->isLockOn)
	{
		/*�J�����̌����Ă�����������ƂɃ��f���̉�]�����o���B*/
		rotation.y = static_cast<float>(
			-atan2(static_cast<double>(playerToTargetDirection.z), static_cast<double>(playerToTargetDirection.x)));
		rotation.y -= 90.0f * (DX_PI_F / 180.0f);
		if (isInputLStick)
		{
			lStick = VNorm(lStick);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(playerToTargetDirection.z), static_cast<double>(playerToTargetDirection.x))
				- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
		}
	}
	else
	{
		/*�J�����̌����Ă�������ƁA�v���C���[���ŏ��Ɍ����Ă������������ƂɃ��f���̉�]�����o���B*/
		if (isInputLStick)
		{
			lStick = VNorm(lStick);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
			this->moveVectorRotation = rotation;
		}
	}

	if (isInputLStick || this->isLockOn)
	{
		this->rigidbody.SetRotation(rotation);
	}
}

/// <summary>
/// state���ړ���ԂɂȂ��Ă��邩
/// </summary>
const bool Player::IsMove()const
{
	return this->state->CheckFlag(this->MASK_MOVE);
}

/// <summary>
/// �U��������
/// </summary>
const bool Player::GetIsSlash()const
{
	return this->state->CheckFlag(this->SLASH);
}
/// <summary>
/// �_���[�W�̎擾
/// </summary>
const int Player::GetDamage()const
{
	return this->damage;
}


/// <summary>
/// �A�j���[�V�����̍X�V
/// </summary>
void Player::UpdateAnimation()
{
	auto& json = Singleton<JsonManager>  ::GetInstance();

	this->nowAnimation = this->animationMap[this->state->GetFlag()];
}
void Player::Reaction()
{
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	if (this->prevHitNum != capsuleColiderData->GetHitNumber())
	{

	}
}

/// <summary>
/// ���b�N�I��
/// </summary>
void Player::LockOn()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	//pad����
	int pad = input.GetPadState();

	//LT�������ꂽ��
	if (pad & PAD_INPUT_6)
	{
		this->isLockOn = true;
	}
	else
	{
		this->isLockOn = false;
	}
}

/// <summary>
/// �f�X
/// </summary>
void Player::Death()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);

	/*����HP���O������������*/
	if (capsuleColiderData->GetHP() < 0)
	{
		this->state->SetFlag(this->DEATH);
	}
}
/// <summary>
/// �u���b�N
/// </summary>
void Player::Block()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);

	/*�u���b�N�ł��邩*/
	if (!CanBlock())return;

	/*LT�������ꂽ��*/
	if (pad & PAD_INPUT_7)
	{
		this->state->SetFlag(this->BLOCK);
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
	}
}
/// <summary>
/// �W�����v
/// </summary>
void Player::Jump()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (this->state->CheckFlag(this->JUMP) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->JUMP);
	}


	/*������ł��邩*/
	if (!CanJump())return;

	/*����X�^�~�i�͑����̂�*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["JUMP_STAMINA_CONSUMPTION"]))return;

	if ((pad & PAD_INPUT_3))
	{
		this->state->SetFlag(this->JUMP);
		this->state->ClearFlag(this->MASK_MOVE | this->IDLE);
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["JUMP_STAMINA_CONSUMPTION"]);
		this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
	}
}
/// <summary>
/// ���
/// </summary>
void Player::Rolling()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (this->state->CheckFlag(this->ROLL) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROLL);
	}

	if (FrameCount(static_cast<int>(FrameCountType::AVOID), json.GetJson(JsonManager::FileType::PLAYER)["AVOID_MAX_FRAME"]))
	{
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = false;
	}

	/*������ł��邩*/
	if (!CanRolling())return;

	/*����X�^�~�i�͑����̂�*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]))return;

	if ((pad & PAD_INPUT_4) && !this->isCount[static_cast<int>(FrameCountType::AVOID)])
	{
		this->state->SetFlag(this->ROLL);
		this->isCount[static_cast<int>(FrameCountType::AVOID)] = true;
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = true;
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]);
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"];
	}

}
/// <summary>
/// �U��
/// </summary>
void Player::Attack()
{
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();

	/*pad����*/
	int pad = input.GetPadState();

	/*�U���ł��邩*/
	if (!CanAttack())return;

	/*X�������ꂽ��*/
	if (pad & PAD_INPUT_1)
	{
		this->state->SetFlag(this->SLASH);
		this->attackNumber++;
	}
}

bool Player::FrameCount(const int _index, const int _maxFrame)
{
	/*�����J�E���g���J�n���Ă�����*/
	if (this->isCount[_index])
	{
		//�J�E���g�𑝉�������
		this->frameCount[_index]++;
		//�����ő���z���Ă�����t���O�������ăJ�E���g������������
		if (this->frameCount[_index] >= _maxFrame)
		{
			this->isCount[_index] = false;
			this->frameCount[_index] = 0;
			return true;
		}
	}
	return false;
}

const bool Player::CanRotation()const
{
	if (this->state->CheckFlag(this->REACTION))		return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH)	)		return false;//�f�X
	if (this->state->CheckFlag(this->ROLL))			return false;//���
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//���
	if (this->state->CheckFlag(this->JUMP))			return false;//�W�����v
	return true;
}
const bool Player::CanRolling()const
{
	if (this->isLockOn)									  return false;//���b�N�I��
	if (this->state->CheckFlag(this->MASK_ATTACK))	  return false;//�U��
	if (this->state->CheckFlag(this->BLOCK))		  return false;//�u���b�N
	if (this->state->CheckFlag(this->MASK_REACTION)) return false;//���A�N�V����
	if (this->state->CheckFlag(this->JUMP))			  return false;//�W�����v
	if (this->state->CheckFlag(this->DEATH))		  return false;//���S
	if (this->state->CheckFlag(this->ROLL))			  return false;//���
	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))	return false;//�f�X
	if (this->state->CheckFlag(this->ROLL))		return false;//���
	if (this->state->CheckFlag(this->SLASH))	return false;//���
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))	return false;//�f�X
	if (this->state->CheckFlag(this->ROLL))		return false;//���
	if (this->state->CheckFlag(this->SLASH))	return false;//�U��
	if (this->state->CheckFlag(this->JUMP))		return false;//�W�����v
	return true;
}
const bool Player::CanJump()const
{
	if (this->state->CheckFlag(this->REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))	return false;//�f�X
	if (this->state->CheckFlag(this->ROLL))		return false;//���
	if (this->state->CheckFlag(this->SLASH))	return false;//�U��
	if (this->state->CheckFlag(this->JUMP))		return false;//�W�����v
	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->REACTION))		return false;//���A�N�V����
	if (this->state->CheckFlag(this->ROLL))			return false;//���
	if (this->state->CheckFlag(this->DEATH))		return false;//�u���b�N
	if (this->state->CheckFlag(this->MASK_MOVE))	return false;//�ړ�
	if (this->state->CheckFlag(this->JUMP))			return false;//�W�����v
	return true;
}

void Player::OnCollide(const Collidable& _colider)
{
	std::string message = "�v���C���[��";
	if (_colider.GetTag() == GameObjectTag::BOSS)
	{
		message += "�{�X";
	}
	else if (_colider.GetTag() == GameObjectTag::GROUND)
	{
		message += "�n��";
	}

	message += "�Ɠ�������\n";
	printfDx(message.c_str());
}

/// <summary>
/// HP�̎擾
/// </summary>
const int Player::GetHP()const
{ 
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	return capsuleColiderData->GetHP();
}
/// <summary>
/// �X�^�~�i�̎擾
/// </summary>
const float Player::GetStamina()const
{
	return this->stamina;
}
/// <summary>
/// �K�v�ȏ���X�^�~�i�ƌ��݂̃X�^�~�i�̎c�ʂ��ׂăA�N�V�������\�Ȃ�true��Ԃ�
/// </summary>
/// <returns></returns>
const bool Player::CanAction(const float _staminaConsumed)const
{
	float staminaConsumed = _staminaConsumed * -1.0f;
	/*�X�^�~�i�̏���ʂ����݂̃X�^�~�i�̑��ʂ����������false��Ԃ�*/
	if (staminaConsumed > this->stamina)return false;
	return true;
}

/// <summary>
/// �X�^�~�i�̉񕜏���
/// </summary>
void Player::CalcStamina(const float _staminaConsumed)
{
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*����ȊO�̏�Ԃ��������Ԃɉ����ăX�^�~�i�������*/
	//����
	this->stamina += _staminaConsumed;
	/*����l�A�����l�𒴂��Ȃ��悤�ɒ���*/
	if (this->stamina >= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"])
	{
		this->stamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	}
	else if (this->stamina < 0)
	{
		this->stamina = 0;
	}
}
