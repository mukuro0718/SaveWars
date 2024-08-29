#include <DxLib.h>
#include "GoriLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Animation.h"
#include "Model.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Boss.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "Vector4.h"
#include "Collider.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Boss::Boss()
	: model		(nullptr)
	, state		(nullptr)
	, moveVector{ 0.0f, 0.0f, 0.0f }
	, moveTarget{ 0.0f, 0.0f, 0.0f }
	, velocity	(0.0f)
	, isHitAttack(false)
	, isDraw(true)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->model		= new Model(asset.GetModel(LoadingAsset::ModelType::ENEMY));
	this->state		= new BitFlag();
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
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);

	/*�֐��}�b�v�̐ݒ�*/
	auto idleSet	  = [this]() {Idle(); };
	auto roarSet	  = [this]() {Taunt(); };
	auto moveSet	  = [this]() {Move(); };
	auto deathSet	  = [this]() {Death(); };
	auto restSet	  = [this]() {Rest(); };
	this->AddItemFunction(this->IDLE					, idleSet);
	this->AddItemFunction(this->ROAR, roarSet);
	this->AddItemFunction(this->WALK, moveSet);
	this->AddItemFunction(this->REST					, restSet);
	this->AddItemFunction(this->DYING, deathSet);
	/*�A�j���[�V�����}�b�v�̐ݒ�*/
	this->stateAnimationMap.emplace(this->DYING, static_cast<int>(AnimationType::DYING));
	this->stateAnimationMap.emplace(this->IDLE, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->ROAR, static_cast<int>(AnimationType::ROAR));
	this->stateAnimationMap.emplace(this->WALK, static_cast<int>(AnimationType::WALK));
	this->stateAnimationMap.emplace(this->REST, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->PUNCH, static_cast<int>(AnimationType::PUNCH));
	this->stateAnimationMap.emplace(this->SLASH, static_cast<int>(AnimationType::SLASH));
	this->stateAnimationMap.emplace(this->THROW_STORN, static_cast<int>(AnimationType::THROW_STORN));
	this->stateAnimationMap.emplace(this->HURRICANE_KICK, static_cast<int>(AnimationType::HURRICANE_KICK));
	this->stateAnimationMap.emplace(this->GRAND_SLAM, static_cast<int>(AnimationType::GRAND_SLAM));
	this->stateAnimationMap.emplace(this->FLAME_MAGIC, static_cast<int>(AnimationType::FLAME_MAGIC));
	this->stateAnimationMap.emplace(this->LIGHTNING, static_cast<int>(AnimationType::LIGHTNING));
	this->stateAnimationMap.emplace(this->CONTINUOUS_SLASH, static_cast<int>(AnimationType::CONTINUOUS_SLASH));
	this->stateAnimationMap.emplace(this->DARK_FIELD, static_cast<int>(AnimationType::DARK_FIELD));
	this->stateAnimationMap.emplace(this->METEO, static_cast<int>(AnimationType::METEO));

	this->state->SetFlag(this->IDLE);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boss::~Boss()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	for (int i = 0; i < this->COLLIDER_NUM; i++)
	{
		DeleteMemberInstance(this->collider[i]);
	}
	this->stateFunctionMap.clear();
	this->stateAnimationMap.clear();
	this->frameCount.clear();
	this->isCount.clear();
}

void Boss::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//��]��
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //�g�嗦

	this->velocity = 0.0f;
	this->hp = 1000;
	this->damage = 5;
	VECTOR toPlayer = VNorm(VSub(VGet(0.0f, 0.0f, 0.0f), this->model->GetPosition()));

	/*���f���̃g�����X�t�H�[���̐ݒ�*/
	this->model->SetTransform(POSITION, ROTATION, SCALE);
}

/// <summary>
/// �X�V
/// </summary>
void Boss::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	VECTOR position = this->model->GetPosition();
	position = VAdd(position, this->moveVector);
	this->model->SetPosition(position);

	/*�A�j���[�V�����̍X�V*/

	this->nowAnimation = this->stateAnimationMap[this->state->GetFlag()];
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->PlayAnimation(this->nowAnimation, this->animationPlayTime);
	/*�R���C�_�[�̐ݒ�*/
	const float HEIGHT = json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	const float RADIUS = json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->SetCapsule(position, HEIGHT, RADIUS);
}

/// <summary>
/// �A�N�V����
/// </summary>
void Boss::Action()
{
	/*�ړ��x�N�g���̏�����*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*��]�����o��*/
	UpdateRotation();

	/*�ړ����x�̍X�V*/
	UpdateVelocity();

	/*�ړ��x�N�g�����o��*/
	UpdateMoveVector();

	/*��Ԃ̐؂�ւ�*/
	ChangeState();

	/*��Ԃ��Ƃ̏��������s*/
	//unsigned int flag = this->state->GetFlag();
	//this->stateFunctionMap[flag].update();
}

/// <summary>
/// ���K
/// </summary>
void Boss::Taunt()
{
	/*���K���ɃA�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->ROAR) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROAR);
	}
}
/// <summary>
/// �ړ�
/// </summary>
void Boss::Move()
{
	
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
	//	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	//	auto& json = Singleton<JsonManager>::GetInstance();
	//auto& player = Singleton<PlayerManager>::GetInstance();

	///*�ҋ@��Ԃ�������V�����U����ݒ肷��*/
	//if (this->state->CheckFlag(this->IDLE))
	//{
	//	/*�U���̎�ނ������_���ŏo��*/
	//	//int attackType = GetRand(static_cast<int>(INHIBITION));
	//	this->attackType = 1;
	//	this->state->ClearFlag(this->IDLE);
	//	this->state->SetFlag(this->attackTypeMap[this->attackType]);
	//	this->waitingCountBeforeAttack = 0;
	//	this->attackCount = 0;
	//	this->moveTarget = player.GetPosition();
	//}
	///*�ݒ肳�ꂽ�U�����Ƃ̊֐������s*/
	////this->attackFunctionMap[this->attackType];
	//switch (this->attackType)
	//{
	//case static_cast<int>(AttackType::RUSH):
	//	RushAttack();
	//	break;
	//case static_cast<int>(AttackType::JUMP):
	//	JumpAttack();
	//	break;
	//case static_cast<int>(AttackType::LASER):
	//	break;
	//case static_cast<int>(AttackType::SPIN):
	//	break;
	//case static_cast<int>(AttackType::INHIBITION):
	//	break;
	//}
}
void Boss::ChangeState()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	if (this->hp < 0)
	{
		this->state->SetFlag(this->DYING);
		if (this->model->GetIsChangeAnim())
		{
			this->isDraw = false;
		}
	}

	/*�f�X���Ă��Ȃ����*/
	if (this->state->CheckFlag(this->DYING))return;

	/*�x�e���邩*/
	if (CanRest())
	{
		this->state->ClearFlag(this->MASK_ALL);
		this->state->SetFlag(this->REST);
		this->isCount[static_cast<int>(FrameCountType::REST)] = true;
	}

	/*�x�e���Ȃ�J�E���g��i�߂�*/
	if (this->state->CheckFlag(this->REST))
	{
		/*�����J�E���g���I�����Ă�����t���O������*/
		if (FrameCount(static_cast<int>(FrameCountType::REST), json.GetJson(JsonManager::FileType::ENEMY)["REST_FRAME_COUNT"]))
		{
			this->state->ClearFlag(this->REST);
			this->state->SetFlag(this->IDLE);
		}
	}

	/*�x�e������Ȃ���Α������^�[��*/
	if (this->state->CheckFlag(this->REST) || this->state->CheckFlag(this->MASK_ATTACK))return;

	/*�ړ����邩*/
	const float TARGET_DISTANCE = VSize(VSub(player.GetPosition(), this->model->GetPosition()));//�v���C���[�Ƃ̋��������߂�
	const float MOVE_DISTANCE = json.GetJson(JsonManager::FileType::ENEMY)["MOVE_DISTANCE"];//�ڕW�Ƃ̍ő勗��
	const float THROW_DISTANCE = json.GetJson(JsonManager::FileType::ENEMY)["THROW_DISTANCE"];//�ڕW�Ƃ̍ő勗��
	
	this->state->ClearFlag(this->WALK | this->IDLE);

	/*�΂𓊂���*/
	if (TARGET_DISTANCE >= THROW_DISTANCE)
	{
		int attackType = static_cast<int>(AttackType::THROW_STORN);
		this->state->SetFlag(this->THROW_STORN);
	}
	/*�����v���C���[�Ƃ̋������ő勗���ȏ㗣��Ă�����ǐՂ���*/
	else if (TARGET_DISTANCE >= MOVE_DISTANCE)
	{
		this->state->SetFlag(this->WALK);
	}
	/*�U�����邩*/
	else
	{
		/*�U���R���C�_�[�p�ϐ�*/
		float radius = 0.0f;
		float offsetScale = 0.0f;
		float offsetY = 0.0f;
		VECTOR position = { 0.0f,0.0f,0.0f };
		int attackType = static_cast<int>(AttackType::NONE);

		/*phase1*/
		//�������Ă�������ƃv���C���[�ւ̙��K�̂Ȃ��Ȃ����ȏゾ�������]�؂������
		VECTOR toPlayer = VNorm(VSub(player.GetPosition(), this->model->GetPosition()));
		int type = GetRand(1);
		if (type == 0)
		{
			this->state->SetFlag(this->PUNCH);
			attackType = static_cast<int>(AttackType::PUNCH);
		}
		else
		{
			this->state->SetFlag(this->SLASH);
			attackType = static_cast<int>(AttackType::SLASH);
		}


		/*phase2*/


		/*�R���C�_�[�̍X�V*/
		if (attackType != static_cast<int>(AttackType::NONE))
		{
			radius = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][attackType];
			offsetScale = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_SCALE"][attackType];
			offsetY = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][attackType];
			position = this->model->GetPosition() + VScale(this->direction, offsetScale);
			position.y += offsetY;
			this->damage = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][attackType];
			this->attackNumber++;
		}
		this->collider[static_cast<int>(ColliderType::ATTACK)]->SetSphere(position, radius);
	}
}

/// <summary>
/// �`��
/// </summary>
const void Boss::Draw()const
{
	//VECTOR position = this->model->GetPosition();
	//VECTOR rotation = this->model->GetRotation();
	//printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	//printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	//printfDx("%f:Boss_DOT\n", this->dot);
	//printfDx("%d:IDLE					\n", this->state->CheckFlag(this->IDLE));
	//printfDx("%d:TAUNT					\n", this->state->CheckFlag(this->TAUNT));
	//printfDx("%d:WALK_FRONT				\n", this->state->CheckFlag(this->WALK_FRONT));
	//printfDx("%d:WALK_LEFT				\n", this->state->CheckFlag(this->WALK_LEFT));
	//printfDx("%d:WALK_RIGHT				\n", this->state->CheckFlag(this->WALK_RIGHT));
	//printfDx("%d:VERTICAL_SLASH			\n", this->state->CheckFlag(this->VERTICAL_SLASH));
	//printfDx("%d:HORIZONTAL_SLASH		\n", this->state->CheckFlag(this->HORIZONTAL_SLASH));
	//printfDx("%d:ROTATION_SLASH			\n", this->state->CheckFlag(this->ROTATION_SLASH));
	//printfDx("%d:KNOCK_UP_SLASH			\n", this->state->CheckFlag(this->KNOCK_UP_SLASH));
	//printfDx("%d:STRONG_HORIZONTAL_SLASH\n", this->state->CheckFlag(this->STRONG_HORIZONTAL_SLASH));
	//printfDx("%d:TWO_COMBO				\n", this->state->CheckFlag(this->TWO_COMBO));
	//printfDx("%d:STRONG_TWO_COMBO		\n", this->state->CheckFlag(this->STRONG_TWO_COMBO));
	//printfDx("%d:THREE_COMBO			\n", this->state->CheckFlag(this->THREE_COMBO));
	//printfDx("%d:REACTION				\n", this->state->CheckFlag(this->REACTION));
	//printfDx("%d:DEATH					\n", this->state->CheckFlag(this->DEATH));
	//printfDx("%d:REST					\n", this->state->CheckFlag(this->REST));

	if (this->isDraw)
	{
		this->model->Draw();
	}
	this->collider[static_cast<int>(ColliderType::CHARACTER)]->DrawHitCapsule();
	this->collider[static_cast<int>(ColliderType::ATTACK)]->DrawHitSphere();
}

/// <summary>
/// ���x�̍X�V
/// </summary>
void Boss::UpdateMoveVector()
{
	VECTOR rotation = this->model->GetRotation();

	/*��]�������ƂɈړ��x�N�g�����o��*/
	this->moveVector = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*�ړ��x�N�g���𐳋K��*/
	this->moveVector = VNorm(this->moveVector);

	/*�ړ��x�N�g���̕�������*/
	this->direction = this->moveVector;

	/*�ړ��x�N�g���ɑ��x�����Z*/
	this->moveVector = VScale(this->moveVector, this->velocity);

}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Boss::UpdateVelocity()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->state->CheckFlag(this->WALK))
	{
		this->velocity = json.GetJson(JsonManager::FileType::ENEMY)["VELOCITY"];
	}
	else
	{
		this->velocity = 0.0f;
	}
}
/// <summary>
/// ���W��getter
/// </summary>
/// <returns></returns>
const VECTOR Boss::GetPosition()const
{
	return this->model->GetPosition();
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
	const VECTOR position  = this->model->GetPosition();//���W
		  VECTOR rotation  = { 0.0f,0.0f,0.0f };		 //��]��
		  this->moveTarget = player.GetPosition();
	
	/*�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��*/
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);

	/*�A�[�N�^���W�F���g���g���Ċp�x�����߂�*/
	rotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
	rotation.y = rotation.y * 180.0f / DX_PI_F;

	/*��]������*/
	this->model->SetRotation(rotation);
}



/// <summary>
/// �U�������߂�
/// </summary>
void Boss::DecideOfAttack() 
{
	/*�X�^�~�i���O�ȉ��������炠�����烊�^�[����Ԃ�*/
	if (this->stamina <= 0)return;

	/*�U���������_���Ō��߂�*/

}


const Collider Boss::GetCharacterCollider()
{
	return *this->collider[static_cast<int>(ColliderType::CHARACTER)];
}
const Collider Boss::GetAttackCollider()
{
	return *this->collider[static_cast<int>(ColliderType::ATTACK)];
}

/// <summary>
/// �ːi�U��
/// </summary>
void Boss::RushAttack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	
}
/// <summary>
/// �W�����v�U��
/// </summary>
void Boss::JumpAttack()
{
	
}

/// <summary>
/// ��]�ł��邩
/// </summary>
/// <returns></returns>
const bool Boss::CanRotation()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK) || this->state->CheckFlag(this->REST))return false;
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
/// �U���ł��邩
/// </summary>
const bool Boss::CanRest()const
{
	/*�U�������A�j���[�V�������I�����Ă���*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->model->GetIsChangeAnim())
	{
		//�R���{�J�E���g��0��������
		if (this->attackComboCount <= 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
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

/// <summary>
/// ���ڂ��Ƃ̊֐��̒ǉ�
/// </summary>
void Boss::AddItemFunction(const unsigned int _item, const FlagsState _update)
{
	FlagsStateSet add;
	add.update = _update;
	this->stateFunctionMap.emplace(_item, add);
}
/// <summary>
/// �_���[�W�̎擾
/// </summary>
const int Boss::GetDamage()const
{
	return this->damage;
}
const bool Boss::IsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}

const VECTOR Boss::GetHeadPosition()const
{
	return MV1GetFramePosition(this->model->GetModelHandle(), 7);
}