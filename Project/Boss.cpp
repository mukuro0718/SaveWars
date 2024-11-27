#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
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
#include "EffectManager.h"
#include "Debug.h"
#include "HitStopManager.h"
#include "Shadow.h"
#include "MapManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Boss::Boss()
	: prevAttack(AttackType::NONE)
	, moveTarget			(Gori::ORIGIN)
	, animationPlayTime		(0.0f)
	, angryValue			(0.0f)
	, nowAnimation			(0)
	, nowAction				(0)
	, angryState			(0)
	, tiredInterval			(0)
	, attackComboCount		(0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));

	/*�A�j���[�V�����̐ݒ�*/
	vector<int>	animationHandle	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int>		animationIndex	  = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
			  this->nowAnimation	  = static_cast<int>(AnimationType::IDLE);
			  this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//�A�j���[�V�����̒ǉ�
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(animationHandle[i], animationIndex[i]);
	}
	//�A�j���[�V�����̃A�^�b�`
	this->animation->Attach(&this->modelHandle);

	/*�A�N�V�����}�b�v�̍쐬*/
	this->actionTypeMap.emplace(static_cast<int>(ActionType::DYING)			,this->DYING);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::IDLE)			,this->IDLE);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROAR)			,this->ROAR);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::WALK)			,this->WALK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::REST)			,this->REST);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_1)		,this->SLASH_1);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_2)		,this->SLASH_2);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::STAB)			,this->STAB);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROTATE_SLASH)	,this->ROTATE_SLASH);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::PUNCH)			,this->PUNCH);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_COMBO_1)	,this->SLASH_COMBO_1);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH_COMBO_2),this->SLASH_COMBO_2);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::JUMP_ATTACK)	,this->JUMP_ATTACK);

	/*�R���C�_�[�f�[�^�̍쐬*/
	CharacterData* data = new BossData();
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, data);

	/*�A�N�V�����p�����[�^�[�̍쐬*/
	this->parameters.emplace_back(new BossDeathAction());
	this->parameters.emplace_back(new BossIdleAction());
	this->parameters.emplace_back(new BossRoarAction());
	this->parameters.emplace_back(new BossChaseAction());
	this->parameters.emplace_back(new BossRestAction());
	this->parameters.emplace_back(new BossSlashAction());
	this->parameters.emplace_back(new BossSlash2Action());
	this->parameters.emplace_back(new BossStabAction());
	this->parameters.emplace_back(new BossRotateSlashAction());
	this->parameters.emplace_back(new BossPunchAction());
	this->parameters.emplace_back(new BossSlashComboAction());
	this->parameters.emplace_back(new BossSlashCombo2Action());
	this->parameters.emplace_back(new BossJumpAttackAction());
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boss::~Boss()
{
	Finalize();
}

void Boss::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);

	/*�ϐ��̏�����*/
	this->isAlive			= true;
	this->isGround			= true;
	this->isDraw			= true;
	this->speed				= 0.0f;
	this->animationPlayTime	= 0.0f;
	this->entryInterval		= 0;
	this->moveTarget		= Gori::ORIGIN;
	this->nowAnimation		= static_cast<int>(AnimationType::ROAR);
	this->nowAction			= static_cast<int>(ActionType::ROAR);
	this->attackComboCount	= 0;
	this->angryState		= static_cast<int>(AngryStateType::NORMAL);
	float height			= json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	collider.topPositon		= VGet(0.0f, height, 0.0f);
	collider.radius			= json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	collider.isUseCollWithGround = true;
	data.hp					= json.GetJson(JsonManager::FileType::ENEMY)["HP"];
	data.isHit				= false;
	
	/*�R���{�̐ݒ�*/
	SetAttackComboCount();

	/*�p�����[�^�̏�����*/
	for (int i = 0; i < this->parameters.size(); i++)
	{
		this->parameters[i]->Initialize();
	}

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
	const COLOR_F BASE = { 1.0f,1.0f,1.0f,1.0f };
	MV1SetDifColorScale(this->modelHandle, BASE);
	MV1SetSpcColorScale(this->modelHandle, BASE);
	MV1SetEmiColorScale(this->modelHandle, BASE);
	MV1SetAmbColorScale(this->modelHandle, BASE);
}

/// <summary>
/// �㏈��
/// </summary>
void Boss::Finalize()
{
	this->actionTypeMap.clear();
	for (int i = 0; i < this->parameters.size(); i++)
	{
		DeleteMemberInstance(this->parameters[i]);
	}
	this->parameters.clear();
}

/// <summary>
/// �X�V
/// </summary>
void Boss::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�{���Ԃ̐ݒ�*/
	SetAngryState();

	/*��Ԃ̐؂�ւ�*/
	ChangeState();

	/*�����Ɋe�A�N�V�������Ƃ̍X�V��������ꂽ��*/
	this->parameters[this->nowAction]->Update(*this);
}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Boss::PlayAnimation()
{
	//�A�j���[�V�����̍Đ�
	if (this->isAlive)
	{
		VECTOR position = this->collider->rigidbody.GetPosition();
		this->animation->Play(&this->modelHandle, this->nowAnimation, this->animationPlayTime);
		this->collider->rigidbody.SetPosition(position);
	}
}

/// <summary>
/// �U���R���{�񐔂̃Z�b�g
/// </summary>
void Boss::SetAttackComboCount()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�R���{���̐ݒ�*/
	this->attackComboCount = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO_COUNT"][this->angryState];
}

/// <summary>
/// ��Ԃ̕ύX
/// </summary>
void Boss::ChangeState()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& debug = Singleton<Debug>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	int debugActionType = json.GetJson(JsonManager::FileType::DEBUG)["BOSS_ACTION"];
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY) && debugActionType != static_cast<int>(Boss::ActionType::NONE))
	{
		/*�������Ă���t���O������*/
		unsigned int clearFlag = this->actionTypeMap[this->nowAction];
		this->state->ClearFlag(clearFlag);
		this->nowAction = debugActionType;
		this->parameters[this->nowAction]->OnIsSelect();
	}
	else
	{
		/*�I������Ă��邩���ׂ�*/
		bool isSelect = false;//�I������Ă��邩
		int  sumDesireValue = 0;	//�~���l�̍��v
		for (int i = 0; i < this->parameters.size(); i++)
		{
			isSelect = this->parameters[i]->GetIsSelect();
			sumDesireValue += parameters[i]->GetDesireValue();
			if (isSelect)return;
		}
		
		/*�����ł��ׂẴp�����[�^�̌v�Z���s��*/
		bool isPriority = false;//�I������Ă��邩
		for (int i = 0; i < this->parameters.size(); i++)
		{
			this->parameters[i]->CalcParameter(*this);
			//�D��t���O�������Ă��邩�����ׂ�
			if (this->parameters[i]->GetIsPriority())
			{
				isPriority = true;
			}
		}

		/*�������Ă���t���O������*/
		unsigned int clearFlag = this->actionTypeMap[this->nowAction];
		this->state->ClearFlag(clearFlag);

		/*�I������Ă��Ȃ�������*/
		if (!isSelect)
		{
			//�e�s���̊��Ғl�����߂�
			std::vector<int> actionWeight;//�d��
			int count = 0;
			for (int i = 0; i < this->parameters.size(); i++)
			{
				actionWeight.emplace_back(this->parameters[i]->GetWeight(sumDesireValue));
				//�������X�g�̒��őI���t���O����ł������Ă�����
				if (isPriority)
				{
					//���݂�item���I���t���O�������Ă��Ȃ�������d�݂�0�ɂ���
					if (!this->parameters[i]->GetIsPriority())
					{
						actionWeight[count] = 0;
					}
				}
				count++;
			}
			//�d�݂������_���ŏo��
			int randomWeight = GetRand(this->parameters[0]->GetBaseWeight() - 1);
			//for��vector���񂵁A�d�݂��O�ȉ��ɂȂ����Ƃ���̃A�N�V�������s��
			for (int i = 0; i < actionWeight.size(); i++)
			{
				randomWeight -= actionWeight[i];
				if (randomWeight < 0 || this->parameters[i]->GetIsPriority())
				{
					this->nowAction = i;
					this->parameters[i]->OnIsSelect();
					isSelect = true;
					break;
				}
			}
		}
		if (!isSelect)
		{
			this->nowAction = static_cast<int>(ActionType::IDLE);
		}
	}
	unsigned int setFlag = this->actionTypeMap[this->nowAction];
	this->state->SetFlag(setFlag);
}

/// <summary>
/// �`��
/// </summary>
const void Boss::DrawCharacterInfo()const
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& shadow = Singleton<Shadow>::GetInstance();
	auto& map = Singleton<MapManager>::GetInstance();
	auto& debug = Singleton<Debug>::GetInstance();
	
	if (debug.IsShowDebugInfo(Debug::ItemType::ENEMY))
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
		printfDx("%d:SLASH_1					\n", this->state->CheckFlag(this->SLASH_1));
		printfDx("%d:SLASH_2					\n", this->state->CheckFlag(this->SLASH_2));
		printfDx("%d:STAB						\n", this->state->CheckFlag(this->STAB));
		printfDx("%d:ROTATE_SLASH				\n", this->state->CheckFlag(this->ROTATE_SLASH));
		printfDx("%d:PUNCH					\n", this->state->CheckFlag(this->PUNCH));
		printfDx("%d:SLASH_COMBO_1			\n", this->state->CheckFlag(this->SLASH_COMBO_1));
		printfDx("%d:SLASH_COMBO_2			\n", this->state->CheckFlag(this->SLASH_COMBO_2));
		printfDx("%d:JUMP_ATTACK				\n", this->state->CheckFlag(this->JUMP_ATTACK));
		printfDx("%d:STATE					\n", this->angryState);
		/*�e�A�N�V�����̓����蔻��}�`�̕`��*/
		this->parameters[this->nowAction]->Draw();
	}

	if (this->isDraw)
	{
		/*�����̕`��*/
		shadow.Draw(map.GetStageModelHandle(), this->collider->rigidbody.GetPosition(), this->SHADOW_HEIGHT, this->SHADOW_SIZE);
	}
}

const bool Boss::GetIsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
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
/// �{���Ԃ̐ݒ�
/// </summary>
void Boss::SetAngryState()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	
	/*�{����*/
	switch (this->angryState)
	{
		//�{��
	case static_cast<int>(AngryStateType::ANGRY):
		this->angryValue--;
		if (this->angryValue < 0)
		{
			this->angryState = static_cast<int>(AngryStateType::TIRED);
			this->angryValue = 0;
		}
		break;
		//�ʏ�
	case static_cast<int>(AngryStateType::NORMAL):
		//�{��l�𑝉�
		this->angryValue++;
		//�U�����������Ă�����{��l������ɑ���
		if (collider.data->isHit)
		{
			this->angryValue++;
		}
		//�{��l���ő�ȏゾ�������Ԃ�ANGRY�ɂ���
		if (this->angryValue >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_ANGRY_VALUE"])
		{
			this->angryState = static_cast<int>(AngryStateType::ANGRY);
		}
		break;
	//���
	case static_cast<int>(AngryStateType::TIRED):
		//��ꎞ�Ԃ𑝉�
		this->tiredInterval++;
		//�ő�l�𒴂������Ԃ�ʏ�ɕύX
		if (this->tiredInterval >= json.GetJson(JsonManager::FileType::ENEMY)["MAX_TIRED_INTERVAL"])
		{
			this->angryState = static_cast<int>(AngryStateType::NORMAL);
			this->tiredInterval = 0;
		}
		break;
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