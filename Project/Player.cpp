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
	, isDecSpeed (false)
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
	vector<int> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);
	//�A�j���[�V�����}�b�v�̐ݒ�
	this->animationMap.emplace(this->IDLE			, static_cast<int>(AnimationType::IDLE				));
	this->animationMap.emplace(this->AVOID			, static_cast<int>(AnimationType::AVOID				));
	this->animationMap.emplace(this->DEATH			, static_cast<int>(AnimationType::DEATH				));
	this->animationMap.emplace(this->BLOCK			, static_cast<int>(AnimationType::BLOCK				));
	this->animationMap.emplace(this->REACTION		, static_cast<int>(AnimationType::REACTION			));
	this->animationMap.emplace(this->BLOCK_REACTION	, static_cast<int>(AnimationType::BLOCK_REACTION	));
	this->animationMap.emplace(this->WALK_FRONT		, static_cast<int>(AnimationType::WALK_FRONT		));
	this->animationMap.emplace(this->WALK_BACK		, static_cast<int>(AnimationType::WALK_BACK			));
	this->animationMap.emplace(this->WALK_LEFT		, static_cast<int>(AnimationType::WALK_LEFT			));
	this->animationMap.emplace(this->WALK_RIGHT		, static_cast<int>(AnimationType::WALK_RIGHT		));
	this->animationMap.emplace(this->RUN_FRONT		, static_cast<int>(AnimationType::RUN_FRONT			));
	this->animationMap.emplace(this->RUN_BACK		, static_cast<int>(AnimationType::RUN_BACK			));
	this->animationMap.emplace(this->RUN_LEFT		, static_cast<int>(AnimationType::RUN_LEFT			));
	this->animationMap.emplace(this->RUN_RIGHT		, static_cast<int>(AnimationType::RUN_RIGHT			));
	this->animationMap.emplace(this->COMBO_1		, static_cast<int>(AnimationType::COMBO_1			));
	this->animationMap.emplace(this->COMBO_2		, static_cast<int>(AnimationType::COMBO_2			));
	this->animationMap.emplace(this->COMBO_3		, static_cast<int>(AnimationType::COMBO_3			));
	this->animationMap.emplace(this->SKILL			, static_cast<int>(AnimationType::SKILL				));
	this->animationMap.emplace(this->HEAL			, static_cast<int>(AnimationType::HEAL				));
	this->animationMap.emplace(this->DOWN			, static_cast<int>(AnimationType::DOWN_REACTION		));
	this->animationMap.emplace(this->DOWN_UP		, static_cast<int>(AnimationType::DOWN_UP			));

	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::NORMAL)	 , this->REACTION);
	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::BLOW_BIG)	 , this->DOWN);
	this->reactionMap.emplace(static_cast<int>(Gori::PlayerReactionType::BLOW_SMALL) , this->REACTION);
	
	this->attackMap.emplace(static_cast<int>(AttackType::COMBO_1) , this->COMBO_1);
	this->attackMap.emplace(static_cast<int>(AttackType::COMBO_2) , this->COMBO_2);
	this->attackMap.emplace(static_cast<int>(AttackType::COMBO_3) , this->COMBO_3);
	this->attackMap.emplace(static_cast<int>(AttackType::SKILL) , this->SKILL);


	this->whenRunMoveState.emplace(this->WALK_FRONT, this->RUN_FRONT);
	this->whenRunMoveState.emplace(this->WALK_BACK, this->RUN_BACK);
	this->whenRunMoveState.emplace(this->WALK_LEFT, this->RUN_LEFT);
	this->whenRunMoveState.emplace(this->WALK_RIGHT, this->RUN_RIGHT);

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
	this->animationMap.clear();
	this->reactionMap.clear();
	this->whenRunMoveState.clear();
	DeleteMemberInstance(this->hitStop);
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
		  rotation.y	  = rotation.y * (DX_PI_F / 180.0f);
	/*�ϐ��̏�����*/
	this->isAlive			 = true;
	this->isDraw			 = true;
	this->isGround			 = false;
	this->isInitialize		 = true;
	this->isBlockingMove	 = false;
	this->isStopAnimation	 = false;
	this->speed				 = 0.0f;
	this->entryInterval		 = 0;
	this->moveVectorRotation = Gori::ORIGIN;
	this->nextRotation = Gori::ORIGIN;
	this->nowAnimation		 = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime	 = 0.0f;
	this->attackComboCount	 = static_cast<int>(AttackType::NONE);
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
		LockOn	();//���b�N�I��
		Reaction();//���A�N�V��������
		Attack	();//�U������
		Move	();//�ړ�����
		Rolling	();//�������
		Block	();//�h�䏈��
		Heal	();//�񕜏���

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

	/*�`��t���O�������Ă�����*/
	if (this->isDraw)
	{
		//�A�j���[�V�����̍X�V
		UpdateAnimation();
		float animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
		//���������Ă���Ƃ��ɃX�^�~�i���Ȃ��Ȃ�΁A�Đ����x��x������
		if (this->state->CheckFlag(this->MASK_RUN) && !CanAction(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
		{
			animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_ANIMATION_SPEED"];
		}
		VECTOR position = this->collider->rigidbody.GetPosition();
		//�����_�E�����ɃA�j���[�V�����Đ����I�����Ă�����
		if (this->state->CheckFlag(this->DOWN) && this->animation->GetIsChangeAnim())return;
		if (this->isStopAnimation) return;
		//�A�j���[�V�����̍Đ�
		this->animation->Play(&this->modelHandle, this->nowAnimation, animationPlayTime);
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
		auto& characterCollider = dynamic_cast<CharacterColliderData&> (*this->collider);
		printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f	\n", position.x, position.y, position.z);
		printfDx("PLAYER_DIRECTION X:%f,Y:%f,Z:%f	\n", direction.x, direction.y, direction.z);
		printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f	\n", rotation.x, rotation.y, rotation.z);
		printfDx("PLAYER_SPEED:%f					\n", this->speed);
		printfDx("%d:IDLE							\n", this->state->CheckFlag(this->IDLE));
		printfDx("%d:AVOID							\n", this->state->CheckFlag(this->AVOID));
		printfDx("%d:DEATH							\n", this->state->CheckFlag(this->DEATH));
		printfDx("%d:BLOCK							\n", this->state->CheckFlag(this->BLOCK));
		printfDx("%d:REACTION						\n", this->state->CheckFlag(this->REACTION));
		printfDx("%d:BLOCK_REACTION					\n", this->state->CheckFlag(this->BLOCK_REACTION));
		printfDx("%d:DOWN							\n", this->state->CheckFlag(this->DOWN));
		printfDx("%d:DOWN_UP						\n", this->state->CheckFlag(this->DOWN_UP));
		printfDx("%d:WALK_FRONT						\n", this->state->CheckFlag(this->WALK_FRONT));
		printfDx("%d:WALK_BACK						\n", this->state->CheckFlag(this->WALK_BACK));
		printfDx("%d:WALK_LEFT						\n", this->state->CheckFlag(this->WALK_LEFT));
		printfDx("%d:WALK_RIGHT						\n", this->state->CheckFlag(this->WALK_RIGHT));
		printfDx("%d:RUN_FRONT						\n", this->state->CheckFlag(this->RUN_FRONT));
		printfDx("%d:RUN_BACK						\n", this->state->CheckFlag(this->RUN_BACK));
		printfDx("%d:RUN_LEFT						\n", this->state->CheckFlag(this->RUN_LEFT));
		printfDx("%d:RUN_RIGHT						\n", this->state->CheckFlag(this->RUN_RIGHT));
		printfDx("%d:COMBO_1						\n", this->state->CheckFlag(this->COMBO_1));
		printfDx("%d:COMBO_2						\n", this->state->CheckFlag(this->COMBO_2));
		printfDx("%d:COMBO_3						\n", this->state->CheckFlag(this->COMBO_3));
		printfDx("%d:SKILL							\n", this->state->CheckFlag(this->SKILL));
		printfDx("%d:HEAL							\n", this->state->CheckFlag(this->HEAL));
		printfDx("%d:LOCK_ON						\n", this->isLockOn);
		printfDx("%d:REACTION_TYPE					\n", characterCollider.data->playerReaction);
		printfDx("DOT:%f							\n", this->dot);
		printfDx("DEG:%f							\n", this->deg);


	}
	
	/*�����̕`��*/
	if (this->isDraw)
	{
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
	VECTOR direction = Gori::ORIGIN;
	VECTOR rotation = Gori::ORIGIN;
	/*�ړ����Ă���Ƃ���������Ă���Ƃ��͈ړ��x�N�g�����o��*/
	if (this->state->CheckFlag(this->MASK_CAN_VELOCITY) || this->isBlockingMove)
	{
		rotation = this->moveVectorRotation;
		direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));
		//direction = VGet(-sinf(ROTATION.y), 0.0f, -cosf(ROTATION.y));
		/*�ړ��x�N�g���𐳋K��*/
		direction = VNorm(direction);
	}

	/*�V����velocity���o��*/
	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = this->collider->rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);
	
	/*�����Ă����烉�[�v�ŕ⊮����*/
	if (this->state->CheckFlag(this->MASK_RUN))
	{
		//�␳�l
		const VECTOR LERP_VALUE = Convert(json.GetJson(JsonManager::FileType::CAMERA)["LERP_VALUE_VELOCITY"]);
		VECTOR larpVelocity = Lerp(prevVelocity, aimVelocity, LERP_VALUE);
	}

	/*�_�E�����A�N�V������������velocity�𔽑΂Ɍ�����*/
	if (this->state->CheckFlag(this->DOWN))
	{
		newVelocity = VScale(newVelocity, -1.0f);
		newVelocity.y = prevVelocity.y;
	}

	/*velocity���Z�b�g*/
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
	if (this->isBlockingMove)
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
	}
	else if (this->state->CheckFlag(this->MASK_WALK))
	{
		maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["WALK_SPEED"];
	}
	else if (this->state->CheckFlag(this->MASK_RUN))
	{
		if (CanAction(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]))
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["RUN_SPEED"];
		}
		else
		{
			maxSpeed = json.GetJson(JsonManager::FileType::PLAYER)["NONE_STAMINA_RUN_SPEED"];
			this->animationPlayTime = 0.3f;
		}
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["RUN_STAMINA_CONSUMPTION"]);
	}


	VECTOR		nowRotation = this->collider->rigidbody.GetRotation();
	float		nowDeg		= (nowRotation.y * 180.0f / DX_PI_F);
	float		nextDeg		= (this->nextRotation.y * 180.0f / DX_PI_F);
	const float PI_2		= 360.0f;
	if (nextDeg < 0.0f) { nextDeg += PI_2; }
	float diff = nowDeg - nextDeg;
	if (diff < 0.0f) { diff *= -1.0f; }
	/*�����萔�ȏ�Ȃ猸��������*/
	if (diff >= 180.0f)
	{
		if (!this->isDecSpeed)
		{
			maxSpeed = 0.0f;
			this->isDecSpeed = true;
		}
	}
	else
	{
		this->isDecSpeed = false;
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
	///*������𒆂�������ړ��p��]���̌����Ɍ�����*/
	//if (this->state->CheckFlag(this->MASK_AVOID))
	//{
	//	this->collider->rigidbody.SetRotation(this->moveVectorRotation);
	//	return;
	//}

	/*������*/
	const float PI = 180.0f;						//�ʓx�@�ł̃�
	VECTOR		nowRotation = this->collider->rigidbody.GetRotation();	//��]��
	bool		isInputLStick = false;						//L�X�e�B�b�N����
	VECTOR		cameraDirection = Gori::ORIGIN;	//�J�����̌���
	VECTOR		playerToTargetDirection = Gori::ORIGIN;	//�J�����̌���
	VECTOR		wasd = Gori::ORIGIN;	//wasd����
	VECTOR		lStick = Gori::ORIGIN;	//lStick����(��:Z+ ��:Z- ��:x- �E:x+)
	this->state->ClearFlag(this->MASK_MOVE);

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();


	/*�ړ��ł��邩*/
	if (CanRotation())
	{
		/*�p�b�h���͂̎擾*/
		int pad = input.GetPadState();

		/*�X�e�B�b�N����*/
		lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));

		/*��������*/
		bool isRun = (input.GetPadState() & PAD_INPUT_6);
		this->isBlockingMove = false;

			//�X�e�B�b�N���͂����邩
			if (lStick.x != 0.0f || lStick.z != 0.0f)
			{
				isInputLStick = true;
				if (this->state->CheckFlag(this->BLOCK))
				{
					this->isBlockingMove = true;
				}
				/*�����U�������Ă��Ȃ���Έړ���Ԃ�؂�ւ���*/
				else if (!this->state->CheckFlag(this->MASK_ATTACK | this->MASK_AVOID))
				{
					unsigned int moveState = 0;
					//�����Ă�����
					if (isRun)
					{
						//moveState = this->whenRunMoveState[moveState];
						moveState = this->RUN_FRONT;
					}
					else
					{
						////���b�N�I�����Ă�����
						//if (this->isLockOn)
						//{
						//	//X�̂ق������͂���Ă���l���傫�����
						//	if (lStick.x * lStick.x > lStick.z * lStick.z)
						//	{
						//		//�E
						//		if (lStick.x > 0.0f)
						//		{
						//			moveState = this->WALK_RIGHT;
						//		}
						//		//��
						//		else
						//		{
						//			moveState = this->WALK_LEFT;
						//		}
						//	}
						//	else
						//	{
						//		//�O
						//		if (lStick.z < 0.0f)
						//		{
						moveState = this->WALK_FRONT;
						//		}
						//		//���
						//		else
						//		{
						//			moveState = this->WALK_BACK;
						//		}
						//	}
						//}
						//else
						//{
						//	moveState = this->WALK_FRONT;
						//}
					}
					//��Ԃ̃Z�b�g
					this->state->ClearFlag(this->MASK_REACTION);
					this->state->SetFlag(moveState);
				}
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


		/*�J�����̌����Ă�������ƁA�v���C���[���ŏ��Ɍ����Ă������������ƂɃ��f���̉�]�����o���B*/
		if (isInputLStick)
		{
			//�X�e�B�b�N���͂𐳋K��
			lStick = VNorm(lStick);

			//if (this->isLockOn && !isRun)
			//{
			//	nextRotation.y = static_cast<float>(
			//		-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x)) - 90.0f * (DX_PI_F / 180.0f));
			//}
			//else
			//{
			this->nextRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
			//}

			//this->moveVectorRotation.y = static_cast<float>(
			//	-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
			//	- atan2(-static_cast<double>(lStick.z), static_cast<double>(lStick.x)));
			//this->moveVectorRotation = nextRotation;
			this->attackComboCount = -1;
		}

		/*�X�e�B�b�N���͂Ȃ���Α������^�[��*/
		else return;
	}

	/*���݂̉�]�����o��*/
	VECTOR lerpValue = Convert(json.GetJson(JsonManager::FileType::PLAYER)["ROTATE_LERP_VALUE"]);
	nowRotation = Lerp360Angle(nowRotation, this->nextRotation, lerpValue);
	
	/*��]���̃Z�b�g*/
	this->moveVectorRotation = nowRotation;
	this->collider->rigidbody.SetRotation(nowRotation);

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
	return this->state->CheckFlag(this->MASK_ATTACK);
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
	
	/*���A�N�V�����L�����Z���̔���*/
	if (FrameCount(static_cast<int>(FrameCountType::REACTION_CANSEL), json.GetJson(JsonManager::FileType::PLAYER)["REACTION_CANCEL_MAX_FRAME"]))
	{
		this->isCancelReaction = true;
	}

	/*�_�E�����ɃA�j���[�V�������I�����Ă�����*/
	if (this->state->CheckFlag(this->DOWN) && this->animation->GetIsChangeAnim())
	{
		//�V���O���g���N���X�̃C���X�^���X�̎擾
		auto& input = Singleton<InputManager> ::GetInstance();
		//�X�e�B�b�N����
		VECTOR lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
		if (lStick.x != 0.0f || lStick.z != 0.0f)
		{
			this->state->ClearFlag(this->DOWN);
			this->state->SetFlag(this->DOWN_UP);
		}
	}

	/*���A�N�V�������ɃA�j���[�V�����̍Đ����I�����Ă�����t���O������*/
	else if (this->state->CheckFlag(this->MASK_REACTION) && this->animation->GetIsChangeAnim())
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
			effect.OnIsEffect(EffectManager::EffectType::PLAYER_GUARD_HIT);
			this->state->ClearFlag(this->MASK_ALL);
			this->state->SetFlag(this->BLOCK_REACTION);
			CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]);
			this->animation->SetAddRate(0.05f);
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

		}
		collider.data->isHit = false;
	}

	/*�_�E�����Ȃ疳�G�t���O�𗧂Ă�*/
	if (this->state->CheckFlag(this->DOWN))
	{
		collider.data->isInvinvible = true;
		//�R���{���Z�b�g�t���O�𗧂Ă�
		this->isCount[static_cast<int>(FrameCountType::REACTION_CANSEL)] = true;
	}
	else
	{
		collider.data->isInvinvible = false;
	}
	if (!this->state->CheckFlag(this->DOWN_UP))
	{
		this->isCancelReaction = false;
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

	/*�u���b�N�ł��邩*/
	if (!CanBlock())return;

	/*RT�������ꂽ��*/
	if (pad & PAD_INPUT_8)
	{
		//�G�̃C���X�^���X���擾
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		//�G�ƃv���C���[�̌����̓��ς��v�Z���A�萔�ȓ��Ȃ�K�[�h����������
		VECTOR enemyFirstDirection	= Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_DIRECTION"]);
		VECTOR playerFirstDirection = Convert(json.GetJson(JsonManager::FileType::PLAYER)["FIRST_DIRECTION"]);
		VECTOR enemyDirection		= VTransform(enemyFirstDirection, MGetRotY(enemy.GetRigidbody().GetRotation().y));
		VECTOR playerDirection		= VTransform(playerFirstDirection, MGetRotY(collider.rigidbody.GetRotation().y));
		float  TOLERANCE_DOT		= json.GetJson(JsonManager::FileType::PLAYER)["TOLERANCE_DOT"];
			   enemyDirection		= VNorm(enemyDirection);
			   playerDirection		= VNorm(playerDirection);
		this->dot = VDot(enemyDirection, playerDirection);
		this->dot = this->dot * 180.0f / DX_PI_F;
		if (this->dot < 0) { this->dot *= -1.0f; }

		//�U���t���O������
		this->state->ClearFlag(this->MASK_ATTACK);
		this->state->SetFlag(this->BLOCK);

		if (this->dot >= TOLERANCE_DOT &&
			CanAction(json.GetJson(JsonManager::FileType::PLAYER)["BLOCK_STAMINA_CONSUMPTION"]))
		{
			if (!data.isGuard)
			{
				data.isGuard = true;
			}
		}
		else
		{
			data.isGuard = false;
		}
		this->animation->SetAddRate(0.3f);
	}
	else
	{
		this->state->ClearFlag(this->BLOCK);
		data.isGuard = false;
		this->animation->SetAddRate(0.05f);
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
	bool isInputY = (pad & PAD_INPUT_1);

	/*�񕜂��Ă�����*/
	if (this->isCount[static_cast<int>(FrameCountType::HEAL)])
	{
		/*�񕜂̃C���^�[�o�����v�Z*/
		FrameCount(static_cast<int>(FrameCountType::HEAL), json.GetJson(JsonManager::FileType::PLAYER)["HEAL_INTERVAL_FRAME"]);
	}

	/*�񕜂ł��邩*/
	if (!CanHeal()) return;

	/*�O�ɉ񕜂��Ă��Ȃ�����X�{�^���������ꂽ��*/
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

	/*�������]�p�t���[���J�E���g�̔���*/
	FrameCount(static_cast<int>(FrameCountType::AVOID_CAN_ROTATE), json.GetJson(JsonManager::FileType::PLAYER)["AVOID_CAN_ROTATE_FRAME"]);

	/*������ł��邩*/
	if (!CanRolling())return;

	/*����X�^�~�i�͑����̂�*/
	if (!CanAction(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]))return;

	/*A�{�^���������ꂽ && ����̖��G���Ԃ��I�����Ă���*/
	if ((pad & PAD_INPUT_3) && !this->isCount[static_cast<int>(FrameCountType::AVOID)])
	{
		//�U���t���O�ƈړ��t���O������
		this->state->ClearFlag(this->MASK_ATTACK | this->MASK_MOVE | this->MASK_REACTION);
		//����t���O�𗧂Ă�
		this->state->SetFlag(this->AVOID);
		//��]�\�t���O�𗧂Ă�
		this->isCount[static_cast<int>(FrameCountType::AVOID_CAN_ROTATE)] = true;
		//�t���[������t���O�𗧂Ă�
		this->isCount[static_cast<int>(FrameCountType::JUST_AVOID)] = true;
		//���G�t���O�𗧂Ă�
		auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
		auto& data = dynamic_cast<PlayerData&>(*collider.data);
		data.isInvinvible = true;
		//�X�^�~�i�̌v�Z
		CalcStamina(json.GetJson(JsonManager::FileType::PLAYER)["AVOID_STAMINA_CONSUMPTION"]);
		//�X�s�[�h�ɉ���̏���������
		this->speed = json.GetJson(JsonManager::FileType::PLAYER)["ROLLING_SPEED"];
	}

}
/// <summary>
/// �U��
/// </summary>
void Player::Attack()
{
	/*�U�����ɃA�j���[�V�������I�����Ă�����U���t���O������*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->MASK_ATTACK);
	}

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();
	auto& attack = Singleton<PlayerAttackManager>  ::GetInstance();
	auto& effect = Singleton<EffectManager>  ::GetInstance();

	const int COMBO_RESET = static_cast<int>(FrameCountType::COMBO_RESET);
	const int ATTACK_CANCEL = static_cast<int>(FrameCountType::ATTACK_CANCEL);
	const int ATTACK_CAN_ROTATE = static_cast<int>(FrameCountType::ATTACK_CAN_ROTATE);
	const int CHARGE_ATTACK = static_cast<int>(FrameCountType::CHARGE_ATTACK);

	/*�R���{���Z�b�g�t���O�̔���*/
	if (FrameCount(COMBO_RESET, json.GetJson(JsonManager::FileType::PLAYER)["CONBO_RESET_MAX_FRAME"]))
	{
		this->attackComboCount = -1;
	}

	/*�U�����Z�b�g�t���O*/
	if (this->attackComboCount != -1 && FrameCount(ATTACK_CANCEL, json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_CANCEL_MAX_FRAME"][this->attackComboCount]))
	{
		this->isCancelAttack = true;
	}

	/*��]�\�t���O�̔���*/
	if (FrameCount(ATTACK_CAN_ROTATE, json.GetJson(JsonManager::FileType::PLAYER)["ATTACK_CAN_ROTATE_FRAME"]))
	{
	}


	/*pad����*/
	int pad = input.GetPadState();
	bool isPushY = (pad & PAD_INPUT_2);
	bool isPushB = (pad & PAD_INPUT_4);
	this->isStopAnimation = false;

	/*�U�������߂Ă���Œ���������*/
	if (this->isChargeAttack)
	{
		bool isStartAttack = false;
		//�t���[���𑝉�
		this->frameCount[CHARGE_ATTACK]++;
		//�t���[�����萔�𒴂��Ă�����A�j���[�V�������~������
		if (this->frameCount[CHARGE_ATTACK] >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_FRAME"])
		{
			this->isStopAnimation = true;
			ResetFrameCount(ATTACK_CAN_ROTATE);
		}
		//�t���[�����萔�𒴂��Ă�����`���[�W���I��������
		if (this->frameCount[CHARGE_ATTACK] >= json.GetJson(JsonManager::FileType::PLAYER)["CHARGE_ATTACK_STOP_MAX_FRAME"])
		{
			isStartAttack = true;
		}
		//�����U���J�n�t���O�������Ă�����U�����J�n����
		if (isStartAttack)
		{
			//�_���[�W�̌v�Z
			int damageFactor = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE_FACTOR"];
			int baseDamage = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE"];
			int damage = baseDamage * (this->frameCount[CHARGE_ATTACK] / damageFactor);
			//�x�[�X�����������Ȃ�Ȃ��悤�ɂ���
			if (damage < baseDamage)
			{
				damage = baseDamage;
			}
			//�_���[�W�̃Z�b�g
			attack.SetDamage(damage);
			//�U���̊J�n
			attack.OnIsStart();
			//�֌W�ϐ��̏�����
			this->isStopAnimation = false;
			this->isChargeAttack = false;
			ResetFrameCount(CHARGE_ATTACK);
		}
	}

	/*�U���ł��邩*/
	if (!CanAttack())return;

	/*�U���{�^����������Ă����瑁�����^�[��*/
	if (!isPushB && !isPushY) return;

	/*����ƃ��A�N�V�����t���O������*/
	this->state->ClearFlag(this->MASK_AVOID | this->MASK_REACTION | this->MASK_ATTACK);

	/*�U��cancel�t���O������*/
	this->isCancelAttack = false;

	//�R���{���Z�b�g�t���O������
	ResetFrameCount(COMBO_RESET);
	//��]�\�t���[����������
	ResetFrameCount(ATTACK_CAN_ROTATE);
	this->isCount[ATTACK_CAN_ROTATE] = true;

	int attackType;
	/*B�������ꂽ��*/
	if (isPushB)
	{
		//�R���{���𑝂₷
		this->attackComboCount++;
		//�R���{���ő�𒴂�����1�ɖ߂�
		if (this->attackComboCount > static_cast<int>(AttackType::COMBO_3))
		{
			this->attackComboCount = static_cast<int>(AttackType::COMBO_1);
		}
		//�R���{����2�ȉ���������
		if (this->attackComboCount <= static_cast<int>(AttackType::COMBO_2))
		{
			//�U���L�����Z���t���O�𗧂Ă�
			this->isCount[ATTACK_CANCEL] = true;
		}
		//�R���{���Z�b�g�t���O�𗧂Ă�
		this->isCount[COMBO_RESET] = true;
		//�R���{�������ƂɁA�U���̎�ނ�ݒ�
		attackType = this->attackComboCount;
		//�_���[�W�̃Z�b�g
		attack.SetDamage(json.GetJson(JsonManager::FileType::PLAYER)["W_ATTACK_DAMAGE"][this->attackComboCount]);
		//�U���t���O�𗧂Ă�
		attack.OnIsStart();
	}

	/*Y�������ꂽ��*/
	else if (isPushY)
	{
		//�U���̎�ނ�ݒ�
		attackType = static_cast<int>(AttackType::SKILL);
		//�U���L�����Z���t���O������
		ResetFrameCount(ATTACK_CANCEL);
		//�R���{�����Z�b�g����
		this->attackComboCount = -1;
		this->isCount[CHARGE_ATTACK] = true;
		this->isChargeAttack = true;
		effect.OnIsEffect(EffectManager::EffectType::PLAYER_CHARGE);
	}

	/*�U���t���O���Z�b�g*/
	this->state->SetFlag(this->attackMap[attackType]);

}

/// <summary>
/// �t���[���J�E���g
/// </summary>
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

/// <summary>
/// �t���[���J�E���g�̏�����
/// </summary>
void Player::ResetFrameCount(const int _index)
{
	this->isCount[_index] = false;
	this->frameCount[_index] = 0;
}


const bool Player::CanRotation()const
{
	if (this->state->CheckFlag(this->MASK_REACTION) && !this->isCancelReaction)	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH)	)			return false;//�f�X
	if (this->state->CheckFlag(this->MASK_AVOID) && !this->isCount[static_cast<int>(FrameCountType::AVOID_CAN_ROTATE)])		return false;//���
	if (this->state->CheckFlag(this->MASK_ATTACK) && !this->isCount[static_cast<int>(FrameCountType::ATTACK_CAN_ROTATE)])		return false;//�U��
	//if (this->state->CheckFlag(this->BLOCK))			return false;//�h��
	if (this->state->CheckFlag(this->HEAL))				return false;//��
	return true;
}
const bool Player::CanRolling()const
{
	//if (this->state->CheckFlag(this->MASK_ATTACK))		return false;//�U��
	if (this->state->CheckFlag(this->BLOCK))									return false;//�u���b�N
	if (this->state->CheckFlag(this->MASK_REACTION) && !this->isCancelReaction)	return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))									return false;//���S
	if (this->state->CheckFlag(this->MASK_AVOID))								return false;//���
	if (this->state->CheckFlag(this->HEAL))										return false;//��

	return true;
}
const bool Player::CanAttack()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))						return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))								return false;//�f�X
	if (this->state->CheckFlag(this->MASK_ATTACK) && !this->isCancelAttack)	return false;//�U��
	if (this->state->CheckFlag(this->HEAL))									return false;//��
	if (this->state->CheckFlag(this->MASK_AVOID))							return false;//���
	return true;
}
const bool Player::CanBlock()const
{
	if (this->state->CheckFlag(this->MASK_REACTION))						return false;//���A�N�V����
	if (this->state->CheckFlag(this->DEATH))								return false;//�f�X
	if (this->state->CheckFlag(this->MASK_AVOID))							return false;//���
	if (this->state->CheckFlag(this->MASK_ATTACK) && !this->isCancelAttack)	return false;//�U��
	if (this->state->CheckFlag(this->HEAL))									return false;//��
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

/// <summary>
/// �X�^�~�i�̎擾
/// </summary>
const int Player::GetStamina()const
{
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<PlayerData&>(*collider.data);
	return data.stamina;
}

/// <summary>
/// ���b�N�I��
/// </summary>
void Player::LockOn()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>  ::GetInstance();

	/*�E�X�e�B�b�N�������݂���������*/
	if ((input.GetPadState() & PAD_INPUT_10))
	{
		if (!this->isCount[static_cast<int>(FrameCountType::LOCK_ON)])
		{
			if (this->isLockOn)
			{
				this->isLockOn = false;
			}
			else
			{
				this->isLockOn = true;
			}
			this->isCount[static_cast<int>(FrameCountType::LOCK_ON)] = true;
		}
	}
	else
	{
		this->isCount[static_cast<int>(FrameCountType::LOCK_ON)] = false;
	}
}

