#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "GoriLib.h"
#include "GameObjectTag.h"
#include "Animation.h"
#include "Model.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "Boss.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "Vector4.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Boss::Boss()
	: Collidable(Collidable::Priority::HIGH,GameObjectTag::BOSS,GoriLib::ColliderData::Kind::CAPSULE,false)
	, state		 (nullptr)
	, animation  (nullptr)
	, moveVector { 0.0f, 0.0f, 0.0f }
	, moveTarget { 0.0f, 0.0f, 0.0f }
	, speed		 (0.0f)
	, modelHandle(-1)
	, isGround	 (false)
	, isHitAttack(false)
	, isDraw	 (true)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->animation	= new Animation();
	this->state		= new BitFlag();
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


	/*�R���C�_�[�f�[�^�̍쐬*/
	auto capsuleColiderData = dynamic_cast<GoriLib::ColliderDataCapsule*>(this->colliderData);
	capsuleColiderData->radius = json.GetJson(JsonManager::FileType::PLAYER)["HIT_RADIUS"];
	capsuleColiderData->height = json.GetJson(JsonManager::FileType::PLAYER)["HIT_HEIGHT"];
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boss::~Boss()
{
	DeleteMemberInstance(this->animation);
	DeleteMemberInstance(this->state);
	this->stateFunctionMap.clear();
	this->stateAnimationMap.clear();
	this->frameCount.clear();
	this->isCount.clear();
}

void Boss::Initialize(GoriLib::Physics* _physics)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//��]��
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //�g�嗦

	this->speed = 0.0f;
	this->hp = 1000;
	this->damage = 5;

	/*�R���C�_�[�̏�����*/
	Collidable::Initialize(_physics);

	/*���������̏�����*/
	this->rigidbody.Initialize(true);
	this->rigidbody.SetPosition(POSITION);
	this->rigidbody.SetRotation(ROTATION);
	this->rigidbody.SetScale(SCALE);
	this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];

	this->state->SetFlag(this->IDLE);
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// �㏈��
/// </summary>
void Boss::Finalize(GoriLib::Physics* _physics)
{
	/*�����o�^�̉���*/
	Collidable::Finalize(_physics);
}

/// <summary>
/// �X�V
/// </summary>
void Boss::Update(GoriLib::Physics* _physics)
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

	Move();

	/*��Ԃ̐؂�ւ�*/
	ChangeState();

	/*�A�j���[�V�����̍X�V*/
	this->nowAnimation = this->stateAnimationMap[this->state->GetFlag()];
	this->animationPlayTime = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	VECTOR position = this->rigidbody.GetPosition();
	this->animation->Play(&this->modelHandle, position, this->nowAnimation, this->animationPlayTime);
}

/// <summary>
/// ���K
/// </summary>
void Boss::Taunt()
{
	/*���K���ɃA�j���[�V�������I�����Ă�����t���O������*/
	if (this->state->CheckFlag(this->ROAR) && this->animation->GetIsChangeAnim())
	{
		this->state->ClearFlag(this->ROAR);
	}
}
/// <summary>
/// �ړ�
/// </summary>
void Boss::Move()
{
	/*�ړ��x�N�g���̏�����*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*��]�����o��*/
	UpdateRotation();

	/*�ړ����x�̍X�V*/
	UpdateVelocity();

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
		if (this->animation->GetIsChangeAnim())
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
	const float TARGET_DISTANCE = VSize(VSub(player.GetPosition(), this->rigidbody.GetPosition()));//�v���C���[�Ƃ̋��������߂�
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
		VECTOR toPlayer = VNorm(VSub(player.GetPosition(), this->rigidbody.GetPosition()));
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
		MV1SetPosition(this->modelHandle, this->rigidbody.GetPosition());
		MV1SetRotationXYZ(this->modelHandle, this->rigidbody.GetRotation());
		MV1SetScale(this->modelHandle, this->rigidbody.GetScale());
		MV1DrawModel(this->modelHandle);
	}
}

/// <summary>
/// ���x�̍X�V
/// </summary>
void Boss::UpdateMoveVector()
{
	/*�ړ��x�N�g��������������*/
	VECTOR direction = { 0.0f,0.0f,0.0f };
	VECTOR rotation = this->rigidbody.GetRotation();

	/*��]�������ƂɈړ��x�N�g�����o��*/
	direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*�ړ��x�N�g���𐳋K��*/
	direction = VNorm(direction);

	/*�ړ��x�N�g���̕�������*/
	this->direction = direction;

	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);

	this->rigidbody.SetVelocity(newVelocity);
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
		this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
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
const VECTOR Boss::GetPosition()const
{
	return this->rigidbody.GetPosition();
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
	const VECTOR position  = this->rigidbody.GetPosition();//���W
		  VECTOR rotation  = { 0.0f,0.0f,0.0f };		 //��]��
		  this->moveTarget = player.GetPosition();
	
	/*�v���C���[���玩���̍��W�܂ł̃x�N�g�����o��*/
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);

	/*�A�[�N�^���W�F���g���g���Ċp�x�����߂�*/
	rotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));
	//rotation.y = rotation.y * 180.0f / DX_PI_F;

	/*��]������*/
	this->rigidbody.SetRotation(rotation);
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
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
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
	return MV1GetFramePosition(this->modelHandle, 7);
}
void Boss::OnCollide(const Collidable& _colider)
{
	std::string message = "�{�X��";
	if (_colider.GetTag() == GameObjectTag::BOSS)
	{
		message += "�v���C���[";
	}
	else if (_colider.GetTag() == GameObjectTag::GROUND)
	{
		message += "�n��";
	}

	message += "�Ɠ�������\n";
	printfDx(message.c_str());
}