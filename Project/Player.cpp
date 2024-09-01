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
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_1, static_cast<int>(AnimationType::MAIN_1));
	this->attackAnimationMap.emplace(this->MAIN_ATTACK_2, static_cast<int>(AnimationType::MAIN_2));
	this->attackAnimationMap.emplace(this->SPECIAL_ATTACK, static_cast<int>(AnimationType::SPECIAL));
	this->attackTypeMap.emplace(this->MAIN_ATTACK_1, static_cast<int>(AttackType::MAIN_1));
	this->attackTypeMap.emplace(this->MAIN_ATTACK_2, static_cast<int>(AttackType::MAIN_2));
	this->attackTypeMap.emplace(this->SPECIAL_ATTACK, static_cast<int>(AttackType::SPECIAL));
	this->attackComboStateMap.emplace(0, this->MAIN_ATTACK_1);
	this->attackComboStateMap.emplace(1, this->MAIN_ATTACK_2);
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

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
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //�g�嗦

	/*�ϐ��̏�����*/
	this->direction = VGet(0.0f, 0.0f, -1.0f);
	capsuleColiderData->hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];	 //�g�嗦

	/*���f���̓ǂݍ���*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::PLAYER));

	/*�R���C�_�[�̏�����*/
	Collidable::Initialize(_physics);
	
	/*���������̏�����*/
	this->rigidbody.Initialize(true);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
	this->speed = json.GetJson(JsonManager::FileType::PLAYER)["SPEED"];

	this->state->SetFlag(this->IDLE);
	this->animation->Attach(&this->modelHandle);
	this->attackNumber = 0;
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
	isGround = false;
	for (const auto& object : hitObjects)
	{
		if (object->GetTag() == GameObjectTag::GROUND)
		{
			isGround = true;
			break;
		}
	}

	/*�A�N�V����*/
	LockOn();
	Block();
	Attack();
	Move();

	/*���������A�N�V���������Ă��Ȃ�������Idle������*/
	if (DontAnyAction()) { this->state->SetFlag(this->IDLE); }

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
	printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("PLAYER_SPEED:%f\n", this->speed);
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

	Avoid();

	/*���x�̍X�V*/
	UpdateSpeed();

	/*�ړ��x�N�g���̍X�V*/
	UpdateMoveVector();

	/*�����̍X�V*/
	//���b�N�I�����Ă��Ȃ�������
	if (!state->CheckFlag(this->LOCK_ON))
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
	/*�ړ��x�N�g��������������*/
	VECTOR direction = { 0.0f,0.0f,0.0f };

	/*�ړ����Ă���Ƃ���������Ă���Ƃ��͈ړ��x�N�g�����o��*/
	if (this->state->CheckFlag(this->MASK_MOVE) || this->state->CheckFlag(this->AVOID))
	{
		/*��]�������ƂɈړ��x�N�g�����o��*/
		direction = VGet(-sinf(this->moveVectorRotation.y), 0.0f, -cosf(this->moveVectorRotation.y));
		/*�ړ��x�N�g���𐳋K��*/
		direction = VNorm(direction);

	}
	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);

	this->rigidbody.SetVelocity(newVelocity);
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateSpeed()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->state->CheckFlag(this->AVOID))
	{
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["AVOID_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->WALK))
	{
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_VELOCITY"];
	}
	else if (this->state->CheckFlag(this->RUN))
	{
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_VELOCITY"];
	}
	else
	{
		this->speed = 0.0f;
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

		//LB���͂�����Ώ�Ԃ�؂�ւ���
		if (pad & PAD_INPUT_5)
		{
			this->state->ClearFlag(this->WALK);
			this->state->SetFlag(this->RUN);
		}
		else
		{
			this->state->ClearFlag(this->RUN);
			this->state->SetFlag(this->WALK);
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
	playerToBossDirection = VSub(enemy.GetPosition(), this->rigidbody.GetPosition());
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

	if (isInputLStick || this->state->CheckFlag(this->LOCK_ON))
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
/// �A�j���[�V�����̍X�V
/// </summary>
void Player::UpdateAnimation()
{
	/*���S�A�j���[�V����*/
	if (this->state->CheckFlag(this->DEATH))
	{
		this->nowAnimation = static_cast<int>(AnimationType::DEATH);
	}
	/*���A�N�V����*/
	else if (this->state->CheckFlag(this->REACTION))
	{
		this->nowAnimation = static_cast<int>(AnimationType::SMALL_IMPACT);
	}
	/*�U��*/
	else if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		this->nowAnimation = this->attackAnimationMap[this->attackType];
	}
	/*�K�[�h*/
	else if (this->state->CheckFlag(this->BLOCK))
	{
		this->nowAnimation = static_cast<int>(AnimationType::BLOCK);
	}
	/*���*/
	else if (this->state->CheckFlag(this->AVOID))
	{
		this->nowAnimation = static_cast<int>(AnimationType::AVOID);
	}
	/*�ړ�*/
	else if (this->state->CheckFlag(this->MASK_MOVE))
	{
		if (this->state->CheckFlag(this->RUN))
		{
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				if (this->lStick.x < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_LEFT);
				}
				else if (this->lStick.x > 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_RIGHT);
				}
				else if (this->lStick.z < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_RUN_BACK);
				}
				else
				{
					this->nowAnimation = static_cast<int>(AnimationType::RUN);
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::RUN);
			}
		}
		else
		{
			if (this->state->CheckFlag(this->LOCK_ON))
			{
				if (this->lStick.x < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_LEFT);
				}
				else if (this->lStick.x > 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_RIGHT);
				}
				else if (this->lStick.z < 0)
				{
					this->nowAnimation = static_cast<int>(AnimationType::LOCK_ON_WALK_BACK);
				}
				else
				{
					this->nowAnimation = static_cast<int>(AnimationType::WALK);
				}
			}
			else
			{
				this->nowAnimation = static_cast<int>(AnimationType::WALK);
			}
		}
	}
	/*�ҋ@*/
	else if (this->state->CheckFlag(this->IDLE))
	{
		this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	}
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
	int pad = input.GetPadState();

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
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (!CanBlock())return;

	if (pad & PAD_INPUT_7)
	{
		this->state->ClearFlag(this->MASK_MOVE);
		this->state->SetFlag(this->BLOCK);
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = true;
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = false;
	}
}

/// <summary>
/// ���
/// </summary>
void Player::Avoid()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();

	if (this->state->CheckFlag(this->AVOID) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->AVOID);
	}

	if (FrameCount(static_cast<int>(FrameCountType::AVOID), json.GetJson(JsonManager::FileType::PLAYER)["AVOID_MAX_FRAME"]))
	{
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = false;
	}


	if (!CanAvoid())return;

	if (pad & PAD_INPUT_4)
	{
		this->state->SetFlag(this->AVOID);
		this->isCount[static_cast<int>(FrameCountType::AVOID)] = true;
		auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
		capsuleColiderData->isCutDamage = true;
	}

}
/// <summary>
/// �U��
/// </summary>
void Player::Attack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();

	int pad = input.GetPadState();

	/*�U���A�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	if (!CanAttack())return;

	if (pad & PAD_INPUT_1)
	{
		this->state->ClearFlag(this->MASK_MOVE);
		this->state->SetFlag(this->attackComboStateMap[this->attackComboCount]);
		this->attackType = this->attackComboStateMap[this->attackComboCount];
		this->attackComboCount++;
		if (this->attackComboCount > json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_COMBO_NUM"])
		{
			this->attackComboCount = 0;
		}
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
		attack.OnIsStart(this->attackTypeMap[this->attackType]);
		this->attackNumber++;
	}
	else if (pad & PAD_INPUT_2)
	{
		this->state->ClearFlag(this->MASK_MOVE);
		this->state->SetFlag(this->SPECIAL_ATTACK);
		this->attackType = this->SPECIAL_ATTACK;
		this->attackComboCount = 0;
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = false;
		attack.OnIsStart(this->attackTypeMap[this->attackType]);
		this->attackNumber++;
	}

	/*��莞��X�������Ă��Ȃ�������R���{��r�؂ꂳ����*/
	if (this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)])
	{
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)]++;
		if (this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] >= json.GetJson(JsonManager::FileType::PLAYER)["CONBO_RESET_MAX_FRAME"])
		{
			this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;
			this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = false;
			this->attackComboCount = 0;
		}
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
	//if (!this->state->CheckFlag(this->BLOCK | this->AVOID))
	//{
	//	this->hp -= _damage;
	//	this->state->ClearFlag(this->MASK_ALL);
	//	if (_damage >= 20)
	//	{
	//		this->state->SetFlag(this->BIG_IMPACT);
	//	}
	//	else
	//	{
	//		this->state->SetFlag(this->SMALL_IMPACT);
	//	}
	//}
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

//HP�̎擾
const int Player::GetHP()const
{ 
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	return capsuleColiderData->GetHP();
}
