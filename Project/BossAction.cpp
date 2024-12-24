#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"	
#include "DeleteInstance.h"
#include "VECTORtoUseful.h"
#include "Rigidbody.h"
#include "ReactionType.h"
#include "ActionParameter.h"
#include "HitStop.h"
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "BossAttack.h"
#include "Boss.h"
#include "BossAction.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BossAction::BossAction()
	: parameter		(nullptr)
	, attack		(nullptr)
	, hitStop		(nullptr)
	, moveTarget	(Gori::ORIGIN)
	, isSelect		(false)
	, isInitialize	(false)
	, isPriority	(false)
	, isAllowAction (false)
	, frameCount	(0)
{
	/*�C���X�^���X�̍쐬*/
	this->parameter = new ActionParameter();
	this->hitStop	= new HitStop();

	/*functionMap�̍쐬*/
	auto getDif = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetDifColorScale(_modelHandle); };
	auto getSpc = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetSpcColorScale(_modelHandle); };
	auto getEmi = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetEmiColorScale(_modelHandle); };
	auto getAmb = [](COLOR_F& _base, COLOR_F& _now, const int _modelHandle) {_base = _now = MV1GetAmbColorScale(_modelHandle); };
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::DIF), getDif);
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::SPC), getSpc);
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::EMI), getEmi);
	this->getColorScaleMap.emplace(static_cast<int>(ColorType::AMB), getAmb);
	auto setDif = [](const COLOR_F _color, const int _modelHandle) {MV1SetDifColorScale(_modelHandle, _color); };
	auto setSpc = [](const COLOR_F _color, const int _modelHandle) {MV1SetSpcColorScale(_modelHandle, _color); };
	auto setEmi = [](const COLOR_F _color, const int _modelHandle) {MV1SetEmiColorScale(_modelHandle, _color); };
	auto setAmb = [](const COLOR_F _color, const int _modelHandle) {MV1SetAmbColorScale(_modelHandle, _color); };
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::DIF), setDif);
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::SPC), setSpc);
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::EMI), setEmi);
	this->setColorScaleMap.emplace(static_cast<int>(ColorType::AMB), setAmb);

	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*colorScaleList�̍쐬*/
	COLOR_F color = { 1.0f,1.0f,1.0f,1.0f };
	for (int i = 0; i < json.GetJson(JsonManager::FileType::ENEMY)["COLOR_SCALE_NUM"]; i++)
	{
		this->baseColorScale.emplace_back(color);
		this->nowColorScale.emplace_back(color);
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BossAction::~BossAction()
{
	DeleteMemberInstance(this->attack);
	DeleteMemberInstance(this->hitStop);
	DeleteMemberInstance(this->parameter);
	this->getColorScaleMap.clear();
	this->setColorScaleMap.clear();
	this->baseColorScale.clear();
	this->nowColorScale.clear();
}
/// <summary>
/// �`��
/// </summary>
const void BossAction::Draw()const
{
	//if (this->attack != nullptr)
	//{
	//	this->attack->Draw();
	//}
	//printfDx("ENEMY_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// �~���l�̎擾
/// </summary>
const int BossAction::GetDesireValue() const
{
	return this->parameter->desireValue;
}

/// <summary>
/// �d�݂̎擾
/// </summary>
const int BossAction::GetWeight(const int _sum)const
{
	return this->parameter->GetWeight(_sum);
}

/// <summary>
/// �d�݂̃x�[�X�̎擾
/// </summary>
const int BossAction::GetBaseWeight()const
{
	return this->parameter->BASE_WEIGHT;
}
/// <summary>
/// �w�肵���t���[���̌v��
/// </summary>
bool BossAction::FrameCount(const int _maxFrame)
{
	//�J�E���g�𑝉�������
	this->frameCount++;
	//�����ő���z���Ă�����t���O�������ăJ�E���g������������
	if (this->frameCount >= _maxFrame)
	{
		return true;
	}
	return false;
}

/// <summary>
/// �ϐ��̏�����
/// </summary>
void BossAction::OffIsSelect(const int _maxInterval)
{
	/*�I������Ă�����*/
	if (this->isSelect)
	{
		this->isSelect = false;
		this->isPriority = false;
		this->isAllowAction = false;
		this->frameCount = 0;
		this->parameter->desireValue = 0;
	}
}

/// <summary>
/// �⊮������]���̎擾
/// </summary>
VECTOR BossAction::GetLerpRotation(Boss& _boss, const VECTOR _positionToTargetVector, const VECTOR _nowRotation, const VECTOR _lerpValue)
{
	VECTOR nextRotation = Gori::ORIGIN;
	VECTOR rotation = Gori::ORIGIN;
	//���̉�]�������߂�
	nextRotation.y = static_cast<float>(atan2(static_cast<double>(_positionToTargetVector.x), static_cast<double>(_positionToTargetVector.z)));
	//��]����ۊǂ���
	rotation = _boss.Lerp360Angle(_nowRotation, nextRotation, _lerpValue);
	return rotation;
}

/// <summary>
/// �ړ��x�N�g���̌v�Z
/// </summary>
VECTOR BossAction::CalcVelocity(const VECTOR _prevVelocity, const VECTOR _nowRotation, const float _speed)
{
	//��]�������ƂɈړ��x�N�g�����o��
	VECTOR direction = VGet(-sinf(_nowRotation.y), 0.0f, -cosf(_nowRotation.y));
	//�ړ��x�N�g���𐳋K��
	direction = VNorm(direction);
	//�V�����ړ��x�N�g�����o���i�d�͂��l�����āAY�����̂ݑO�̂��̂��g�p����j
	VECTOR aimVelocity = VScale(direction, _speed);					 //�Z�o���ꂽ�ړ��x�N�g��
	VECTOR newVelocity = VGet(aimVelocity.x, _prevVelocity.y, aimVelocity.z);//�V�����ړ��x�N�g��

	return newVelocity;
}


/// <summary>
/// ���[�v
/// </summary>
float BossAction::Lerp(const float _start, const float _end, const float _percent)
{
	return _start + _percent * (_end - _start);
}
COLOR_F BossAction::LerpColor(const COLOR_F _start, const COLOR_F _end, const COLOR_F _percent)
{
	COLOR_F out;
	out.r = Lerp(_start.r, _end.r, _percent.r);
	out.g = Lerp(_start.g, _end.g, _percent.g);
	out.b = Lerp(_start.b, _end.b, _percent.b);
	out.a = Lerp(_start.a, _end.a, _percent.a);
	return out;
}

/// <summary>
/// vector<float>��COLOR_F�ɕϊ�
/// </summary>
const COLOR_F BossAction::ColorConvert(const std::vector<float> _value)
{
	const COLOR_F out = { _value[0],_value[1], _value[2],_value[3] };
	return out;
}
