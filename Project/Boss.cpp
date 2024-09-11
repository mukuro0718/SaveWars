#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"

#include "Rigidbody.h"

#include "CharacterData.h"
#include "BossData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"

#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"
#include "Boss.h"

#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "BossAttackManager.h"
#include "EffectManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Boss::Boss()
	: animationPlayTime(0.0f)
	, moveTarget			{ 0.0f, 0.0f, 0.0f }
	, attackComboCount		(0)
	, attackComboIndexOffset(0)
	, nowAnimation			(0)
	, attackType			(0)
	, nowPhase				(0)
	, prevPhase				(-1)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));

	/*vector�̒ǉ�*/
	for (int i = 0; i < this->COUNT_NUM; i++)
	{
		this->frameCount.emplace_back(0);
		this->isCount.emplace_back(false);
	}

	/*�A�j���[�V�����̐ݒ�*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
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

	/*�R���C�_�[�f�[�^�̍쐬*/
	CharacterData* data = new BossData();
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, data);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boss::~Boss()
{
	this->stateAnimationMap.clear();
	this->frameCount.clear();
	this->isCount.clear();
	this->attackCombo.clear();
}

void Boss::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);


	/*�ϐ��̏�����*/
	collider.radius = json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	float height = json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	collider.topPositon = VAdd(collider.rigidbody.GetPosition(), VGet(0.0f, height, 0.0f));
	data.hp			= json.GetJson(JsonManager::FileType::ENEMY)["HP"];
	data.isHit		= false;

	/*�t�F�[�Y�̐ݒ�*/
	SetPhase();
	this->prevPhase = this->nowPhase;

	/*�U���R���{�̏�����*/
	this->attackComboCount = -1;

	/*�X�s�[�h�̏�����*/
	this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	this->speed = 0.0f;

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

	/*��Ԃ�ҋ@��Ԃɏ�����*/
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

	/*�t�F�[�Y�̏�����*/
	SetPhase();

	/*�ړ�����*/
	Move();

	/*��Ԃ̐؂�ւ�*/
	ChangeState();
	if (!this->state->CheckFlag(this->MASK_ALL))
	{
		this->state->SetFlag(this->IDLE);
	}

	/*�A�j���[�V�����̍X�V*/
	this->nowAnimation = this->stateAnimationMap[this->state->GetFlag()];
	//�Đ����Ԃ̐ݒ�
	float animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	SlowAnimationPlayTime(FrameCountType::SLASH, json.GetJson(JsonManager::FileType::ENEMY)["SLASH_SLOW_FRAME_COUNT"], animationPlayTime);
	SlowAnimationPlayTime(FrameCountType::ROTATE_PUNCH, json.GetJson(JsonManager::FileType::ENEMY)["ROTATE_PUNCH_SLOW_FRAME_COUNT"], animationPlayTime);
	SlowAnimationPlayTime(FrameCountType::JUMP_ATTACK, json.GetJson(JsonManager::FileType::ENEMY)["STAB_SLOW_FRAME_COUNT"], animationPlayTime);
	AddAnimationPlayTime(FrameCountType::HURRICANE_KICK, json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_ADD_FRAME_COUNT"], animationPlayTime);
	SlowAnimationPlayTime(FrameCountType::FLY_ATTACK, json.GetJson(JsonManager::FileType::ENEMY)["FLY_ATTACK_SLOW_FRAME_COUNT"], animationPlayTime);
	//�A�j���[�V�����̍Đ�
	VECTOR position = this->collider->rigidbody.GetPosition();
	this->animation->Play(&this->modelHandle, position, this->nowAnimation, this->animationPlayTime);
	this->collider->rigidbody.SetPosition(position);
}

/// <summary>
/// ���K
/// </summary>
void Boss::Roar()
{
	/*���K���ɃA�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->ROAR) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROAR);
		this->prevPhase = this->nowPhase;
	}

	/*���K�ł��邩*/
	CanRoar();

	if (this->nowPhase != this->prevPhase)
	{
		this->state->ClearFlag(this->MASK_ALL);
		this->state->SetFlag(this->ROAR);
	}
}

/// <summary>
/// �ړ�
/// </summary>
void Boss::Move()
{
	/*��]�����o��*/
	UpdateRotation();

	/*�ړ����x�̍X�V*/
	UpdateSpeed();

	/*�ړ��x�N�g�����o��*/
	UpdateMoveVector();
}

/// <summary>
/// �q�b�g���A�N�V����
/// </summary>
void Boss::Reaction()
{

}
/// <summary>
/// �x�e
/// </summary>
void Boss::Rest()
{

}
/// <summary>
/// ���S
/// </summary>
void Boss::Death()
{

}
/// <summary>
/// �U��
/// </summary>
void Boss::Attack()
{
	
}
void Boss::ChangeState()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& effect = Singleton<EffectManager>::GetInstance();
	auto& attack = Singleton<BossAttackManager>::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);

	/*�f�X����*/
	//�f�X���Ă��邩
	if (data.hp < 0)
	{
		this->state->SetFlag(this->DYING);
		if (this->animation->GetIsChangeAnim())
		{
			this->isAlive = false;
		}
	}
	//�f�X���Ă��Ȃ����
	if (this->state->CheckFlag(this->DYING))return;

	/*���K����*/
	Roar();
	//���K���Ă�����
	if (this->state->CheckFlag(this->ROAR))return;

	/*�x�e����*/
	//�x�e���邩
	if (CanRest())
	{
		//�R���{�J�E���g��0��������
		if (this->attackComboCount < 0)
		{
			this->state->ClearFlag(this->MASK_ALL);
			this->state->SetFlag(this->REST);
			this->isCount[static_cast<int>(FrameCountType::REST)] = true;
		}
		else
		{
			this->state->ClearFlag(this->MASK_ALL);
		}
	}
	//�x�e���Ȃ�J�E���g��i�߂�
	if (this->state->CheckFlag(this->REST))
	{
		//�����J�E���g���I�����Ă�����t���O������
		if (FrameCount(static_cast<int>(FrameCountType::REST), json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]))
		{
			this->state->ClearFlag(this->REST);
			this->state->SetFlag(this->IDLE);
		}
	}
	//�x�e������Ȃ���Α������^�[��
	if (this->state->CheckFlag(this->REST) || this->state->CheckFlag(this->MASK_ATTACK))return;

	/*�ړ����邩*/
	const float TARGET_DISTANCE = VSize(VSub(player.GetRigidbody().GetPosition(), this->collider->rigidbody.GetPosition()));//�v���C���[�Ƃ̋��������߂�
	const float MOVE_DISTANCE = json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"];//�ړ������鋗��
	
	this->state->ClearFlag(this->WALK | this->IDLE);

	/*�����ċ߂Â�3*/
	if (TARGET_DISTANCE >= MOVE_DISTANCE)
	{
		this->state->SetFlag(this->WALK);
	}
	/*�U��*/
	else
	{
		SetAttackCombo();
	}
}

/// <summary>
/// �`��
/// </summary>
const void Boss::DrawCharacterInfo()const
{
	//VECTOR position = this->rigidbody.GetPosition();
	//VECTOR rotation = this->rigidbody.GetRotation();
	//printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	//printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	//printfDx("%d:DYING					\n", this->state->CheckFlag(this->DYING));
	//printfDx("%d:IDLE					\n", this->state->CheckFlag(this->IDLE));
	//printfDx("%d:ROAR					\n", this->state->CheckFlag(this->ROAR));
	//printfDx("%d:WALK					\n", this->state->CheckFlag(this->WALK));
	//printfDx("%d:REST					\n", this->state->CheckFlag(this->REST));
	//printfDx("%d:SLASH					\n", this->state->CheckFlag(this->SLASH));
	//printfDx("%d:FLY_ATTACK				\n", this->state->CheckFlag(this->FLY_ATTACK));
	//printfDx("%d:HURRICANE_KICK			\n", this->state->CheckFlag(this->HURRICANE_KICK));
	//printfDx("%d:JUMP_ATTACK			\n", this->state->CheckFlag(this->JUMP_ATTACK));
	//printfDx("%d:ROTATE_PUNCH			\n", this->state->CheckFlag(this->ROTATE_PUNCH));
	//printfDx("%d:BOSS_HIT_NUM			\n", this->GetHitNumber());
}

/// <summary>
/// ���x�̍X�V
/// </summary>
void Boss::UpdateMoveVector()
{
	/*�ړ��x�N�g��������������*/
	VECTOR direction = { 0.0f,0.0f,0.0f };
	VECTOR rotation = this->collider->rigidbody.GetRotation();

	/*��]�������ƂɈړ��x�N�g�����o��*/
	direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*�ړ��x�N�g���𐳋K��*/
	direction = VNorm(direction);

	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = this->collider->rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);

	this->collider->rigidbody.SetVelocity(newVelocity);
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Boss::UpdateSpeed()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->state->CheckFlag(this->WALK))
	{
		this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	}
	else if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		AttackSpeed(this->attackType, json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_SPEED"][this->attackType]);
	}
	else
	{
		this->speed = 0.0f;
	}
}

/// <summary>
/// ��]���̍X�V
/// </summary>
void Boss::UpdateRotation()
{
	/*��]�ł��Ȃ���Α������^�[��*/
	if (!CanRotation())return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR position	 = this->collider->rigidbody.GetPosition();//���W
		  VECTOR nowRotation = this->collider->rigidbody.GetRotation();//��]��
		  VECTOR nextMoveTarget = player.GetRigidbody().GetPosition();
		  

	if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		if (this->isCount[this->attackType])
		{
			this->moveTarget = nextMoveTarget;
		}
		else
		{
			if (this->state->CheckFlag(this->HURRICANE_KICK))
			{
				this->moveTarget = Lerp(this->moveTarget,nextMoveTarget,VGet(0.01f,0.01f,0.01f));
			}
		}
	}
	else
	{
		this->moveTarget = nextMoveTarget;
	}
	
	/*�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��*/
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);

	/*�A�[�N�^���W�F���g���g���Ċp�x�����߂�*/
	nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));

	/*��]������*/
	this->collider->rigidbody.SetRotation(nowRotation);
}




/// <summary>
/// ��]�ł��邩
/// </summary>
/// <returns></returns>
const bool Boss::CanRotation()const
{
	//if (this->state->CheckFlag(this->MASK_ATTACK)/* || this->state->CheckFlag(this->REST)*/)return false;
	return true;
}

const bool Boss::CanMove()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return false;
	if (this->state->CheckFlag(this->ROAR))return false;
	return true;
}
/// <summary>
/// �U���ł��邩
/// </summary>
const bool Boss::CanAttack()const
{
	//if (this->state->CheckFlag(this->PUNCH_ATTACK))		 return false;
	//if (this->state->CheckFlag(this->KNOCKGROUND_ATTACK))return false;
	//if (this->state->CheckFlag(this->RUSH_ATTACK))		 return false;
	//if (this->state->CheckFlag(this->BREATH_ATTACK))	 return false;
	return true;
}
/// <summary>
/// �x�e�ł��邩
/// </summary>
const bool Boss::CanRest()const
{
	/*�U�������A�j���[�V�������I�����Ă���*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		return true;
	}
	return false;
}
/// <summary>
/// ���K�ł��邩
/// </summary>
const bool Boss::CanRoar()const
{
	/*�U�������A�j���[�V�������I�����Ă���*/
	if (this->state->CheckFlag(this->REST)) return true;
	return false;
}
/// <summary>
/// �w�肵���t���[���̌v��
/// </summary>
bool Boss::FrameCount(const int _index, const int _maxFrame)
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
/// �G�t�F�N�g�t���O�𗧂Ă�
/// </summary>
void Boss::OnEffectFlag(const int _attack)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& effect = Singleton<EffectManager>::GetInstance();

	switch (_attack)
	{
	case static_cast<int>(AttackType::NONE):
		break;
	case static_cast<int>(AttackType::SLASH):
		effect.OnIsBossSlashEffect();
		break;
	case static_cast<int>(AttackType::FLY_ATTACK):
		break;
	case static_cast<int>(AttackType::HURRICANE_KICK):
		break;
	case static_cast<int>(AttackType::JUMP_ATTACK):
		effect.OnIsBossJumpAttackEffect();
		break;
	case static_cast<int>(AttackType::ROTATE_PUNCH):
		effect.OnIsBossRotatePunchEffect();
		break;
	default:
		break;
	}
}
/// <summary>
/// �U���t���O�𗧂Ă�
/// </summary>
void Boss::SetAttackFlag(const int _attack)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& effect = Singleton<EffectManager>::GetInstance();

	switch (_attack)
	{
	case static_cast<int>(AttackType::NONE):
		break;
	case static_cast<int>(AttackType::SLASH):
		this->state->SetFlag(this->SLASH);
		break;
	case static_cast<int>(AttackType::FLY_ATTACK):
		this->state->SetFlag(this->FLY_ATTACK);
		break;
	case static_cast<int>(AttackType::HURRICANE_KICK):
		this->state->SetFlag(this->HURRICANE_KICK);
		break;
	case static_cast<int>(AttackType::JUMP_ATTACK):
		this->state->SetFlag(this->JUMP_ATTACK);
		break;
	case static_cast<int>(AttackType::ROTATE_PUNCH):
		this->state->SetFlag(this->ROTATE_PUNCH);
		break;
	default:
		break;
	}
}
/// <summary>
/// �A�j���[�V�����̍Đ����Ԃ�x������
/// </summary>
void Boss::SlowAnimationPlayTime(const FrameCountType _type,const int _targetCount, const float _maxTime)
{
	int type = static_cast<int>(_type);
	if (this->attackType == type)
	{
		float animationPlayTime = _maxTime;
		FrameCount(type, _targetCount);
		if (this->isCount[type])
		{
			animationPlayTime *= 0.1f;
			this->animationPlayTime = animationPlayTime;
		}
		else
		{
			this->animationPlayTime = _maxTime;
		}
	}
}
/// <summary>
/// �A�j���[�V�����̍Đ����Ԃ�ǉ�
/// </summary>
void Boss::AddAnimationPlayTime(const FrameCountType _type, const int _targetCount, const float _maxTime)
{
	int type = static_cast<int>(_type);
	if (this->attackType == type)
	{
		if (this->isCount[type])
		{
			if (this->frameCount[type] == 0)
			{
				this->animationPlayTime = 0.0f;
			}
		}
		FrameCount(type, _targetCount);
		this->animationPlayTime += _maxTime / static_cast<float>(_targetCount);
		if (this->animationPlayTime >= _maxTime)
		{
			this->animationPlayTime = _maxTime;
		}
	}
}
/// <summary>
/// �U�����̃X�s�[�h�̐ݒ�
/// </summary>
void Boss::AttackSpeed(const int _type,const float _speed)
{
	if (this->attackType == _type)
	{
		if (this->isCount[_type])
		{
			this->speed = 0.0f;
		}
		else
		{
			this->speed = _speed;
			VECTOR position = this->collider->rigidbody.GetPosition();
			VECTOR distance = VSub(this->moveTarget, position);
			float distanceSize = VSize(distance);
			if (distanceSize < 5.0f)
			{
				this->speed = 0.0f;
			}
		}
	}
}

void Boss::SetAttackComboCount()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->attackComboCount = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO_COUNT"][this->nowPhase];
	this->attackComboIndexOffset = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO_INDEX_OFFSET"][this->nowPhase];
}

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

void Boss::SetAttackCombo()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& attack = Singleton<BossAttackManager>::GetInstance();

	/*�U�����������瑁�����^�[��*/
	if (this->state->CheckFlag(this->MASK_ATTACK))return;

	//�R���{�J�E���g��0��������
	if (this->attackComboCount < 0)
	{
		SetAttackComboCount();
		int type = GetRand(this->attackComboCount) + this->attackComboIndexOffset;
		vector<int> combo = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO"][type];
		this->attackCombo = combo;
	}

	this->attackType = this->attackCombo[this->attackComboCount];
	SetAttackFlag(this->attackType);
	attack.OnIsStart(this->attackType);
	OnEffectFlag(this->attackType);
	this->isCount[this->attackType] = true;
	this->attackComboCount--;
}