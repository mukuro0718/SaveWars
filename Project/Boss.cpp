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
	, collider	(nullptr)
	, moveVector{ 0.0f, 0.0f, 0.0f }
	, moveTarget{ 0.0f, 0.0f, 0.0f }
	, velocity	(0.0f)
	, waitingCountBeforeAttack(0)
	, restCount(0)
	, attackCount(0)
	, isRest(false)
	, attackType(0)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�����o�N���X�̃C���X�^���X�̍쐬*/
	this->model		= new Model(asset.GetModel(LoadingAsset::ModelType::ENEMY));
	this->state		= new BitFlag();
	this->collider	= new Collider();

	/*�U���֐��̐ݒ�*/
	//auto rushAttack = [this]() {this->RushAttack(); };
	//this->attackFunctionMap.emplace(this->RUSH, rushAttack);

	/*�U����ނ̐ݒ�*/
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::RUSH), this->RUSH);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::JUMP), this->JUMP);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::LASER), this->LASER);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::SPIN), this->SPIN);
	//this->attackTypeMap.emplace(static_cast<int>(AttackType::INHIBITION), this->INHIBITION);

	this->state->SetFlag(this->IDLE);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Boss::~Boss()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	DeleteMemberInstance(this->collider);
	this->attackFunctionMap.clear();
}

void Boss::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//���W
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//��]��
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //�g�嗦

	this->velocity = 0.0f;
	this->waitingCountBeforeAttack = 0;
	this->restCount = 0;
	this->attackCount = 0;
	this->isRest = false;
	this->attackType = 0;

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

	/*�U���̐ݒ�*/
	SetState();

	/*�R���C�_�[�̐ݒ�*/
	const VECTOR MAX_OFFSET = Convert(json.GetJson(JsonManager::FileType::ENEMY)["MAX_HIT_POSITION_OFFSET"]);
	const VECTOR MIN_OFFSET = Convert(json.GetJson(JsonManager::FileType::ENEMY)["MIN_HIT_POSITION_OFFSET"]);
	const VECTOR POSITION = this->model->GetPosition();
	this->collider->SetOBB(POSITION + MAX_OFFSET, POSITION + MIN_OFFSET,this->model->GetPosition(), this->model->GetRotation());
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
void Boss::SetState()
{
	/*�x�e���Ă����瑁�����^�[��*/
	if (IsRest())return;

	/*�ړ�*/
	Move();

	/*�U������*/
	//Attack();
}
bool Boss::IsRest()
{
	/*�x�e���Ă��Ȃ������瑁�����^�[��*/
	if (!this->isRest)return false;


	/*�x�e����������*/
	//�J�E���g�𑝂₷
	this->restCount++;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	const float REST_COUNT = json.GetJson(JsonManager::FileType::ENEMY)["REST_COUNT"];

	/*�J�E���g���ڕW�J�E���g�𒴂��Ă����瑁�����^�[��*/
	if (this->restCount >= REST_COUNT)
	{
		this->isRest = false;
		this->restCount = 0;
		return false;
	}

	return true;
}

/// <summary>
/// �ړ�
/// </summary>
void Boss::Move()
{
	/*�ړ��ł��Ȃ������瑁�����^�[��*/
	if (!CanMove())return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�v���C���[����̋������萔�ȏゾ������ǐ�*/
	const float CHASE_RANGE = json.GetJson(JsonManager::FileType::ENEMY)["CHASE_RANGE"];
	const 
}

/// <summary>
/// �`��
/// </summary>
const void Boss::Draw()const
{
	VECTOR position = this->model->GetPosition();
	VECTOR rotation = this->model->GetRotation();
	printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("Boss_VELOCITY:%f\n", this->velocity);
	this->model->Draw();
	this->collider->DrawHitBox();
}

/// <summary>
/// ���x�̍X�V
/// </summary>
void Boss::UpdateMoveVector()
{
	VECTOR rotation = this->model->GetRotation();

	/*��]�������ƂɈړ��x�N�g�����o��*/
	this->moveVector = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*�ړ��x�N�g���ɑ��x�����Z*/
	this->moveVector = VNorm(this->moveVector);
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
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*�g�p����l�̏���*/
	const VECTOR position		= this->model->GetPosition();//���W
		  VECTOR rotation		= { 0.0f,0.0f,0.0f };		 //��]��
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

/// <summary>
/// �ړ��ł��邩
/// </summary>
const bool Boss::CanMove()const
{
	if (this->isRest)						 return false;//�x�e����
	if (this->state->CheckFlag(this->TAUNT)) return false;//���K����
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

const Collider Boss::GetCollider()
{
	return *this->collider;
}

/// <summary>
/// �ːi�U��
/// </summary>
void Boss::RushAttack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�U���O*/
	int WAIT_TIME = json.GetJson(JsonManager::FileType::ENEMY)["WAIT_TIME_BEFORE_RUSH"];
	//�ڕW�ҋ@���ԂɒB���Ă��Ȃ������瑁�����^�[��
	if (this->waitingCountBeforeAttack < WAIT_TIME)
	{
		/*��]���̍X�V*/
		UpdateRotation();
		//�ړ��x�N�g���̐ݒ�
		UpdateMoveVector();
		//�J�E���g�𑝉�
		this->waitingCountBeforeAttack++;
		//�������^�[��
		return;
	}

	/*�U����*/
	int ATTACK_TIME = json.GetJson(JsonManager::FileType::ENEMY)["RUSH_TIME"];
	//�J�E���g�𑝉�
	this->attackCount++;
	//�ڕW�U�����ԂɒB���Ă�����u�ҋ@�v��ԂɕύX���đ������^�[��
	if (this->attackCount >= ATTACK_TIME)
	{
		//�u�ҋ@�v��Ԃɐݒ肷��
		this->state->ClearFlag(this->attackType);
		this->state->SetFlag(this->IDLE);
		this->isRest = true;
		return;
	}
	const float VELOCITY = json.GetJson(JsonManager::FileType::ENEMY)["RUSH_VELOCITY"];
	VECTOR position = this->model->GetPosition();

	position = VAdd(position, VScale(this->moveVector, VELOCITY));
	this->model->SetPosition(position);
}
/// <summary>
/// �W�����v�U��
/// </summary>
void Boss::JumpAttack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	//auto& player = Singleton<PlayerManager>::GetInstance();

	/*�U���O*/
	int WAIT_TIME = json.GetJson(JsonManager::FileType::ENEMY)["WAIT_TIME_BEFORE_JUMP"];
	//�ڕW�ҋ@���ԂɒB���Ă��Ȃ������瑁�����^�[��
	if (this->waitingCountBeforeAttack < WAIT_TIME)
	{
		/*��]���̍X�V*/
		UpdateRotation();
		//�J�E���g�𑝉�
		this->waitingCountBeforeAttack++;
		//�W�����v�͂̏�����
		this->jumpPower = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_POWER"];
		//�������^�[��
		return;
	}

	/*�U����*/
	int ATTACK_TIME = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_TIME"];
	float ATTACK_RANGE = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_RANGE"];

	//�ڕW�U�����ԂɒB���Ă����痎��������
	if (this->attackCount >= ATTACK_TIME)
	{
		//���W�̎擾
		VECTOR position = this->model->GetPosition();
		//�d�͂̎擾
		const float GRAVITY = json.GetJson(JsonManager::FileType::ENEMY)["GRAVITY"];
		//�ړ��x�N�g���̍X�V
		VECTOR moveVector = VGet(0.0f, GRAVITY, 0.0f);
		//���W�̍X�V
		position = VAdd(position, moveVector);

		/*�n�ʂɂ��Ă�����ҋ@��Ԃɂ���*/
		const float HIT_SCALE = json.GetJson(JsonManager::FileType::ENEMY)["HIT_SCALE"];
		if (position.y <= HIT_SCALE)
		{
			position.y = HIT_SCALE;
			//�u�ҋ@�v��Ԃɐݒ肷��
			this->state->ClearFlag(this->attackType);
			this->state->SetFlag(this->IDLE);
			this->isRest = true;
		}
		this->model->SetPosition(position);
		return;
	}
	//�^�[�Q�b�g���W�ɋ߂Â�����J�E���g�𑝉�
	VECTOR moveTarget = this->moveTarget;
	moveTarget.y = 0.0f;
	VECTOR position = this->model->GetPosition();
	position.y = 0.0f;
	float size = VSize(VSub(moveTarget, position));
	if (size <= ATTACK_RANGE)
	{
		this->attackCount++;
	}
	if (this->attackCount == 0)
	{
		/*���W�ړ�*/
		//���x�̎擾
		const float VELOCITY = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_VELOCITY"];
		//���W�̎擾
		VECTOR position = this->model->GetPosition();
		//�W�����v
		const float JUMP_HEIGHT = json.GetJson(JsonManager::FileType::ENEMY)["JUMP_HEIGHT"];
		moveTarget.y = JUMP_HEIGHT;
		this->moveVector = VNorm(VSub(moveTarget, this->model->GetPosition()));
		//���W�̍X�V
		position = VAdd(position, VScale(this->moveVector, VELOCITY));
		this->model->SetPosition(position);
	}
}