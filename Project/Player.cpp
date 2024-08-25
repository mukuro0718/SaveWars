#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Animation.h"
#include "Model.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "Player.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "Vector4.h"
#include "Collider.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
	: model				(nullptr)
	, collider			(nullptr)
	, moveVector		{ 0.0f, 0.0f, 0.0f }
	, direction			{ 0.0f, 0.0f, 0.0f }
	, fixVector			{ 0.0f, 0.0f, 0.0f }
	, moveVectorRotation{ 0.0f, 0.0f, 0.0f }
	, wasd				{ 0.0f, 0.0f, 0.0f }
	, lStick			{ 0.0f, 0.0f, 0.0f }
	, velocity			(0.0f)
	, attackComboCount  (0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�C���X�^���X�̍쐬*/
	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::PLAYER));
	this->state = new BitFlag();
	this->collider = new Collider();

	/*vector�̒ǉ�*/
	for (int i = 0; i < this->COUNT_NUM; i++)
	{
		this->frameCount.emplace_back(0);
		this->isCount.emplace_back(false);
	}

	/*�A�j���[�V�����̐ݒ�*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);
	this->attackAnimationMap.emplace(this->CASTING				, static_cast<int>(AnimationType::CASTING));
	this->attackAnimationMap.emplace(this->COMBO_ATTACK			, static_cast<int>(AnimationType::COMBO));
	this->attackAnimationMap.emplace(this->CROUCH_SLASH			, static_cast<int>(AnimationType::CROUCH_SLASH));
	this->attackAnimationMap.emplace(this->KICK					, static_cast<int>(AnimationType::KICK));
	this->attackAnimationMap.emplace(this->PUNCH				, static_cast<int>(AnimationType::PUNCH));
	this->attackAnimationMap.emplace(this->ROTATION_ATTACK		, static_cast<int>(AnimationType::ROTATION_ATTACK));
	this->attackAnimationMap.emplace(this->SLASH_1				, static_cast<int>(AnimationType::SLASH_1));
	this->attackAnimationMap.emplace(this->SLASH_2				, static_cast<int>(AnimationType::SLASH_2));
	this->attackComboStateMap.emplace(0, this->SLASH_1);
	this->attackComboStateMap.emplace(1, this->SLASH_2);
	this->attackComboStateMap.emplace(2, this->COMBO_ATTACK);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	DeleteMemberInstance(this->collider);
	this->frameCount.clear();
	this->isCount.clear();
}

void Player::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR position = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//���W
	const VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//��]��
	const VECTOR scale = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //�g�嗦
	this->velocity = 0.0f;
	this->direction = VGet(0.0f, 0.0f, -1.0f);

	/*���f���̃g�����X�t�H�[���̐ݒ�*/
	this->model->SetTransform(position, rotation, scale);
}

/// <summary>
/// �A�N�V����
/// </summary>
void Player::Action()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*��������̐؂�ւ�*/
	StandOrSit();

	/*�ړ�*/
	Move();

	/*���b�N�I��*/
	LockOn();

	/*�W�����v*/
	Jump();
	
	/*�K�[�h*/
	Block();

	/*���*/
	Avoid();

	/*���K*/
	Roar();

	/*�U��*/
	Attack();

	/*�Ȃɂ��ړ����Ă��Ȃ���Αҋ@��Ԃ��Z�b�g����*/
	if (DontAnyAction())
	{
		this->state->SetFlag(this->IDLE);
	}

	/*�R���C�_�[�̐ݒ�*/
	const VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius	  = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];
	this->collider->SetSphere(position, radius);
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];

	/*Y���W - ���a���O����������������*/
	if (position.y <= 0.0f)
	{
		position.y = 0.0f;
		this->state->ClearFlag(this->JUMP);
		this->jumpPower = 0.0f;
	}
	this->model->SetPosition(position);

	/*�R���C�_�[�̍X�V*/
	this->collider->SetSphere(position, radius);

	/*�A�j���[�V�����̍X�V*/
	UpdateAnimation();

	this->model->PlayAnimation(this->nowAnimation,this->animationPlayTime);
}

/// <summary>
/// �`��
/// </summary>
const void Player::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();

	VECTOR position = this->model->GetPosition();
	VECTOR rotation = this->model->GetRotation();
	printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("PLAYER_VELOCITY:%f\n", this->velocity);
	printfDx("%d:IDLE					\n", this->state->CheckFlag(this->IDLE));
	printfDx("%d:REACTION				\n", this->state->CheckFlag(this->REACTION));
	printfDx("%d:DEATH					\n", this->state->CheckFlag(this->DEATH));
	printfDx("%d:LOCK_ON				\n", this->state->CheckFlag(this->LOCK_ON));
	printfDx("%d:STAND					\n", this->state->CheckFlag(this->STAND));
	printfDx("%d:CROUCH					\n", this->state->CheckFlag(this->CROUCH));
	printfDx("%d:RUN					\n", this->state->CheckFlag(this->RUN));
	printfDx("%d:WALK					\n", this->state->CheckFlag(this->WALK));
	printfDx("%d:AVOID					\n", this->state->CheckFlag(this->AVOID));
	printfDx("%d:JUMP					\n", this->state->CheckFlag(this->JUMP));
	printfDx("%d:BLOCK					\n", this->state->CheckFlag(this->BLOCK));
	printfDx("%d:BIG_IMPACT				\n", this->state->CheckFlag(this->BIG_IMPACT));
	printfDx("%d:SMALL_IMPACT			\n", this->state->CheckFlag(this->SMALL_IMPACT));
	printfDx("%d:CASTING				\n", this->state->CheckFlag(this->CASTING));
	printfDx("%d:COMBO_ATTACK			\n", this->state->CheckFlag(this->COMBO_ATTACK));
	printfDx("%d:CROUCH_SLASH			\n", this->state->CheckFlag(this->CROUCH_SLASH));
	printfDx("%d:KICK					\n", this->state->CheckFlag(this->KICK));
	printfDx("%d:PUNCH					\n", this->state->CheckFlag(this->PUNCH));
	printfDx("%d:ROTATION_ATTACK		\n", this->state->CheckFlag(this->ROTATION_ATTACK));
	printfDx("%d:SLASH_1				\n", this->state->CheckFlag(this->SLASH_1));
	printfDx("%d:SLASH_2				\n", this->state->CheckFlag(this->SLASH_2));
	printfDx("%d:ROAR					\n", this->state->CheckFlag(this->ROAR));
	this->model->Draw();
	this->collider->DrawHitSphere();
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
	UpdateVelocity();

	/*�ړ��x�N�g���̍X�V*/
	UpdateMoveVector();

	/*�����̍X�V*/
	//���b�N�I�����Ă��Ȃ�������
	if (!state->CheckFlag(this->LOCK_ON))
	{
		if (state->CheckFlag(this->MASK_MOVE))
		{
			this->direction = this->moveVector;
		}
		this->direction.y = 0.0f;
	}
	else
	{
		this->direction = VSub(enemy.GetPosition(),this->model->GetPosition());
	}
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateMoveVector()
{
	/*�ړ��x�N�g��������������*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*�ړ����Ă���Ƃ���������Ă���Ƃ��͈ړ��x�N�g�����o��*/
	if (this->state->CheckFlag(this->MASK_MOVE) || this->state->CheckFlag(this->AVOID))
	{
		/*��]�������ƂɈړ��x�N�g�����o��*/
		this->moveVector = VGet(-sinf(this->moveVectorRotation.y), 0.0f, -cosf(this->moveVectorRotation.y));
		/*�ړ��x�N�g���𐳋K��*/
		this->moveVector = VNorm(this->moveVector);
		/*�ړ��x�N�g���ɑ��x�����Z*/
		this->moveVector = this->moveVector * this->velocity;
	}
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateVelocity()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*��Ԃ��ړ���ԂłȂ���Α��x�𗎂Ƃ�*/
	if (!this->state->CheckFlag(this->MASK_MOVE))
	{
		/*�萔�^�ɑ��*/
		float DECEL = json.GetJson(JsonManager::FileType::PLAYER)["DECEL"];

		/*�������x���ő呬�x�ȏゾ�����烊�^�[����Ԃ�*/
		if (this->velocity <= 0.0f)
		{
			this->velocity = 0.0f;
			return;
		}

		/*����������*/
		this->velocity -= DECEL;
	}
	/*�ړ����Ă����瑬�x���ő呬�x�܂ŏグ��*/
	else
	{
		//�����x
		const float ACCEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];
		//�ő呬�x�̌���
		float maxVelocity = 0.0f;
		if (this->state->CheckFlag(this->AVOID))
		{
			maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
		}
		else if (this->state->CheckFlag(this->WALK))
		{
			maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["WALK_VELOCITY"];
		}
		else if (this->state->CheckFlag(this->RUN))
		{
			maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["RUN_VELOCITY"];
		}
		/*�������x���ő呬�x�ȏゾ�����烊�^�[����Ԃ�*/
		if (this->velocity >= maxVelocity)
		{
			this->velocity = maxVelocity;
			return;
		}

		/*����������*/
		this->velocity += ACCEL;
	}
}

/// <summary>
/// ���W��getter
/// </summary>
/// <returns></returns>
const VECTOR Player::GetPosition()const
{
	return this->model->GetPosition();
}

/// <summary>
/// ��]���̍X�V
/// </summary>
void Player::UpdateRotation()
{
	/*�ړ��ł��邩*/
	if (!CanRotation())return;

	/*�g�p����l�̏���*/
	const float PI = 180.0f;				//�ʓx�@�ł̃�
	VECTOR rotation = { 0.0f,0.0f,0.0f };	//��]��
	bool   isInputLStick = false;				//L�X�e�B�b�N����
	VECTOR cameraDirection = { 0.0f,0.0f,0.0f };	//�J�����̌���
	VECTOR playerToBossDirection = { 0.0f,0.0f,0.0f };	//�J�����̌���

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input  = Singleton<InputManager> ::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager> ::GetInstance();
	auto& json	 = Singleton<JsonManager>  ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	//�X�e�B�b�N����
	this->lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
	//�X�e�B�b�N�����͂���Ă�����ړ����Ă���
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;

		//�Ȃɂ��ړ���Ԃ������Ă��Ȃ����WALK���Z�b�g����
		if (!this->state->CheckFlag(this->MASK_MOVE))
		{
			this->state->SetFlag(this->WALK);
		}
		//L�X�e�B�b�N�������݂�����Ώ�Ԃ�؂�ւ���
		if (pad & PAD_INPUT_9 && !this->isCount[static_cast<int>(FrameCountType::SWITCH_MOVE_STATE)])
		{
			if (this->state->CheckFlag(this->RUN))
			{
				this->state->ClearFlag(this->RUN);
				this->state->SetFlag(this->WALK);
			}
			else
			{
				this->state->ClearFlag(this->WALK);
				this->state->SetFlag(this->RUN);
			}
			this->isCount[static_cast<int>(FrameCountType::SWITCH_MOVE_STATE)] = true;
		}
	}
	else
	{
		this->state->ClearFlag(this->RUN | this->WALK);
	}
	FrameCount(static_cast<int>(FrameCountType::SWITCH_MOVE_STATE), json.GetJson(JsonManager::FileType::PLAYER)["SWITCH_MOVE_STATE_MAX_FRAME"]);


	/*�J�����̌������o��*/
	//�J�������W����v���C���[�̍��W�ւ̃x�N�g�����o��
	cameraDirection = this->direction = camera.GetCameraDirection();
	//���߂��x�N�g���𐳋K������
	cameraDirection = VNorm(cameraDirection);
	//���]���Ă����̂Ńx�N�g���𔽓]����
	cameraDirection = cameraDirection * -1.0f;

	/*�v���C���[����{�X�ւ̌������o��*/
	//�{�X���W����v���C���[�̍��W�ւ̃x�N�g�����o��
	playerToBossDirection = VSub(enemy.GetPosition(), this->model->GetPosition());
	//���߂��x�N�g���𐳋K������
	playerToBossDirection = VNorm(playerToBossDirection);


	/*�������b�N�I�����Ă�����*/
	if (this->state->CheckFlag(this->LOCK_ON))
	{
		/*�J�����̌����Ă�����������ƂɃ��f���̉�]�����o���B*/
		rotation.y = static_cast<float>(
			-atan2(static_cast<double>(playerToBossDirection.z), static_cast<double>(playerToBossDirection.x)));
		rotation.y -= 90.0f * (DX_PI_F / 180.0f);
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(playerToBossDirection.z), static_cast<double>(playerToBossDirection.x))
				- atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
		}
	}
	else
	{
		/*�J�����̌����Ă�������ƁA�v���C���[���ŏ��Ɍ����Ă������������ƂɃ��f���̉�]�����o���B*/
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				- atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
			this->moveVectorRotation = rotation;
		}
	}
	rotation = VGet(rotation.x * PI / DX_PI_F, (rotation.y * PI / DX_PI_F), rotation.z * PI / DX_PI_F);

	if (isInputLStick || this->state->CheckFlag(this->LOCK_ON))
	{
		this->model->SetRotation(rotation);
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
const bool Player::IsAttack()const
{
	/*�U���}�X�N���m�F���ăr�b�g�������Ă�����true��Ԃ�*/
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}

/// <summary>
/// �y�U�������Ă��邩
/// </summary>
const bool Player::IsLightAttack()const
{
	return this->state->CheckFlag(this->MASK_ATTACK);
}


/// <summary>
/// �d�U�������Ă��邩
/// </summary>
const bool Player::IsHeavyAttack()const
{
	return this->state->CheckFlag(this->MASK_ATTACK);
}

/// <summary>
/// �ړ��x�N�g���̕␳
/// </summary>
const void Player::FixMoveVector(const VECTOR _fixVector)
{
	this->moveVector = this->moveVector + _fixVector; 
}

/// <summary>
/// �R���C�_�[�̎擾
/// </summary>
const Collider Player::GetCollider()
{
	return *this->collider;
}

/// <summary>
/// �A�j���[�V�����̍X�V
/// </summary>
void Player::UpdateAnimation()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*
	HACK:�A�j���[�V�����̗D�揇��
	�f�X�����A�N�V�������U���E�h��E�W�����v�E���
	*/

	/*�f�X���Ă�����*/
	if (this->state->CheckFlag(this->DEATH))
	{
		this->nowAnimation = static_cast<int>(AnimationType::DEATH);
	}
	/*�U�����Ă�����*/
	else if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		this->nowAnimation = this->attackAnimationMap[this->state->GetTargetFlag(this->MASK_ATTACK)];
	}
	/*������Ă�����*/
	else if (this->state->CheckFlag(this->AVOID))
	{
		this->nowAnimation = static_cast<int>(AnimationType::AVOID);
	}
	/*�W�����v���Ă�����*/
	else if (this->state->CheckFlag(this->JUMP))
	{
		//�ړ����Ă�����
		if (this->state->CheckFlag(this->MASK_MOVE))
		{
			//���b�N�I�����Ă�����ҋ@��Ԃ̃W�����v������
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				this->nowAnimation = static_cast<int>(AnimationType::IDLE_JUMP);
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::MOVE_JUMP);
			}
		}
		else
		{
			this->nowAnimation = static_cast<int>(AnimationType::IDLE_JUMP);
		}
	}
	/*�h�䂵�Ă�����*/
	else if (this->state->CheckFlag(this->BLOCK))
	{
		this->nowAnimation = static_cast<int>(AnimationType::BLOCK);
	}
	/*�ړ����Ă�����*/
	else if (this->state->CheckFlag(this->MASK_MOVE))
	{
		//����
		if (this->state->CheckFlag(this->RUN))
		{
			//���b�N�I�����Ă��邩
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				//Z��X�̓��͂̐�Βl���ׂāA�傫���ق��Ŕ��f����
				if (this->lStick.x * this->lStick.x < this->lStick.z * this->lStick.z)
				{
					//���
					if (this->lStick.z > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_BACK);
					}
					//�O
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::RUN);
					}
				}
				else
				{
					//�E
					if (this->lStick.x > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_RIGHT);
					}
					//��
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_LEFT);
					}
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::RUN);
			}
		}
		//����
		if (this->state->CheckFlag(this->WALK))
		{
			//���b�N�I�����Ă��邩
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				//Z��X�̓��͂̐�Βl���ׂāA�傫���ق��Ŕ��f����
				if (this->lStick.x * this->lStick.x < this->lStick.z * this->lStick.z)
				{
					//���
					if (this->lStick.z > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_BACK);
					}
					//�O
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::WALK);
					}
				}
				else
				{
					//�E
					if (this->lStick.x > 0)
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_RIGHT);
					}
					//��
					else
					{
						this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_LEFT);
					}
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::WALK);
			}
		}
	}
	/*�������Ă��Ȃ�������ҋ@���*/
	else if (this->state->CheckFlag(this->IDLE))
	{
		if (this->state->CheckFlag(this->STAND))
		{
			this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
		}
		else if (this->state->CheckFlag(this->CROUCH))
		{
			this->nowAnimation = static_cast<int>(AnimationType::CROUCH_IDLE);
		}
	}

	/*�A�j���[�V�����Đ����Ԃ��擾*/
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

void Player::StandOrSit()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	/*��������*/
	if (pad & PAD_INPUT_7)
	{
		this->state->SetFlag(this->CROUCH);
	}
	else
	{
		this->state->SetFlag(this->STAND);
	}
}

/// <summary>
/// ���b�N�I��
/// </summary>
void Player::LockOn()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	/*L�X�e�B�b�N�������݂�����΃��b�N�I��*/
	if (pad & PAD_INPUT_10 && !this->isCount[static_cast<int>(FrameCountType::SWITCH_LOCK_ON)])
	{
		if (this->state->CheckFlag(this->LOCK_ON))
		{
			this->state->ClearFlag(this->LOCK_ON);
		}
		else
		{
			this->state->SetFlag(this->LOCK_ON);
		}
		this->isCount[static_cast<int>(FrameCountType::SWITCH_LOCK_ON)] = true;
	}
	FrameCount(static_cast<int>(FrameCountType::SWITCH_LOCK_ON), json.GetJson(JsonManager::FileType::PLAYER)["SWITCH_LOCK_ON_MAX_FRAME"]);
}

/// <summary>
/// �W�����v
/// </summary>
void Player::Jump()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& input = Singleton<InputManager> ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	/*A�{�^���������ꂽ��*/
	if (CanJump() && pad & PAD_INPUT_3)
	{
		this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
		this->state->SetFlag(this->JUMP);
	}

	/*�d�͂���*/
	float y = json.GetJson(JsonManager::FileType::PLAYER)["GRAVITY"];

	if (this->state->CheckFlag(this->JUMP))
	{
		float jumpDecel = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_DECEL"];
		this->jumpPower -= jumpDecel;
	}
	this->moveVector.y = y + this->jumpPower;
}
/// <summary>
/// �u���b�N
/// </summary>
void Player::Block()
{
	/*�u���b�N�ł��Ȃ���Α������^�[��*/
	if (!CanBlock())return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	/*LB�{�^����������Ă��邩*/
	if (pad & PAD_INPUT_5)
	{
		this->state->SetFlag(this->BLOCK);
		this->state->ClearFlag(this->MASK_MOVE);
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
	}
}

/// <summary>
/// ���
/// </summary>
void Player::Avoid()
{
	/*��𒆂ɃA�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->AVOID) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->AVOID);
	}

	/*����ł��Ȃ���Α������^�[��*/
	if (!CanAvoid())return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	/*���*/
	if (input.GetPadState() & PAD_INPUT_4)
	{
		this->state->SetFlag(this->AVOID);
		this->state->ClearFlag(this->MASK_MOVE);

		//���p�̏����x������
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
	}
}

/// <summary>
/// ���K
/// </summary>
void Player::Roar()
{
	/*���K���ɃA�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->ROAR) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROAR);
	}

	/*���K���ł��Ȃ���Α������^�[��*/
	if (!CanRoar())return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	/*���K*/
	if (input.GetPadState() & PAD_INPUT_7 && input.GetPadState() & PAD_INPUT_8)
	{
		this->state->SetFlag(this->ROAR);
	}

}
/// <summary>
/// �U��
/// </summary>
void Player::Attack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager> ::GetInstance();

	/*�U�����ɃA�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	/*�U���ł��Ȃ��Ȃ瑁�����^�[��*/
	if (!CanAttack())return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	//�K�[�h���Ă�����
	if (this->state->CheckFlag(this->BLOCK))
	{
		if (pad & PAD_INPUT_2)
		{
			this->state->SetFlag(this->KICK);
		}
	}
	else
	{
		//��]�U��
		if (pad & PAD_INPUT_6)
		{
			this->state->SetFlag(this->ROTATION_ATTACK);
		}
		//�r���U��
		else if (pad & PAD_INPUT_8)
		{
			this->state->SetFlag(this->CASTING);
		}
		//�ʏ�U��
		else if (pad & PAD_INPUT_1)
		{
			if (this->attackComboCount >= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
			{
				this->attackComboCount = 0;
			}
			this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
			this->attackComboCount++;
			this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
			this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
		}
		//Y�����Ă���
		else if (pad & PAD_INPUT_2)
		{
			this->state->SetFlag(this->PUNCH);
		}
	}

	/*�����U�����Ă�����ړ��t���O������*/
	if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		this->state->ClearFlag(this->MASK_MOVE);
	}

	/*��莞�ԍU�����͂��Ȃ���΃R���{��r�؂ꂳ����*/
	if (FrameCount(static_cast<int>(FrameCountType::ATTACK_INTERVAL), json.GetJson(JsonManager::FileType::PLAYER)["CONBO_RESET_MAX_FRAME"]))
	{
		this->attackComboCount = 0;
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
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->BLOCK))		return false;//�K�[�h
	if (this->state->CheckFlag(this->CROUCH))		return false;//���Ⴊ��
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->CROUCH))		return false;//���Ⴊ��
	return true;
}
const bool Player::CanAvoid()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->BLOCK))		return false;//�K�[�h
	if (this->state->CheckFlag(this->CROUCH))		return false;//���Ⴊ��
	return true;
}
const bool Player::CanRoar()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->ROAR))			return false;//���K
	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->ROAR))			return false;//���K
	if (this->state->CheckFlag(this->JUMP))			return false;//�W�����v
	return true;
}
const bool Player::CanJump()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->ROAR))			return false;//���K
	if (this->state->CheckFlag(this->JUMP))			return false;//�W�����v
	if (this->state->CheckFlag(this->BLOCK))		return false;//�u���b�N
	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->ROAR))			return false;//���K
	if (this->state->CheckFlag(this->JUMP))			return false;//�W�����v
	if (this->state->CheckFlag(this->BLOCK))		return false;//�u���b�N
	if (this->state->CheckFlag(this->MASK_MOVE))	return false;//�ړ�

	return true;
}