#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include <iostream>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "CharacterData.h"
#include "PlayerData.h"
#include "CharacterColliderData.h"
#include "Character.h"
#include "Animation.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Player.h"
#include "InputManager.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "PlayerAttackManager.h"
#include "Debug.h"
#include "EffectManager.h"
#include "HitStop.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
	: attackComboCount	(0)
	, healOrbNum		(0)
	, nowAnimation		(static_cast<int>(AnimationType::IDLE))
	, animationPlayTime	(0.0f)
	, moveVectorRotation(Gori::ORIGIN)
	, hitStop			(nullptr)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->hitStop = new HitStop();
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
	this->animationMap.emplace(this->IDLE			 , static_cast<int>(AnimationType::IDLE				));
	this->animationMap.emplace(this->AVOID			 , static_cast<int>(AnimationType::AVOID));
	this->animationMap.emplace(this->DEATH			 , static_cast<int>(AnimationType::DEATH			));
	this->animationMap.emplace(this->BLOCK			 , static_cast<int>(AnimationType::BLOCK			));
	this->animationMap.emplace(this->REACTION		 , static_cast<int>(AnimationType::REACTION			));
	this->animationMap.emplace(this->BLOCK_REACTION	 , static_cast<int>(AnimationType::BLOCK_REACTION	));
	this->animationMap.emplace(this->WALK_FRONT		 , static_cast<int>(AnimationType::WALK_FRONT		));
	this->animationMap.emplace(this->SLASH			 , static_cast<int>(AnimationType::SLASH			));
	this->animationMap.emplace(this->HEAL			 , static_cast<int>(AnimationType::HEAL));

	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::NORMAL)	 , this->REACTION);
	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::BLOW_BIG)	 , this->REACTION);
	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL), this->REACTION);

	/*�R���C�_�[�f�[�^�̍쐬*/
	CharacterData* data = new PlayerData;
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::PLAYER, data);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	this->frameCount.clear();
	this->isCount.clear();
}

/// <summary>
/// ������
/// </summary>
void Player::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json		= Singleton<JsonManager>::GetInstance();
	auto& asset		= Singleton<LoadingAsset>::GetInstance();
	auto& collider	= dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data		= dynamic_cast<PlayerData&>(*collider.data);

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//���W
	const VECTOR SCALE	  = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //�g�嗦
		  VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//��]��
		  rotation.y	  = rotation.y * 180.0f / DX_PI_F;
	/*�ϐ��̏�����*/
	this->isAlive			 = true;
	this->isDraw			 = true;
	this->isGround			 = false;
	this->isInitialize		 = true;
	this->speed				 = 0.0f;
	this->entryInterval		 = 0;
	this->moveVectorRotation = Gori::ORIGIN;
	this->nowAnimation		 = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime	 = 0.0f;
	this->attackComboCount	 = 0;
	this->healOrbNum		 = json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];	//�ő�񕜃I�[�u��
	for (int i = 0; i < this->frameCount.size(); i++)
	{
		this->frameCount[i] = 0;
		this->isCount[i] = false;
	}
	
	/*���f���̓ǂݍ���*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::PLAYER));

	/*�R���C�_�[�̏�����*/
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(rotation);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	float height		= json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];		//�J�v�Z���̍���
	collider.topPositon = /*VAdd(collider.rigidbody.GetPosition(), */VGet(0.0f, height, 0.0f)/*)*/;	//�J�v�Z���̏���W
	collider.radius		= json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];			//�J�v�Z���̔��a
	data.hp				= json.GetJson(JsonManager::FileType::PLAYER)["HP"];				//HP
	data.stamina		= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];			//�X�^�~�i
	data.isInvinvible	= false;															//�_���[�W���J�b�g���邩
	data.isGuard		= false;															//�_���[�W���J�b�g���邩
	data.isHit			= false;															//�U�����q�b�g������
	this->healOrbNum	= json.GetJson(JsonManager::FileType::PLAYER)["MAX_HEAL_ORB_NUM"];	//�ő�񕜃I�[�u��


	/*��Ԃ̏�����*/
	this->state->ClearFlag(this->DEATH);
	this->state->ClearFlag(this->MASK_ALL);
	this->state->SetFlag(this->IDLE);

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// �㏈��
/// </summary>
void Player::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->hitStop->IsHitStop()) return;

	/*�t���O�̏�����*/
	this->state->ClearFlag(this->MASK_ALWAYS_TURN_OFF);

	/*�A�N�V����*/
	Death();//�f�X����
	if (!this->state->CheckFlag(this->DEATH))
	{
		Reaction();//���A�N�V��������
		Attack();//�U������
		Move();//�ړ�����
		Rolling();//�������
		Block();//�h�䏈��
		Heal();//�񕜏���

	//���������A�N�V���������Ă��Ȃ�������Idle������
		if (DontAnyAction())
		{
			this->state->SetFlag(this->IDLE);
		}
		else
		{
			this->state->ClearFlag(this->IDLE);
		}

		//��Ԃ������܂��͑ҋ@�̎��̂݃X�^�~�i���񕜂���i���̑��X�^�~�i����͂��̏ꏊ�ōs���Ă���
		//�X�^�~�i�v�Z�̏ꏊ���U��΂��Ă���̂œ��ꂵ����
		if (!this->state->CheckFlag(this->MASK_CANT_RECOVERY_STAMINA))
		{
			CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);
		}
	}
	else
	{
		this->speed = 0.0f;
		VECTOR direction = this->collider->rigidbody.GetDirection();
		VECTOR aimVelocity = VScale(direction, this->speed);
		VECTOR prevVelocity = this->collider->rigidbody.GetVelocity();
		VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);
		this->collider->rigidbody.SetVelocity(newVelocity);
	}

	if (this->isDraw)
	{
		/*�A�j���[�V�����̍X�V*/
		UpdateAnimation();
		float animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
		VECTOR position = this->collider->rigidbody.GetPosition();
		this->animation->Play(&this->modelHandle, position, this->nowAnimation, animationPlayTime);
	}
}

/// <summary>
/// �`��
/// </summary>
const void Player::DrawCharacterInfo()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();

	VECTOR position = this->collider->rigidbody.GetPosition();
	VECTOR direction = this->collider->rigidbody.GetDirection();
	VECTOR rotation = this->collider->rigidbody.GetRotation();
	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER))
	{
		printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
		printfDx("PLAYER_DIRECTION X:%f,Y:%f,Z:%f	\n", direction.x, direction.y, direction.z);
		printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
		printfDx("PLAYER_SPEED:%f					\n", this->speed);
		printfDx("%d:IDLE							\n", this->state->CheckFlag(this->IDLE));
		printfDx("%d:AVOID						\n", this->state->CheckFlag(this->AVOID));
		printfDx("%d:DEATH						\n", this->state->CheckFlag(this->DEATH));
		printfDx("%d:BLOCK						\n", this->state->CheckFlag(this->BLOCK));
		printfDx("%d:REACTION						\n", this->state->CheckFlag(this->REACTION));
		printfDx("%d:BLOCK_REACTION				\n", this->state->CheckFlag(this->BLOCK_REACTION));
		printfDx("%d:WALK_FRONT					\n", this->state->CheckFlag(this->WALK_FRONT));
		printfDx("%d:SLASH						\n", this->state->CheckFlag(this->SLASH));
		printfDx("%d:HEAL							\n", this->state->CheckFlag(this->HEAL));
		auto& characterCollider = dynamic_cast<CharacterColliderData&> (*this->collider);
		printfDx("%d:REACTION_TYPE				\n", characterCollider.data->playerReaction);
		printfDx("DOT:%f\n"							, this->dot);

	}
	if (this->isDraw)
	{
		/*�����̕`��*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
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
		//direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));
		/*�ړ��x�N�g���𐳋K��*/
		direction = VNorm(direction);
	}
	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = this->collider->rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);

	this->collider->rigidbody.SetVelocity(newVelocity);
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
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
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
/// ��]���̍X�V
/// </summary>
void Player::UpdateRotation()
{
	/*�ړ��ł��邩*/
	if (!CanRotation())return;

	/*������*/
	const float PI = 180.0f;						//�ʓx�@�ł̃�
	VECTOR		rotation = VGet(0.0f, 0.0f, 0.0f);	//��]��
	bool		isInputLStick = false;						//L�X�e�B�b�N����
	VECTOR		cameraDirection = VGet(0.0f, 0.0f, 0.0f);	//�J�����̌���
	VECTOR		playerToTargetDirection = VGet(0.0f, 0.0f, 0.0f);	//�J�����̌���
	VECTOR		wasd = VGet(0.0f, 0.0f, 0.0f);	//wasd����
	VECTOR		lStick = VGet(0.0f, 0.0f, 0.0f);	//lStick����(��:Z+ ��:Z- ��:x- �E:x+)
	this->state->ClearFlag(this->MASK_MOVE);

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*�p�b�h���͂̎擾*/
	int pad = input.GetPadState();
	//�X�e�B�b�N����
	lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));

	/*�ړ���Ԃ̐؂�ւ�*/
	//�X�e�B�b�N���͂����邩
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;
		/*x��z�̒l���傫���ق��Ŕ��肷��*/
		this->state->SetFlag(this->WALK_FRONT);
	}


	/*�J�����̌������o��*/
	//�J�������W����v���C���[�̍��W�ւ̃x�N�g�����o��
	cameraDirection = camera.GetCameraDirection();
	//���߂��x�N�g���𐳋K������
	cameraDirection = VNorm(cameraDirection);

	///*�v���C���[����{�X�ւ̌������o��*/
	////�{�X���W����v���C���[�̍��W�ւ̃x�N�g�����o��
	//playerToTargetDirection = VSub(enemy.GetRigidbody().GetPosition(), this->collider->rigidbody.GetPosition());
	////���߂��x�N�g���𐳋K������
	//playerToTargetDirection = VNorm(playerToTargetDirection);


	/*�������b�N�I�����Ă�����*/
		/*�J�����̌����Ă�������ƁA�v���C���[���ŏ��Ɍ����Ă������������ƂɃ��f���̉�]�����o���B*/
	if (isInputLStick)
	{
		lStick = VNorm(lStick);
		rotation.y = static_cast<float>(
			-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))/* - 90.0f * (DX_PI_F / 180.0f);*/
			- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
		//this->moveVectorRotation.y = static_cast<float>(
		//	- atan2(static_cast<double>(playerToTargetDirection.z), static_cast<double>(playerToTargetDirection.x))
		//	- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
		this->moveVectorRotation = rotation;
	}

	if (isInputLStick)
	{
		this->collider->rigidbody.SetRotation(rotation);
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
const bool Player::GetIsAttack()const
{
	return this->state->CheckFlag(this->SLASH);
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
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& effect = Singleton<EffectManager>  ::GetInstance();
	
	if (this->state->CheckFlag(this->MASK_REACTION) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ALL);
	}

	/*�q�b�g�ԍ����擾*/
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);

	/*���݂̃q�b�g�ԍ��ƑO�̃q�b�g�ԍ����قȂ��Ă�����*/
	if (collider.data->isHit)
	{
		//�K�[�h��
		//���ς��萔�ȓ� & �K�[�h�� & �K�[�h�ɕK�v�ȃX�^�~�i������Ă���
		if (this->state->CheckFlag(this->BLOCK) && collider.data->isGuard)
		{
			if (collider.data->isInvinvible)
			{
				//effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
				this->state->ClearFlag(this->MASK_ALL);
				this->state->SetFlag(this->BLOCK_REACTION);
				CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["STAMINA_RECOVERY_VALUE"]);
				this->isCount[static_cast<int>(FrameCountType::JUST_AVOID)] = false;
				this->frameCount[static_cast<int>(FrameCountType::JUST_AVOID)] = 0;

			}
			else
			{
				effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
				this->state->ClearFlag(this->MASK_ALL);
				this->state->SetFlag(this->BLOCK_REACTION);
				CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]);
			}
		}
		else
		{
			auto& data = dynamic_cast<PlayerData&>(*collider.data);
			//���G����Ȃ����
			if (!data.isInvinvible)
			{
				auto& effect = Singleton<EffectManager>::GetInstance();
				effect.OnIsEffect(EffectManager::EffectType::BOSS_IMPACT);

				this->state->ClearFlag(this->MASK_ALL);
				this->state->SetFlag(this->reactionMap[data.playerReaction]);
				this->speed = json.GetJson(JsonManager::FileType::PLAYER)["REACTION_SPEED"][data.playerReaction];
				this->hitStop->SetHitStop
				(
					data.hitStopTime,
					data.hitStopType,
					data.hitStopDelay,
					data.slowFactor
				);
			}
			else
			{
				CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_RECOVERY_VALUE"]);
			}
		}
		collider.data->isHit = false;
	}
}

/// <summary>
/// �f�X
/// </summary>
void Player::Death()
{
	if (!this->isAlive) return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	int pad = input.GetPadState();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);

	if (this->state->CheckFlag(this->DEATH))
	{
		if (this->animation->GetIsChangeAnim())
		{
			this->isAlive = false;
			this->isDraw = false;
		}
	}
	else
	{
		/*����HP���O������������*/
		if (collider.data->hp < 0)
		{
			this->state->ClearFlag(this->MASK_ALL);
			this->state->SetFlag(this->DEATH);
		}
	}
}

/// <summary>
/// �u���b�N
/// </summary>
void Player::Block()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input		= Singleton<InputManager> ::GetInstance();
	auto& json		= Singleton<JsonManager>  ::GetInstance();
	auto& collider	= dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data		= dynamic_cast<PlayerData&>(*collider.data);

	/*pad����*/
	int pad = input.GetPadState();

	/*�t���[�����p�t���[���J�E���g���ő�ɒB���Ă����疳�G�t���O������*/
	if (FrameCount(static_cast<int>(FrameCountType::JUST_BLOCK), json.GetJson(JsonManager::FileType::PLAYER)["JUST_BLOCK_MAX_FRAME"]))
	{
		auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
		auto& data = dynamic_cast<PlayerData&>(*collider.data);
		data.isInvinvible = false;
	}

	/*�u���b�N�ł��邩*/
	if (!CanBlock())return;

	/*LT�������ꂽ��*/
	if (pad & PAD_INPUT_7)
	{
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		VECTOR enemyFirstDirection = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
		VECTOR playerFirstDirection = Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
		VECTOR enemyDirection = VTransform(enemyFirstDirection, MGetRotY(enemy.GetRigidbody().GetRotation().y));
		VECTOR playerDirection = VTransform(playerFirstDirection, MGetRotY(collider.rigidbody.GetRotation().y));

		float TOLERANCE_DOT = json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];

		VECTOR e = VNorm(enemyDirection);
		VECTOR p = VNorm(playerDirection);
		this->dot = VDot(e, p);
		this->dot = this->dot * 180.0f / DX_PI_F;

		this->state->SetFlag(this->BLOCK);

		if (this->dot >= TOLERANCE_DOT &&
			CanAction(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]))
		{
			this->isCount[static_cast<int>(FrameCountType::JUST_AVOID)] = true;
			data.isGuard = true;
			data.isInvinvible = true;
		}
		else
		{
			data.isGuard = false;
			data.isInvinvible = false;
			this->isCount[static_cast<int>(FrameCountType::JUST_AVOID)] = false;
			this->frameCount[static_cast<int>(FrameCountType::JUST_AVOID)] = 0;
		}
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
		data.isGuard = false;
		data.isInvinvible = false;
		this->isCount[static_cast<int>(FrameCountType::JUST_AVOID)] = false;
		this->frameCount[static_cast<int>(FrameCountType::JUST_AVOID)] = 0;
	}
}

/// <summary>
/// ��
/// </summary>
void Player::Heal()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager>	::GetInstance();
	auto& json = Singleton<JsonManager>		::GetInstance();
	auto& debug = Singleton<Debug>			::GetInstance();
	auto& effect = Singleton<EffectManager> ::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);

	/*���G�t���O�������Ă�����ő�HP����ς��Ȃ�*/
	if (debug.IsShowDebugInfo(Debug::ItemType::PLAYER) && json.GetJson(JsonManager::FileType::DEBUG)["PLAYER_INVINCIBLE"])
	{
		data.hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
	}

	/*�񕜃A�j���[�V�������I�����Ă�����*/
	if (this->state->CheckFlag(this->HEAL) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->HEAL);
	}

	/*pad����*/
	int pad = input.GetPadState();
	bool isInputY = (pad & PAD_INPUT_2);

	/*�񕜂��Ă�����*/
	if (this->isCount[static_cast<int>(FrameCountType::HEAL)])
	{
		/*�񕜂̃C���^�[�o�����v�Z*/
		FrameCount(static_cast<int>(FrameCountType::HEAL), json.GetJson(JsonManager::FileType::PLAYER)["HEAL_INTERVAL_FRAME"]);
	}

	/*�񕜂ł��邩*/
	if (!CanHeal()) return;

	/*�O�ɉ񕜂��Ă��Ȃ�����Y�{�^���������ꂽ��*/
	if (isInputY)
	{
		if (!this->state->CheckFlag(this->HEAL))
		{
			effect.OnIsEffect(EffectManager::EffectType::PLAYER_HEAL);
			this->state->ClearFlag(this->BLOCK);
			this->state->SetFlag(this->HEAL);
			data.hp += json.GetJson(JsonManager::FileType::PLAYER)["HEAL_VALUE"];
			this->healOrbNum--;
			this->isCount[static_cast<int>(FrameCountType::HEAL)] = true;
			//�ő�HP�𒴂��Ȃ��悤�ɂ���
			if (data.hp >= json.GetJson(JsonManager::FileType::PLAYER)["HP"])
			{
				data.hp = json.GetJson(JsonManager::FileType::PLAYER)["HP"];
			}
		}
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

	/*������Ă�����*/
	if (this->state->CheckFlag(this->MASK_AVOID))
	{
		//�A�j���[�V�������I�����Ă�����
		if (this->animation->GetIsChangeAnim())
		{
			this->state->ClearFlag(this->MASK_AVOID);
		}
	}

	/*�t���[�����p�t���[���J�E���g���ő�ɒB���Ă����疳�G�t���O������*/
	if (FrameCount(static_cast<int>(FrameCountType::JUST_AVOID), json.GetJson(JsonManager::FileType::PLAYER)["JUST_AVOID_MAX_FRAME"]))
	{
		auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
		auto& data = dynamic_cast<PlayerData&>(*collider.data);
		data.isInvinvible = false;
	}

	/*������ł��邩*/
	if (!CanRolling())return;

	/*����X�^�~�i�͑����̂�*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]))return;

	if ((pad & PAD_INPUT_4) && !this->isCount[static_cast<int>(FrameCountType::AVOID)])
	{
		this->state->ClearFlag(this->MASK_ATTACK);
		this->state->SetFlag(this->AVOID);
		this->isCount[static_cast<int>(FrameCountType::JUST_AVOID)] = true;
		auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
		auto& data = dynamic_cast<PlayerData&>(*collider.data);
		data.isInvinvible = true;
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

	/*����X�^�~�i�͑����̂�*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_STAMINA_CONSUMPTION"]))return;

	/*X�������ꂽ��*/
	if (pad & PAD_INPUT_1)
	{
		this->state->SetFlag(this->SLASH);
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_STAMINA_CONSUMPTION"]);
		attack.OnIsStart();
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
	if (this->state->CheckFlag(this->MASK_REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH)	)			return false;//�f�X
	if (this->state->CheckFlag(this->MASK_AVOID))		return false;//���
	if (this->state->CheckFlag(this->MASK_ATTACK))		return false;//�U��
	if (this->state->CheckFlag(this->BLOCK))			return false;//�h��
	if (this->state->CheckFlag(this->HEAL))				return false;//��
	return true;
}
const bool Player::CanRolling()const
{
	//if (this->state->CheckFlag(this->MASK_ATTACK))		return false;//�U��
	if (this->state->CheckFlag(this->BLOCK))			return false;//�u���b�N
	if (this->state->CheckFlag(this->MASK_REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))			return false;//���S
	if (this->state->CheckFlag(this->MASK_AVOID))		return false;//���
	if (this->state->CheckFlag(this->HEAL))				return false;//��

	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))			return false;//�f�X
	if (this->state->CheckFlag(this->MASK_AVOID))		return false;//���
	if (this->state->CheckFlag(this->SLASH))			return false;//���
	if (this->state->CheckFlag(this->HEAL))				return false;//��
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))			return false;//�f�X
	if (this->state->CheckFlag(this->MASK_AVOID))		return false;//���
	//if (this->state->CheckFlag(this->SLASH))			return false;//�U��
	if (this->state->CheckFlag(this->HEAL))				return false;//��
	return true;
}
const bool Player::CanHeal()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))		return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))				return false;//�f�X
	if (this->state->CheckFlag(this->MASK_AVOID))			return false;//���
	if (this->state->CheckFlag(this->MASK_ATTACK))			return false;//�U��
	if (this->healOrbNum == 0)									return false;
	if (this->isCount[static_cast<int>(FrameCountType::HEAL)])	return false;

	return true;
}
const bool Player::DontAnyAction()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))		return false;//�U��
	if (this->state->CheckFlag(this->MASK_REACTION))	return false;//���A�N�V����
	if (this->state->CheckFlag(this->MASK_AVOID))		return false;//���
	if (this->state->CheckFlag(this->DEATH))			return false;//�u���b�N
	if (this->state->CheckFlag(this->MASK_MOVE))		return false;//�ړ�
	if (this->state->CheckFlag(this->BLOCK))			return false;//�W�����v
	if (this->state->CheckFlag(this->HEAL))				return false;//��

	return true;
}

/// <summary>
/// �K�v�ȏ���X�^�~�i�ƌ��݂̃X�^�~�i�̎c�ʂ��ׂăA�N�V�������\�Ȃ�true��Ԃ�
/// </summary>
/// <returns></returns>
const bool Player::CanAction(const float _staminaConsumed)const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);
	float staminaConsumed = _staminaConsumed * -1.0f;
	/*�X�^�~�i�̏���ʂ����݂̃X�^�~�i�̑��ʂ����������false��Ԃ�*/
	if (staminaConsumed > data.stamina)return false;
	return true;
}

/// <summary>
/// �X�^�~�i�̉񕜏���
/// </summary>
void Player::CalcStamina(const float _staminaConsumed)
{
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);

	/*����ȊO�̏�Ԃ��������Ԃɉ����ăX�^�~�i�������*/
	//����
	data.stamina += _staminaConsumed;
	/*����l�A�����l�𒴂��Ȃ��悤�ɒ���*/
	if (data.stamina >= json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"])
	{
		data.stamina = json.GetJson(JsonManager::FileType::PLAYER)["STAMINA"];
	}
	else if (data.stamina < 0)
	{
		data.stamina = 0;
	}
}
const int Player::GetStamina()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);
	return data.stamina;
}