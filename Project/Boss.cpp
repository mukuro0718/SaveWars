#include <DxLib.h>
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
	auto tauntSet	  = [this]() {Taunt(); };
	auto moveSet	  = [this]() {Move(); };
	auto reactionSet  = [this]() {Reaction(); };
	auto deathSet	  = [this]() {Death(); };
	auto restSet	  = [this]() {Rest(); };
	auto attackSet	  = [this]() {Attack(); };
	this->AddItemFunction(this->IDLE					, idleSet);
	this->AddItemFunction(this->TAUNT					, tauntSet);
	this->AddItemFunction(this->WALK_FRONT				, moveSet);
	this->AddItemFunction(this->WALK_LEFT				, moveSet);
	this->AddItemFunction(this->WALK_RIGHT				, moveSet);
	this->AddItemFunction(this->VERTICAL_SLASH			, attackSet);
	this->AddItemFunction(this->HORIZONTAL_SLASH		, attackSet);
	this->AddItemFunction(this->ROTATION_SLASH			, attackSet);
	this->AddItemFunction(this->KNOCK_UP_SLASH			, attackSet);
	this->AddItemFunction(this->STRONG_HORIZONTAL_SLASH	, attackSet);
	this->AddItemFunction(this->TWO_COMBO				, attackSet);
	this->AddItemFunction(this->THREE_COMBO				, attackSet);
	this->AddItemFunction(this->REACTION				, reactionSet);
	this->AddItemFunction(this->DEATH					, deathSet);
	this->AddItemFunction(this->REST					, restSet);
	/*�A�j���[�V�����}�b�v�̐ݒ�*/
	this->stateAnimationMap.emplace(this->IDLE					  , static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->TAUNT					  , static_cast<int>(AnimationType::TAUNT));
	this->stateAnimationMap.emplace(this->WALK_FRONT			  , static_cast<int>(AnimationType::WALK_FRONT));
	this->stateAnimationMap.emplace(this->WALK_LEFT				  , static_cast<int>(AnimationType::WALK_LEFT));
	this->stateAnimationMap.emplace(this->WALK_RIGHT			  , static_cast<int>(AnimationType::WALK_RIGHT));
	this->stateAnimationMap.emplace(this->VERTICAL_SLASH		  , static_cast<int>(AnimationType::VERTICAL_SLASH));
	this->stateAnimationMap.emplace(this->HORIZONTAL_SLASH		  , static_cast<int>(AnimationType::HORIZONTAL_SLASH));
	this->stateAnimationMap.emplace(this->ROTATION_SLASH		  , static_cast<int>(AnimationType::ROTATION_SLASH));
	this->stateAnimationMap.emplace(this->KNOCK_UP_SLASH		  , static_cast<int>(AnimationType::KNOCK_UP_SLASH));
	this->stateAnimationMap.emplace(this->STRONG_HORIZONTAL_SLASH , static_cast<int>(AnimationType::STRONG_HORIZONTAL_SLASH));
	this->stateAnimationMap.emplace(this->TWO_COMBO				  , static_cast<int>(AnimationType::TWO_COMBO));
	this->stateAnimationMap.emplace(this->THREE_COMBO			  , static_cast<int>(AnimationType::THREE_COMBO));
	this->stateAnimationMap.emplace(this->REACTION				  , static_cast<int>(AnimationType::REACTION));
	this->stateAnimationMap.emplace(this->DEATH					  , static_cast<int>(AnimationType::DEATH));
	this->stateAnimationMap.emplace(this->REST					  , static_cast<int>(AnimationType::IDLE));

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
	unsigned int flag = this->state->GetFlag();
	this->stateFunctionMap[flag].update();
}

/// <summary>
/// ���K
/// </summary>
void Boss::Taunt()
{
	/*���K���ɃA�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->TAUNT) && this->model->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->TAUNT);
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
		this->state->SetFlag(this->DEATH);
		if (this->model->GetIsChangeAnim())
		{
			this->isDraw = false;
		}
	}

	/*�f�X���Ă��Ȃ����*/
	if (this->state->CheckFlag(this->DEATH))return;

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
	const float MAX_TARGET_DISTANCE = json.GetJson(JsonManager::FileType::ENEMY)["TARGET_DISTANCE"];//�ڕW�Ƃ̍ő勗��
	//�����v���C���[�Ƃ̋������ő勗���ȏ㗣��Ă�����ǐՂ���
	if (TARGET_DISTANCE >= MAX_TARGET_DISTANCE)
	{
		this->state->ClearFlag(this->MASK_MOVE | this->IDLE);
		this->state->SetFlag(this->WALK_FRONT);
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
		this->state->ClearFlag(this->MASK_MOVE | this->IDLE);

		/*phase1*/
		//�������Ă�������ƃv���C���[�ւ̙��K�̂Ȃ��Ȃ����ȏゾ�������]�؂������
		VECTOR toPlayer = VNorm(VSub(player.GetPosition(),this->model->GetPosition()));
		this->dot = VDot(this->direction, toPlayer);
		this->dot = this->dot * 180.0f / DX_PI_F;
		if (this->dot < 0.0f)
		{
			this->state->SetFlag(this->ROTATION_SLASH);
			attackType = static_cast<int>(AttackType::ROTATION_SLASH);
		}
		//�ȑO�̍U�����������Ă�����R���{�U��������
		else if (this->isHitAttack)
		{
			this->state->SetFlag(this->TWO_COMBO);
			attackType = static_cast<int>(AttackType::TWO_COMBO);
		}
		//�c�؂肩���؂�̓����_���Ō��߂�
		else
		{
			int type = GetRand(1);
			if (type == 0)
			{
				this->state->SetFlag(this->VERTICAL_SLASH);
				attackType = static_cast<int>(AttackType::VERTICAL_SLASH);
			}
			else
			{
				this->state->SetFlag(this->HORIZONTAL_SLASH);
				attackType = static_cast<int>(AttackType::HORIZONTAL_SLASH);
			}
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

	if (this->state->CheckFlag(this->MASK_MOVE))
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
	///*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	//auto& json = Singleton<JsonManager>::GetInstance();
	////auto& player = Singleton<PlayerManager>::GetInstance();

	///*�U���O*/
	//int WAIT_TIME = json.GetJson(JsonManager::FileType::ENEMY)["WAIT_TIME_BEFORE_JUMP"];
	////�ڕW�ҋ@���ԂɒB���Ă��Ȃ������瑁�����^�[��
	//if (this->waitingCountBeforeAttack < WAIT_TIME)
	//{
	//	/*��]���̍X�V*/
	//	UpdateRotation();
	//	//�J�E���g�𑝉�
	//	this->waitingCountBeforeAttack++;
	//	//�W�����v�͂̏�����
	//	this->jumpPower = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_POWER"];
	//	//�������^�[��
	//	return;
	//}

	///*�U����*/
	//int ATTACK_TIME = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_TIME"];
	//float ATTACK_RANGE = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_RANGE"];

	////�ڕW�U�����ԂɒB���Ă����痎��������
	//if (this->attackCount >= ATTACK_TIME)
	//{
	//	//���W�̎擾
	//	VECTOR position = this->model->GetPosition();
	//	//�d�͂̎擾
	//	const float GRAVITY = json.GetJson(JsonManager::FileType::ENEMY)["GRAVITY"];
	//	//�ړ��x�N�g���̍X�V
	//	VECTOR moveVector = VGet(0.0f, GRAVITY, 0.0f);
	//	//���W�̍X�V
	//	position = VAdd(position, moveVector);

	//	/*�n�ʂɂ��Ă�����ҋ@��Ԃɂ���*/
	//	const float HIT_SCALE = json.GetJson(JsonManager::FileType::ENEMY)["HIT_SCALE"];
	//	if (position.y <= HIT_SCALE)
	//	{
	//		position.y = HIT_SCALE;
	//		//�u�ҋ@�v��Ԃɐݒ肷��
	//		this->state->ClearFlag(this->attackType);
	//		this->state->SetFlag(this->IDLE);
	//		this->isRest = true;
	//	}
	//	this->model->SetPosition(position);
	//	return;
	//}
	////�^�[�Q�b�g���W�ɋ߂Â�����J�E���g�𑝉�
	//VECTOR moveTarget = this->moveTarget;
	//moveTarget.y = 0.0f;
	//VECTOR position = this->model->GetPosition();
	//position.y = 0.0f;
	//float size = VSize(VSub(moveTarget, position));
	//if (size <= ATTACK_RANGE)
	//{
	//	this->attackCount++;
	//}
	//if (this->attackCount == 0)
	//{
	//	/*���W�ړ�*/
	//	//���x�̎擾
	//	const float VELOCITY = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_VELOCITY"];
	//	//���W�̎擾
	//	VECTOR position = this->model->GetPosition();
	//	//�W�����v
	//	const float JUMP_HEIGHT = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_HEIGHT"];
	//	moveTarget.y = JUMP_HEIGHT;
	//	this->moveVector = VNorm(VSub(moveTarget, this->model->GetPosition()));
	//	//���W�̍X�V
	//	position = VAdd(position, VScale(this->moveVector, VELOCITY));
	//	this->model->SetPosition(position);
	//}
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
	if (this->state->CheckFlag(this->TAUNT))return false;
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