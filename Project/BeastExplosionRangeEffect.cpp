#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BeastExplosionRangeEffect.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BeastExplosionRangeEffect::BeastExplosionRangeEffect(const int _effectResourceHandle)
	: Effect			 (_effectResourceHandle)
	, scale				 { 0.0f,0.0f,0.0f }
	, playingEffectHandle(-1)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->scale		 = Gori::Convert(json.GetJson(JsonManager::FileType::EFFECT)["BEAST_EXPLOSION_RANGE_SCALE"]);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_EXPLOSION_RANGE_START_FRAME"];
	this->endFrame	 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_EXPLOSION_RANGE_END_FRAME"];
	this->useIndexNum = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_EXPLOSION_USE_FRAME"];
}

/// <summary>
/// �X�V
/// </summary>
void BeastExplosionRangeEffect::Update()
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect)return;

	/*�����̊J�n���Ɉ�x�����Ă�*/
	if (this->frameCount == 0)
	{
		//�g�����X�t�H�[���̐ݒ�
		auto& enemy = Singleton<EnemyManager>::GetInstance();
		VECTOR position = MV1GetFramePosition(enemy.GetModelHandle(), this->useIndexNum);
		position.y = enemy.GetRigidbody().GetPosition().y;
		VECTOR rotation = enemy.GetRigidbody().GetRotation();
		this->transform->SetPosition(position);
		this->transform->SetScale(this->scale);
		this->transform->SetRotationOfRadian(rotation);
	}

	/*�t���[���v��*/
	this->frameCount++;
	//�J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��
	if (this->frameCount < this->startFrame)return;

	/*�G�t�F�N�g�̍X�V*/
	//�Đ��G�t�F�N�g�̃n���h��
	if (this->frameCount == this->startFrame)
	{
		//�Đ�����G�t�F�N�g�̃n���h�����擾
		this->playingEffectHandle = PlayEffekseer3DEffect(this->effectResourceHandle);
		//��]���A�g�嗦�A���W��ݒ�
		SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
		SetScalePlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
		SetPosPlayingEffekseer3DEffect(this->playingEffectHandle, this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
	}
	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//�I���t���[���𒴂��Ă����珉����
	if (this->frameCount >= this->endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = 0;
		StopEffekseer3DEffect(this->playingEffectHandle);
		this->playingEffectHandle = -1;
	}
}