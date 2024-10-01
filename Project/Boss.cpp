#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "CharacterData.h"
#include "BossData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"
#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"
#include "BossActionHeader.h"
#include "Boss.h"
#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "BossAttackManager.h"
#include "EffectManager.h"
#include "Debug.h"
#include "HitStop.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Boss::Boss()
	: animationPlayTime(0.0f)
	, moveTarget			{ 0.0f, 0.0f, 0.0f }
	, nowAnimation			(0)
	, nowPhase				(0)
	, prevPhase				(-1)
	, actionType(0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));

	/*�A�j���[�V�����̐ݒ�*/
	vector<string>	animationHandle	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
			  this->nowAnimation	  = static_cast<int>(AnimationType::IDLE);
			  this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�A�j���[�V�����}�b�v�̐ݒ�*/
	this->stateAnimationMap.emplace(this->DYING, static_cast<int>(AnimationType::DYING));
	this->stateAnimationMap.emplace(this->IDLE, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->ROAR, static_cast<int>(AnimationType::ROAR));
	this->stateAnimationMap.emplace(this->WALK, static_cast<int>(AnimationType::WALK));
	this->stateAnimationMap.emplace(this->REST, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->SLASH, static_cast<int>(AnimationType::SLASH));
	this->stateAnimationMap.emplace(this->FLY_ATTACK, static_cast<int>(AnimationType::FLY_ATTACK));
	this->stateAnimationMap.emplace(this->HURRICANE_KICK, static_cast<int>(AnimationType::HURRICANE_KICK));
	this->stateAnimationMap.emplace(this->JUMP_ATTACK, static_cast<int>(AnimationType::JUMP_ATTACK));
	this->stateAnimationMap.emplace(this->ROTATE_PUNCH, static_cast<int>(AnimationType::ROTATE_PUNCH));
	/*�A�N�V�����}�b�v�̍쐬*/
	this->actionTypeMap.emplace(static_cast<int>(ActionType::DYING),this->DYING);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::IDLE),this->IDLE);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROAR),this->ROAR);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::WALK),this->WALK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::REST),this->REST);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH),this->SLASH);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::FLY_ATTACK),this->FLY_ATTACK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::HURRICANE_KICK),this->HURRICANE_KICK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::JUMP_ATTACK),this->JUMP_ATTACK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROTATE_PUNCH),this->ROTATE_PUNCH);

	/*�R���C�_�[�f�[�^�̍쐬*/
	CharacterData* data = new BossData();
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, data);

	/*�A�N�V�����p�����[�^�[�̍쐬*/
	this->parameters.emplace_back(new BossDeathAction());
	this->parameters.emplace_back(new BossIdleAction());
	this->parameters.emplace_back(new BossRoarAction());
	this->parameters.emplace_back(new BossChaseAction());
	this->parameters.emplace_back(new BossRestAction());
	/*this->parameters.emplace_back(new BossSlashAction());
	this->parameters.emplace_back(new BossFlyAction());
	this->parameters.emplace_back(new BossHurricaneKickAction());
	this->parameters.emplace_back(new BossStabAction());
	this->parameters.emplace_back(new BossRotatePunchAction());*/
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boss::~Boss()
{
	this->stateAnimationMap.clear();
}

void Boss::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);

	/*�ϐ��̏�����*/
	this->isAlive				 = false;
	this->isGround				 = true;
	this->isDraw = false;
	this->speed					 = 0.0f;
	this->animationPlayTime		 = 0.0f;
	this->entryInterval			 = 0;
	this->nowPhase				 = 0;
	this->prevPhase				 = -1;
	this->moveTarget			 = Gori::ORIGIN;
	this->nowAnimation			 = static_cast<int>(AnimationType::ROAR);
	this->actionType			 = static_cast<int>(ActionType::ROAR);
	for (int i = 0; i < this->parameters.size(); i++)
	{
		this->parameters[i]->Initialize();
	}
	float height		= json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	collider.topPositon = /*VAdd(collider.rigidbody.GetPosition(),*/ VGet(0.0f, height, 0.0f)/*)*/;
	collider.radius		= json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	data.hp				= json.GetJson(JsonManager::FileType::ENEMY)["HP"];
	data.isHit			= false;

	/*�t�F�[�Y�̐ݒ�*/
	SetPhase();

	/*���������̏�����*/
	//json�f�[�^��萔�ɑ��
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//��]��
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //�g�嗦
	//������
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);
	MV1SetPosition	 (this->modelHandle, this->collider->rigidbody.GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->collider->rigidbody.GetRotation());
	MV1SetScale		 (this->modelHandle, this->collider->rigidbody.GetScale());

	this->state->ClearFlag(this->MASK_ALL);
	this->state->SetFlag(this->ROAR);

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// �㏈��
/// </summary>
void Boss::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
void Boss::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& hitStop = Singleton<HitStop>			::GetInstance();

	if (!this->isAlive)
	{
		if (player.GetIsAlive())
		{
			if (this->entryInterval == json.GetJson(JsonManager::FileType::ENEMY)["ON_ENTRY_EFFECT_INTERVAL"])
			{
				auto& effect = Singleton<EffectManager>::GetInstance();
				effect.OnIsEffect(EffectManager::EffectType::BOSS_ENTRY);
			}
			this->entryInterval++;
			if (this->entryInterval >= json.GetJson(JsonManager::FileType::ENEMY)["ENTRY_INTERVAL"])
			{
				this->entryInterval = 0;
				this->isAlive = true;
				this->isDraw = true;
			}
		}
	}
	else
	{
		/*�t�F�[�Y�̏�����*/
		SetPhase();

		/*��Ԃ̐؂�ւ�*/
		ChangeState();

		if (!hitStop.IsHitStop())
		{

			/*�����ł��ׂẴp�����[�^�̌v�Z���s��*/
			for (auto& item : this->parameters)
			{
				item->CalcParameter(*this);
			}

			/*�����Ɋe�A�N�V�������Ƃ̍X�V��������ꂽ��*/
			this->parameters[this->actionType]->Update(*this);

			/*�A�j���[�V�����̍X�V*/
			unsigned int nowState = this->state->GetFlag();
			this->nowAnimation = this->stateAnimationMap[nowState];

			//�A�j���[�V�����̍Đ�
			VECTOR position = this->collider->rigidbody.GetPosition();
			this->animation->Play(&this->modelHandle, position, this->nowAnimation, this->animationPlayTime);
			this->collider->rigidbody.SetPosition(position);
		}
	}
}

void Boss::ChangeState()
{
	/*�I������Ă��邩���ׂ�*/
	bool isSelect		= false;//�I������Ă��邩
	int  sumDesireValue = 0;	//�~���l�̍��v
	for (auto& item : this->parameters)
	{
		isSelect		= item->GetIsSelect();
		sumDesireValue += item->GetDesireValue();
		if (isSelect)return;
	}
	
	/*�������Ă���t���O������*/
	unsigned int clearFlag = this->actionTypeMap[this->actionType];
	this->state->ClearFlag(clearFlag);

	/*�I������Ă��Ȃ�������*/
	if (!isSelect)
	{
		//�e�s���̊��Ғl�����߂�
		std::vector<int> actionWeight;//�d��
		for (auto& item : this->parameters)
		{
			actionWeight.emplace_back(item->GetWeight(sumDesireValue));
		}
		//�d�݂������_���ŏo��
		int randomWeight = GetRand(this->parameters[0]->GetBaseWeight() - 1);
		//for��vector���񂵁A�d�݂��O�ȉ��ɂȂ����Ƃ���̃A�N�V�������s��
		for (int i = 0; i < actionWeight.size(); i++)
		{
			randomWeight -= actionWeight[i];
			if (randomWeight < 0)
			{
				this->actionType = i;
				this->parameters[i]->OnIsSelect();
				break;
			}
		}
	}

	unsigned int setFlag = this->actionTypeMap[this->actionType];
	this->state->SetFlag(setFlag);
}

/// <summary>
/// �`��
/// </summary>
const void Boss::DrawCharacterInfo()const
{
	auto& debug = Singleton<Debug>::GetInstance();
	if (debug.CheckEnemyFlag())
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		VECTOR rotation = this->collider->rigidbody.GetRotation();
		printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
		printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
		printfDx("%d:DYING					\n", this->state->CheckFlag(this->DYING));
		printfDx("%d:IDLE						\n", this->state->CheckFlag(this->IDLE));
		printfDx("%d:ROAR						\n", this->state->CheckFlag(this->ROAR));
		printfDx("%d:WALK						\n", this->state->CheckFlag(this->WALK));
		printfDx("%d:REST						\n", this->state->CheckFlag(this->REST));
		printfDx("%d:SLASH					\n", this->state->CheckFlag(this->SLASH));
		printfDx("%d:FLY_ATTACK				\n", this->state->CheckFlag(this->FLY_ATTACK));
		printfDx("%d:HURRICANE_KICK			\n", this->state->CheckFlag(this->HURRICANE_KICK));
		printfDx("%d:JUMP_ATTACK				\n", this->state->CheckFlag(this->JUMP_ATTACK));
		printfDx("%d:ROTATE_PUNCH				\n", this->state->CheckFlag(this->ROTATE_PUNCH));
	}
}





const bool Boss::GetIsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}
const VECTOR Boss::GetHeadPosition()const
{
	return MV1GetFramePosition(this->modelHandle, 7);
}

/// <summary>
/// �A�j���[�V�����Đ����Ԃ̎擾
/// </summary>
const float Boss::GetAnimationPlayTime()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// �t�F�[�Y�̐ݒ�
/// </summary>
void Boss::SetPhase()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);
	const float HP = data.hp;
	const float MAX_HP = json.GetJson(JsonManager::FileType::ENEMY)["HP"];

	/*HP���ő�l��1/3������������t�F�[�Y�R*/
	if (HP < (MAX_HP / 3))
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_3);
	}
	/*HP���ő�l��2/3������������t�F�[�Y2*/
	else if (HP < (MAX_HP / 3) * 2)
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_2);
	}
	/*�t�F�[�Y�P*/
	else
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_1);
	}
}


/// <summary>
/// ��]���̐ݒ�
/// </summary>
void Boss::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// �ړ��x�N�g���̐ݒ�
/// </summary>
void Boss::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}