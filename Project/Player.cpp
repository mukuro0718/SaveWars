#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"
#include "Animation.h"
#include "Model.h"
#include "BitFlag.h"
#include "LoadingAsset.h"
#include "InputManager.h"
#include "Satellite.h"
#include "Shield.h"
#include "Player.h"
#include "CameraManager.h"
#include "EnemyManager.h"
#include "Vector4.h"
#include "Collider.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player()
	: model				(nullptr)
	, state				(nullptr)
	, shield			(nullptr)
	, collider			(nullptr)
	, moveVector		{ 0.0f, 0.0f, 0.0f }
	, direction			{ 0.0f, 0.0f, 0.0f }
	, fixVector			{ 0.0f, 0.0f, 0.0f }
	, moveVectorRotation{ 0.0f, 0.0f, 0.0f }
	, wasd				{ 0.0f, 0.0f, 0.0f }
	, lStick			{ 0.0f, 0.0f, 0.0f }
	, velocity			(0.0f)
	, isShot			(false)
	, isShowElementWheel(false)
	, elementAngle		(0.0f)
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�C���X�^���X�̍쐬*/
	this->model = new Model(asset.GetModel(LoadingAsset::ModelType::PLAYER));
	this->shield = new Shield();
	this->state = new BitFlag();
	this->collider = new Collider();
	for (int i = 0; i < this->SATELLITE_NUM; i++)
	{
		this->satellite[i] = new Satellite();
	}

	/*vector�̒ǉ�*/
	this->frameCount.emplace_back(0);
	this->isCount.emplace_back(false);

	/*�A�j���[�V�����̐ݒ�*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	this->model->AddAnimation(animationHandle, animationIndex);
	this->animationMap.emplace(this->IDLE		 , static_cast<int>(AnimationType::IDLE));
	this->animationMap.emplace(this->WALK		 , static_cast<int>(AnimationType::WALK));
	this->animationMap.emplace(this->RUN		 , static_cast<int>(AnimationType::RUN));
	this->animationMap.emplace(this->AVOID		 , static_cast<int>(AnimationType::AVOID));
	this->animationMap.emplace(this->JUMP		 , static_cast<int>(AnimationType::JUMP));
	this->animationMap.emplace(this->FLAME_ATTACK, static_cast<int>(AnimationType::FLAME_ATTACK));
	this->animationMap.emplace(this->BOLT_ATTACK , static_cast<int>(AnimationType::BOLT_ATTACK));
	this->animationMap.emplace(this->ICE_ATTACK	 , static_cast<int>(AnimationType::ICE_ATTACK));
	this->animationMap.emplace(this->EARTH_ATTACK, static_cast<int>(AnimationType::EARTH_ATTACK));
	this->animationMap.emplace(this->STORM_ATTACK, static_cast<int>(AnimationType::STORM_ATTACK));
	this->attackTypeMap.emplace(static_cast<int>(Element::FRAME), this->FLAME_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::BOLT), this->BOLT_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::ICE), this->ICE_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::EARTH), this->EARTH_ATTACK);
	this->attackTypeMap.emplace(static_cast<int>(Element::STORM), this->STORM_ATTACK);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	DeleteMemberInstance(this->model);
	DeleteMemberInstance(this->state);
	for (int i = 0; i < this->SATELLITE_NUM; i++)
	{
		DeleteMemberInstance(this->satellite[i]);
	}
	DeleteMemberInstance(this->shield);
	DeleteMemberInstance(this->collider);
	this->frameCount.clear();
	this->isCount.clear();
	this->animationMap.clear();
	this->attackTypeMap.clear();
}

void Player::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*json�f�[�^���e�萔�^�ɑ��*/
	const VECTOR position = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_POSITION"]);//���W
	const VECTOR rotation = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_ROTATION"]);//��]��
	const VECTOR scale = Convert(json.GetJson(JsonManager::FileType::PLAYER)["INIT_SCALE"]);	 //�g�嗦
	float ANGLE_OFFSET = json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_ANGLE_OFFSET"];
	this->velocity = 0.0f;
	this->direction = VGet(0.0f, 0.0f, -1.0f);

	/*���f���̃g�����X�t�H�[���̐ݒ�*/
	this->model->SetTransform(position, rotation, scale);

	this->shield->Initialize();

	satellite[0]->Initialize(0.0f,position, Convert(json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_POSITION_OFFSET_A"]));
	satellite[1]->Initialize(180.0f,position, Convert(json.GetJson(JsonManager::FileType::PLAYER)["SATELLITE_POSITION_OFFSET_A"]));
}

/// <summary>
/// �A�N�V����
/// </summary>
void Player::Action()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	UpdateElement();

	/*��]���̍X�V*/
	UpdateRotation();

	/*�ړ�*/
	Move();

	/*�W�����v*/
	Jump();

	/*�U��*/
	Attack();

	/*�R���C�_�[�̐ݒ�*/
	const VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius	  = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];
	this->collider->SetSphere(position, radius);
}

/// <summary>
/// �X�V
/// </summary>
void Player::Update()
{	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	VECTOR position = this->model->GetPosition() + this->moveVector;
	const float  radius = json.GetJson(JsonManager::FileType::PLAYER)["RADIUS"];	 //

	/*Y���W - ���a���O����������������*/
	if (position.y - radius <= 0)
	{
		position.y = radius;
		this->state->ClearFlag(this->JUMP);
		this->jumpPower = 0.0f;
	}
	this->model->SetPosition(position);

	/*�R���C�_�[�̍X�V*/
	this->collider->SetSphere(position, radius);

	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		satellite[0]->Update(true, VNorm(this->direction), this->model->GetPosition());
		satellite[1]->Update(false, VNorm(this->direction), this->model->GetPosition());
	}
	this->shield->Update(this->model->GetPosition());

	/*�A�j���[�V�����̍X�V*/
	UpdateAnimation();
	this->model->PlayAnimation(this->nowAnimation,this->animationPlayTime);
}

/// <summary>
/// �`��
/// </summary>
const void Player::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();

	VECTOR position = this->model->GetPosition();
	VECTOR rotation = this->model->GetRotation();
	printfDx("PLAYER_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("PLAYER_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("PLAYER_VELOCITY:%f\n", this->velocity);
	printfDx("ELEMENT_ANGLE:%f\n", this->elementAngle);

	this->model->Draw();
	for (int i = 0; i < this->SATELLITE_NUM; i++)
	{
		satellite[i]->Draw();
	}
	DrawLine(950, 540, 970, 541, GetColor(200, 200, 200), 2);
	DrawLine(960, 530, 961, 550, GetColor(200, 200, 200), 2);

	this->collider->DrawHitSphere();
}

/// <summary>
/// �ړ�
/// </summary>
void Player::Move()
{
	/*�ړ��x�N�g���̏�����*/
	this->moveVector = { 0.0f,0.0f,0.0f };

	/*�ړ��ł��邩*/
	if (!CanMove())return;

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& camera = Singleton<CameraManager>::GetInstance();

	/*�X�e�B�b�N���͂��Ȃ���Έړ��t���O��������return��Ԃ�*/
	if (this->lStick.x == 0 && this->lStick.z == 0 && this->wasd.x == 0 && this->wasd.z == 0)
	{
		//�ړ���Ԃ��폜����
		this->state->ClearFlag(this->MASK_MOVE);
	}
	else
	{
		/*�ړ���Ԃ��폜����*/
		this->state->ClearFlag(this->MASK_MOVE);
		
		/*�ړ���Ԃ��Z�b�g*/
		//���邩
		if (input.GetPadState() & PAD_INPUT_6 || CheckHitKey(KEY_INPUT_LSHIFT))
		{
			this->state->SetFlag(this->RUN);
		}
		else
		{
			this->state->SetFlag(this->WALK);
		}
	}

	/*���x�̍X�V*/
	UpdateVelocity();

	/*�ړ��x�N�g���̍X�V*/
	UpdateMoveVector();

	/*�����̍X�V*/
	if (!this->isShot)
	{
		if (state->CheckFlag(this->MASK_MOVE))
		{
			this->direction = this->moveVector;
		}
		this->direction.y = 0.0f;
	}
	else
	{
		this->direction = camera.GetCameraDirection() * -1.0f;
	}
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateMoveVector()
{
	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		/*��]�������ƂɈړ��x�N�g�����o��*/
		this->moveVector = VGet(-sinf(this->moveVectorRotation.y), 0.0f, -cosf(this->moveVectorRotation.y));

		/*�ړ��x�N�g���𐳋K��*/
		this->moveVector = VNorm(this->moveVector);

		/*�ړ��x�N�g���ɑ��x�����Z*/
		this->moveVector = this->moveVector * this->velocity;
	}
}
/// <summary>
/// ���x�̍X�V
/// </summary>
void Player::UpdateVelocity()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	
	/*��Ԃ��ړ���ԂłȂ���Α��x�𗎂Ƃ�*/
	if (!this->state->CheckFlag(this->MASK_MOVE))
	{
		/*�萔�^�ɑ��*/
		float DECEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];

		/*�������x���ő呬�x�ȏゾ�����烊�^�[����Ԃ�*/
		if (this->velocity <= 0.0f)
		{
			this->velocity = 0.0f;
			return;
		}

		/*����������*/
		this->velocity -= DECEL;
	}
	/*�ړ����Ă����瑬�x���ő呬�x�܂ŏグ��*/
	else
	{
		/*�萔�^�ɑ��*/
		float maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["VELOCITY"];
		const float ACCEL = json.GetJson(JsonManager::FileType::PLAYER)["ACCEL"];

		if (this->state->CheckFlag(this->RUN))
		{
			maxVelocity = json.GetJson(JsonManager::FileType::PLAYER)["RUN_VELOCITY"];
		}

		/*�������x���ő呬�x�ȏゾ�����烊�^�[����Ԃ�*/
		if (this->velocity >= maxVelocity)
		{
			this->velocity = maxVelocity;
			return;
		}

		/*����������*/
		this->velocity += ACCEL;
	}
}

/// <summary>
/// ���W��getter
/// </summary>
/// <returns></returns>
const VECTOR Player::GetPosition()const
{
	return this->model->GetPosition();
}

/// <summary>
/// ��]���̍X�V
/// </summary>
void Player::UpdateRotation()
{
	/*�g�p����l�̏���*/
	const float PI			= 180.0f;				//�ʓx�@�ł̃�
	VECTOR rotation			= { 0.0f,0.0f,0.0f };	//��]��
	bool   isInputKey		= false;				//�L�[���͂̎擾
	bool   isInputLStick	= false;				//L�X�e�B�b�N����
	VECTOR cameraDirection	= { 0.0f,0.0f,0.0f };	//�J�����̌���
	this->lStick			= { 0.0f,0.0f,0.0f };	//���X�e�B�b�N���͕ێ��ϐ�
	this->wasd				= { 0.0f,0.0f,0.0f };	//wasd���͕ێ��ϐ�
	
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& camera = Singleton<CameraManager>::GetInstance();
	auto& enemy  = Singleton<EnemyManager>::GetInstance();
	auto& input  = Singleton<InputManager> ::GetInstance();
	auto& json   = Singleton<JsonManager>  ::GetInstance();


	/*���X�e�B�b�N���͂���*/
	lStick = VGet(static_cast<float>(input.GetLStickState().XBuf), 0.0f, static_cast<float>(input.GetLStickState().YBuf));
	if (lStick.x != 0.0f || lStick.z != 0.0f)
	{
		isInputLStick = true;
	}

	/*wasd���͂���*/
	if (CheckHitKey(KEY_INPUT_W))
	{
		isInputKey = true;
		wasd.z = -1.0f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		isInputKey = true;
		wasd.z = 1.0f;
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		isInputKey = true;
		wasd.x = -1.0f;
	}
	if (CheckHitKey(KEY_INPUT_D))
	{
		isInputKey = true;
		wasd.x = 1.0f;
	}

	/*�J�����̌������o��*/
	//�J�������W����v���C���[�̍��W�ւ̃x�N�g�����o��
	cameraDirection = this->direction = camera.GetCameraDirection();
	//���߂��x�N�g���𐳋K������
	cameraDirection = VNorm(cameraDirection);
	//���]���Ă����̂Ńx�N�g���𔽓]����
	cameraDirection = cameraDirection * -1.0f;

	/*�����G�C����Ԃ�������*/
	if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		/*�J�����̌����Ă�����������ƂɃ��f���̉�]�����o���B*/
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x)));
			rotation.y -= 90.0f * (DX_PI_F / 180.0f);
		if (isInputKey)
		{
			this->wasd = VNorm(this->wasd);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->wasd.z), static_cast<double>(this->wasd.x)));
		}
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			this->moveVectorRotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
		}
	}
	else
	{
		/*�J�����̌����Ă�������ƁA�v���C���[���ŏ��Ɍ����Ă������������ƂɃ��f���̉�]�����o���B*/
		if (isInputKey)
		{
			this->wasd = VNorm(this->wasd);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->wasd.z), static_cast<double>(this->wasd.x)));
			this->moveVectorRotation = rotation;
		}
		if (isInputLStick)
		{
			this->lStick = VNorm(this->lStick);
			rotation.y = static_cast<float>(
				-atan2(static_cast<double>(cameraDirection.z), static_cast<double>(cameraDirection.x))
				-atan2(-static_cast<double>(this->lStick.z), static_cast<double>(this->lStick.x)));
			this->moveVectorRotation = rotation;
		}
	}
	rotation = VGet(rotation.x * PI / DX_PI_F, (rotation.y * PI / DX_PI_F), rotation.z * PI / DX_PI_F);

	if (isInputKey || isInputLStick || this->isShot)
	{
		this->model->SetRotation(rotation);
	}
}

/// <summary>
/// �U��
/// </summary>
void Player::Attack()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();

	
	/*�U���ł��邩*/
	if (CanAttack())
	{
		/*�U���֌W�̏�����*/
		this->state->ClearFlag(this->MASK_ATTACK);
		this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = false;
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = 0;


		/*X�{�^���܂��̓L�[�{�[�h��X�������ꂽ��*/
		if (input.GetPadState() & PAD_INPUT_1 || CheckHitKey(KEY_INPUT_X))
		{
			this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] = true;
			unsigned int flag = this->attackTypeMap[this->element];
			this->state->SetFlag(flag);
		}
	}
	else
	{
		this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)]++;
		if (this->model->GetIsChangeAnim())
		{
			this->state->ClearFlag(this->MASK_ATTACK);
			this->state->SetFlag(this->IDLE);
		}
	}
}


/// <summary>
/// �����
/// </summary>
void Player::Jump()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*����ׂ邩*/
	if (!CanJump())return;

	/*�ϐ��̏���*/
	float y = json.GetJson(JsonManager::FileType::PLAYER)["GRAVITY"];

	/*B�{�^���܂��̓L�[�{�[�h��B�������ꂽ��*/
	if (!this->state->CheckFlag(this->JUMP))
	{
		if (input.GetPadState() & PAD_INPUT_4 || CheckHitKey(KEY_INPUT_SPACE))
		{
			this->jumpPower = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_POWER"];
			/*�ړ���Ԃ��Z�b�g*/
			this->state->SetFlag(this->JUMP);
		}
	}
	else
	{
		float jumpDecel = json.GetJson(JsonManager::FileType::PLAYER)["JUMP_DECEL"];
		this->jumpPower -= jumpDecel;
	}
	this->moveVector.y = y + this->jumpPower;
}

/// <summary>
/// state���ړ���ԂɂȂ��Ă��邩
/// </summary>
const bool Player::IsMove()const
{
	return this->state->CheckFlag(this->MASK_MOVE);
}

/// <summary>
/// �ړ��ł��邩
/// </summary>
const bool Player::CanMove()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))	return false;
	return true;
}

/// <summary>
/// �U���ł��邩
/// </summary>
const bool Player::CanAttack()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*�U���ҋ@�p�t���[���J�E���g�t���O�������Ă��Ȃ�������true��Ԃ�*/
	if (!this->isCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)])return true;

	/*�U�����Ƃɍő�t���[���J�E���g�����߂�*/
	int maxFrameCount = 0;
	if (this->state->CheckFlag(this->FLAME_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["FLAME_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->BOLT_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["BOLT_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->ICE_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["ICE_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->EARTH_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["EARTH_MAGIC_INTERVAL"];
	}
	else if (this->state->CheckFlag(this->STORM_ATTACK))
	{
		maxFrameCount = json.GetJson(JsonManager::FileType::PLAYER)["STORM_MAGIC_INTERVAL"];
	}

	/*�����t���[���J�E���g���萔�ȏゾ������true��Ԃ�*/
	if (this->frameCount[static_cast<int>(FrameCountType::ATTACK_INTERVAL)] >= maxFrameCount)return true;

	return false;
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
/// ����ׂ邩
/// </summary>
const bool Player::CanJump()const
{
	return true;
}

/// <summary>
/// �y�U�������Ă��邩
/// </summary>
const bool Player::IsLightAttack()const
{
	return this->state->CheckFlag(this->MASK_ATTACK);
}


/// <summary>
/// �d�U�������Ă��邩
/// </summary>
const bool Player::IsHeavyAttack()const
{
	return this->state->CheckFlag(this->MASK_ATTACK);
}

/// <summary>
/// �ړ��x�N�g���̕␳
/// </summary>
const void Player::FixMoveVector(const VECTOR _fixVector)
{
	this->moveVector = this->moveVector + _fixVector; 
}

/// <summary>
/// �R���C�_�[�̎擾
/// </summary>
const Collider Player::GetCollider()
{
	return *this->collider;
}

/// <summary>
/// �����̍X�V
/// </summary>
void Player::UpdateElement()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& input = Singleton<InputManager> ::GetInstance();
	auto& json = Singleton<JsonManager>  ::GetInstance();

	/*�����z�C�[����\�����邩*/
	if (input.GetPadState() & PAD_INPUT_5)
	{
		this->isShowElementWheel = true;
	}
	else
	{
		//false�Ȃ瑁�����^�[��
		this->isShowElementWheel = false;
		return;
	}

	/*�E�X�e�B�b�N�̌X�������ƂɁA������ύX����*/
	vector<int> buf = { input.GetRStickState().XBuf ,input.GetRStickState().YBuf };
	this->rStick = buf;
	/*�X�e�B�b�N���͂��Ȃ���΃��^�[����Ԃ�*/
	if (this->rStick[0] == 0 && this->rStick[1] == 0)return;
	this->elementAngle = (- atan2(this->rStick[1], this->rStick[0]) * 180.0f / DX_PI_F) - 90.0f;
	if (this->elementAngle < 0)
	{
		this->elementAngle += 360.0f;
		if (this->elementAngle == 360.0f)
		{
			this->elementAngle = 0.0f;
		}
	}

	if (31.0f >= this->elementAngle || this->elementAngle >= 320.0f)
	{
		this->element = static_cast<int>(Element::ICE);
	}
	else if (103.0f >= this->elementAngle && this->elementAngle >= 32.0f)
	{
		this->element = static_cast<int>(Element::BOLT);
	}
	else if (175.0f >= this->elementAngle && this->elementAngle >= 104.0f)
	{
		this->element = static_cast<int>(Element::FRAME);
	}
	else if (247.0f >= this->elementAngle && this->elementAngle >= 176.0f)
	{
		this->element = static_cast<int>(Element::STORM);
	}
	else if (319.0f >= this->elementAngle && this->elementAngle >= 248.0f)
	{
		this->element = static_cast<int>(Element::EARTH);
	}
}

/// <summary>
/// �A�j���[�V�����̍X�V
/// </summary>
void Player::UpdateAnimation()
{
	/*�U�����Ă�����*/
	if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		//��
		if (this->state->CheckFlag(this->FLAME_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::FLAME_ATTACK);
		}
		//��
		else if (this->state->CheckFlag(this->BOLT_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::BOLT_ATTACK);
		}
		//�X
		else if (this->state->CheckFlag(this->ICE_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::ICE_ATTACK);
		}
		//�y
		else if (this->state->CheckFlag(this->EARTH_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::EARTH_ATTACK);
		}
		//��
		else if (this->state->CheckFlag(this->STORM_ATTACK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::STORM_ATTACK);
		}

		return;
	}

	/*������Ă�����*/
	if (this->state->CheckFlag(this->AVOID))
	{
		this->nowAnimation = static_cast<int>(AnimationType::AVOID);
		return;
	}
	/*�W�����v���Ă�����*/
	if (this->state->CheckFlag(this->JUMP))
	{
		this->nowAnimation = static_cast<int>(AnimationType::JUMP);
		return;
	}

	/*�ړ����Ă�����*/
	if (this->state->CheckFlag(this->MASK_MOVE))
	{
		//����
		if (this->state->CheckFlag(this->RUN))
		{
			this->nowAnimation = static_cast<int>(AnimationType::RUN);
		}
		//����
		if (this->state->CheckFlag(this->WALK))
		{
			this->nowAnimation = static_cast<int>(AnimationType::WALK);
		}
		return;
	}
	/*�������Ă��Ȃ�������ҋ@���*/
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
}