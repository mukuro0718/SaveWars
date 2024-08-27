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
	for (int i = 0; i < this->COLLIDER_NUM; i++)
	{
		this->collider[i] = new Collider();
	}

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
	this->model->AddAnimation(animationHandle, animationIndex);
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_1, static_cast<int>(AnimationType::MAIN_1));
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_2, static_cast<int>(AnimationType::MAIN_2));
	this->attackAnimationMap.emplace(this->SPECIAL_ATTACK, static_cast<int>(AnimationType::SPECIAL));
	this->attackComboStateMap.emplace(0, this->MAIN_ATTACK_1);
	this->attackComboStateMap.emplace(1, this->MAIN_ATTACK_2);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	for (int i = 0; i < this->COLLIDER_NUM; i++)
	{
		DeleteMemberInstance(this->collider[i]);
	}
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
	this->hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];	 //�g�嗦
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
	
	/*���A�N�V����*/
	Reaction();

	/*�ړ�*/
	Move();

	/*���b�N�I��*/
	LockOn();
	
	/*�K�[�h*/
	Block();

	/*���*/
	Avoid();

	/*�U��*/
	Attack();

	/*�Ȃɂ��ړ����Ă��Ȃ���Αҋ@��Ԃ��Z�b�g����*/
	if (DontAnyAction())
	{
		this->state->SetFlag(this->IDLE);
	}
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];

	this->model->SetPosition(position);

	/*�R���C�_�[�̍X�V*/
	float hitHeight = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->SetCapsule(position,hitHeight, radius);

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
	printfDx("%d:RUN					\n", this->state->CheckFlag(this->RUN));
	printfDx("%d:WALK					\n", this->state->CheckFlag(this->WALK));
	printfDx("%d:AVOID					\n", this->state->CheckFlag(this->AVOID));
	printfDx("%d:BLOCK					\n", this->state->CheckFlag(this->BLOCK));
	printfDx("%d:BIG_IMPACT				\n", this->state->CheckFlag(this->BIG_IMPACT));
	printfDx("%d:SMALL_IMPACT			\n", this->state->CheckFlag(this->SMALL_IMPACT));
	printfDx("%d:CASTING				\n", this->state->CheckFlag(this->MAIN_ATTACK_1));
	printfDx("%d:COMBO_ATTACK			\n", this->state->CheckFlag(this->MAIN_ATTACK_2));
	printfDx("%d:KICK					\n", this->state->CheckFlag(this->SPECIAL_ATTACK));
	this->model->Draw();
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->DrawHitCapsule();
	this->collider[static_cast<int>(ColliderType::ATTACK)]->DrawHitSphere();
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
		this->direction = VNorm(this->direction);
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

	if (this->state->CheckFlag(this->AVOID))
	{
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->WALK))
	{
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["WALK_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->RUN))
	{
		this->velocity = json.GetJson(JsonManager::FileType::PLAYER)["RUN_VELOCITY"];
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
	cameraDirection = camera.GetCameraDirection();
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
/// �_���[�W�̎擾
/// </summary>
const int Player::GetDamage()const
{
	return this->damage;
}

/// <summary>
/// �ړ��x�N�g���̕␳
/// </summary>
void Player::FixMoveVector(const VECTOR _fixVector)
{
	this->moveVector = this->moveVector + _fixVector; 
}

/// <summary>
/// �R���C�_�[�̎擾
/// </summary>
const Collider Player::GetCharacterCollider()
{
	return *this->collider[static_cast<int>(ColliderType::CHARACTER)];
}
const Collider Player::GetAttackCollider()
{
	return *this->collider[static_cast<int>(ColliderType::ATTACK)];
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
	else if (this->state->CheckFlag(this->MASK_REACTION))
	{
		if (this->state->CheckFlag(this->BIG_IMPACT))
		{
			this->nowAnimation = static_cast<int>(AnimationType::BIG_IMPACT);
		}
		else if (this->state->CheckFlag(this->SMALL_IMPACT))
		{
			this->nowAnimation = static_cast<int>(AnimationType::SMALL_IMPACT);
		}
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
		this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	}

	/*�A�j���[�V�����Đ����Ԃ��擾*/
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}
void Player::Reaction()
{
	if (this->state->CheckFlag(this->MASK_REACTION) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_REACTION);
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

	/*�U���R���C�_�[�p�ϐ�*/
	float radius = 0.0f;
	float offsetScale = 0.0f;
	float offsetY = 0.0f;
	VECTOR position = { 0.0f,0.0f,0.0f };
	int attackType = -1;

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();

	//����U��
	if (pad & PAD_INPUT_2)
	{
		this->state->SetFlag(this->SPECIAL_ATTACK);
		attackType = static_cast<int>(AttackType::SPECIAL);
	}
	//�ʏ�U��
	else if (pad & PAD_INPUT_1)
	{
		if (this->attackComboCount >= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
		{
			this->attackComboCount = 0;
		}
		switch (this->attackComboCount)
		{
		case 0:
			attackType = static_cast<int>(AttackType::MAIN_1);
			break;
		case 1:
			attackType = static_cast<int>(AttackType::MAIN_2);
			break;
		}

		this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
		this->attackComboCount++;
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
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

	/*�R���C�_�[�̍X�V*/
	if (attackType != static_cast<int>(AttackType::NONE))
	{
		radius = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_RADIUS"][attackType];
		offsetScale = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_SCALE"][attackType];
		offsetY = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_OFFSET_Y"][attackType];
		position = this->model->GetPosition() + VScale(this->direction, offsetScale);
		position.y += offsetY;
		this->damage = json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_DAMAGE"][attackType];
		this->attackNumber++;
	}
	this->collider[static_cast<int>(ColliderType::ATTACK)]->SetSphere(position, radius);
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
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	return true;
}
const bool Player::CanAvoid()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->BLOCK))		return false;//�K�[�h
	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))return false;//���A�N�V����
	if (this->state->CheckFlag(this->AVOID))		return false;//���
	if (this->state->CheckFlag(this->BLOCK))		return false;//�u���b�N
	if (this->state->CheckFlag(this->MASK_MOVE))	return false;//�ړ�

	return true;
}
void Player::CalcDamage(const int _damage) 
{ 
	if (!this->state->CheckFlag(this->BLOCK | this->AVOID))
	{
		this->hp -= _damage;
		this->state->ClearFlag(this->MASK_ALL);
		if (_damage >= 20)
		{
			this->state->SetFlag(this->BIG_IMPACT);
		}
		else
		{
			this->state->SetFlag(this->SMALL_IMPACT);
		}
	}
}