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
	this->frameCount.emplace_back(0);
	this->isCount.emplace_back(false);

	/*�A�j���[�V�����̐ݒ�*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);
	this->attackAnimationMap.emplace(this->CASTING				, static_cast<int>(AnimationType::CASTING));
	this->attackAnimationMap.emplace(this->COMBO_ATTACK			, static_cast<int>(AnimationType::COMBO));
	this->attackAnimationMap.emplace(this->CROUCH_SLASH			, static_cast<int>(AnimationType::CROUCH_SLASH));
	this->attackAnimationMap.emplace(this->JUMP_ATTACK			, static_cast<int>(AnimationType::JUMP_ATTACK));
	this->attackAnimationMap.emplace(this->JUMP_ROTATION_ATTACK	, static_cast<int>(AnimationType::JUMP_ROTATION_ATTACK));
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

	/*��]���̍X�V*/
	UpdateRotation();

	/*�ړ�*/
	Move();

	/*�W�����v*/
	Jump();
	
	/*�K�[�h*/

	/*�U��*/
	Attack();

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
	if (position.y - radius <= 0)
	{
		position.y = radius;
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
	this->model->Draw();
	this->collider->DrawHitSphere();
}

/// <summary>
/// �ړ�
/// </summary>
void Player::Move()
{
	/*�ړ��x�N�g���̏�����*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*�ړ��ł��邩*/

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();

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
		this->direction = camera.GetCameraDirection() * -1.0f;
	}
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateMoveVector()
{
	if (this->state->CheckFlag(this->MASK_MOVE))
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
		float DECEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];

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
		/*�萔�^�ɑ��*/
		float maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["VELOCITY"];
		const float ACCEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];

		if (this->state->CheckFlag(this->RUN))
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
	/*�g�p����l�̏���*/
	const float PI			= 180.0f;				//�ʓx�@�ł̃�
	VECTOR rotation			= { 0.0f,0.0f,0.0f };	//��]��
	bool   isInputKey		= false;				//�L�[���͂̎擾
	bool   isInputLStick	= false;				//L�X�e�B�b�N����
	VECTOR cameraDirection	= { 0.0f,0.0f,0.0f };	//�J�����̌���
	this->lStick			= { 0.0f,0.0f,0.0f };	//���X�e�B�b�N���͕ێ��ϐ�
	this->wasd				= { 0.0f,0.0f,0.0f };	//wasd���͕ێ��ϐ�
	
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager>::GetInstance();
	auto& input  = Singleton<InputManager> ::GetInstance();
	auto& json   = Singleton<JsonManager>  ::GetInstance();


	/*���X�e�B�b�N���͂���*/
	lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;
	}

	/*wasd���͂���*/
	if (CheckHitKey(KEY_INPUT_W))
	{
		isInputKey = true;
		wasd.z = -1.0f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		isInputKey = true;
		wasd.z = 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		isInputKey = true;
		wasd.x = -1.0f;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		isInputKey = true;
		wasd.x = 1.0f;
	}

	/*�J�����̌������o��*/
	//�J�������W����v���C���[�̍��W�ւ̃x�N�g�����o��
	cameraDirection = this->direction = camera.GetCameraDirection();
	//���߂��x�N�g���𐳋K������
	cameraDirection = VNorm(cameraDirection);
	//���]���Ă����̂Ńx�N�g���𔽓]����
	cameraDirection = cameraDirection * -1.0f;

	/*�������b�N�I�����Ă�����*/
	if (this->state->CheckFlag(this->LOCK_ON))
	{
		/*�J�����̌����Ă�����������ƂɃ��f���̉�]�����o���B*/
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x)));
			rotation.y -= 90.0f * (DX_PI_F / 180.0f);
		if (isInputKey)
		{
			this->wasd = VNorm(this->wasd);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->wasd.z), static_cast<double>(this->wasd.x)));
		}
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
		}
	}
	else
	{
		/*�J�����̌����Ă�������ƁA�v���C���[���ŏ��Ɍ����Ă������������ƂɃ��f���̉�]�����o���B*/
		if (isInputKey)
		{
			this->wasd = VNorm(this->wasd);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->wasd.z), static_cast<double>(this->wasd.x)));
			this->moveVectorRotation = rotation;
		}
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
			this->moveVectorRotation = rotation;
		}
	}
	rotation = VGet(rotation.x * PI / DX_PI_F, (rotation.y * PI / DX_PI_F), rotation.z * PI / DX_PI_F);

	if (isInputKey || isInputLStick || this->state->CheckFlag(this->LOCK_ON))
	{
		this->model->SetRotation(rotation);
	}
}

/// <summary>
/// �U��
/// </summary>
void Player::Attack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
}


/// <summary>
/// �����
/// </summary>
void Player::Jump()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*����ׂ邩*/
	if (!CanJump())return;

	/*�ϐ��̏���*/
	float y = json.GetJson(JsonManager::FileType::PLAYER)["GRAVITY"];

	/*B�{�^���܂��̓L�[�{�[�h��B�������ꂽ��*/
	if (this->state->CheckFlag(this->JUMP))
	{
		float jumpDecel = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_DECEL"];
		this->jumpPower -= jumpDecel;
	}
	this->moveVector.y = y + this->jumpPower;
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
	else
	{
		/*������Ă�����*/
		if (this->state->CheckFlag(this->AVOID))
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
						if (this->lStick.z < 0)
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
						if (this->lStick.z < 0)
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
	}
	/*�������Ă��Ȃ�������ҋ@���*/
	this->nowAnimation = static_cast<int>(AnimationType::STANDING_IDLE);
}

/// <summary>
/// �u���b�N
/// </summary>
void Player::Block()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager>::GetInstance();

}

void Player::StateChanger()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*�ϐ��̏���*/
	int pad = input.GetPadState();

	/*�C�ӂ̏�Ԃ�������*/
	this->state->ClearFlag(this->MASK_ALWAYS_INITIALIZE);

	/*��������*/
	if (pad & PAD_INPUT_7)
	{
		this->state->SetFlag(this->CROUCH);
	}
	else
	{
		this->state->SetFlag(this->STAND);
	}

	/*�ړ�*/
	if (this->state->CheckFlag(this->MASK_CANT_MOVE))
	{
		//�X�e�B�b�N����
		this->lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
		//�X�e�B�b�N�����͂���Ă�����ړ����Ă���
		if (lStick.x != 0.0f || lStick.z != 0.0f)
		{
			//L�X�e�B�b�N�������݂�����΃_�b�V��
			if (pad & PAD_INPUT_9)
			{
				this->state->SetFlag(this->RUN);
			}
			else
			{
				this->state->SetFlag(this->WALK);
			}
		}
	}
	/*L�X�e�B�b�N�������݂�����΃��b�N�I��*/
	if (pad & PAD_INPUT_10)
	{
		this->state->SetFlag(this->LOCK_ON);
	}

	/*�W�����v*/
	if (this->state->CheckFlag(this->JUMP) && pad & PAD_INPUT_3)
	{
		this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
		this->state->SetFlag(this->JUMP);
	}

	/*�K�[�h*/
	if (input.GetPadState() & PAD_INPUT_5)
	{
		this->state->SetFlag(this->BLOCK);
	}

	/*���*/
	if (this->state->CheckFlag(this->MASK_CANT_AVOID) && input.GetPadState() & PAD_INPUT_4)
	{
		this->state->SetFlag(this->AVOID);
	}

	/*���K*/
	if (this->state->CheckFlag(this->MASK_CANT_ROAR) && input.GetPadState() & PAD_INPUT_7 && input.GetPadState() & PAD_INPUT_7)
	{
		this->state->SetFlag(this->AVOID);
	}

	/*�U��*/
	//���C���U��
	if (this->state->CheckFlag(this->MASK_CANT_MAIN_ATTACK))
	{
		//�W�����v���Ă�����
		if (this->state->CheckFlag(this->JUMP))
		{
			//�W�����v�U��
			if (input.GetPadState() & PAD_INPUT_1)
			{
				this->state->SetFlag(this->JUMP_ATTACK);
			}
			//�W�����v��]�U��
			else if (input.GetPadState() & PAD_INPUT_6)
			{
				this->state->SetFlag(this->JUMP_ROTATION_ATTACK);
			}
		}
		else
		{
			//��]�U��
			if (input.GetPadState() & PAD_INPUT_6)
			{
				this->state->SetFlag(this->JUMP_ATTACK);
			}
			//�r���U��
			else if (input.GetPadState() & PAD_INPUT_8)
			{
				this->state->SetFlag(this->JUMP_ATTACK);
			}
			//�ʏ�U��
			else if (input.GetPadState() & PAD_INPUT_1)
			{
				this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
				this->attackComboCount++;
				if (this->attackComboCount >= json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
				{
					this->attackComboCount = 0;
				}
			}
		}
	}
	//�T�u�U��
	if (this->state->CheckFlag(this->MASK_CANT_SUB_ATTACK))
	{
		//Y�����Ă���
		if (this->state->CheckFlag(this->BLOCK))
		{
			//�K�[�h���Ă�����
			if (this->state->CheckFlag(this->BLOCK))
			{
				this->state->SetFlag(this->KICK);
			}
			else
			{
				this->state->SetFlag(this->PUNCH);
			}
		}
	}
}