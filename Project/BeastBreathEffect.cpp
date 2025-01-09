#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BeastBreathEffect.h"
#include "Character.h"
#include "Enemy.h"
#include "EnemyManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BeastBreathEffect::BeastBreathEffect(const int _effectResourceHandle)
	: Effect		(_effectResourceHandle)
	, useFrameIndex1(-1)
	, useFrameIndex2(-1)
	, useEffectNum	(0)
	, maxEffectNum	(0)
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->scale			 = Gori::Convert(json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_SCALE"]);
	this->startFrame	 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_START_FRAME"];
	this->endFrame		 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_END_FRAME"];
	this->useFrameIndex1 = json.GetJson(JsonManager::FileType::BEAST)["BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][0];
	this->useFrameIndex2 = json.GetJson(JsonManager::FileType::BEAST)["BREATH_FRAME_INDEX_USED_CAPSULE_DIRECTION"][1];
	this->maxEffectNum	 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_MAX_PLAY_NUM"];
	this->moveSpeed		 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_SPEED"];
	this->maxDistance	 = json.GetJson(JsonManager::FileType::EFFECT)["BEAST_BREATH_MAX_DISTANCE"];
	for (int i = 0; i < this->maxEffectNum; i++)
	{
		this->playingEffectHandle.emplace_back(-1);
		this->position.emplace_back(Gori::ORIGIN);
		this->velocity.emplace_back(Gori::ORIGIN);
		this->isPlay.emplace_back(false);
	}
}

/// <summary>
/// �X�V
/// </summary>
void BeastBreathEffect::Update()
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect)return;

	/*�����̊J�n���Ɉ�x�����Ă�*/
	if (this->frameCount == 0)
	{
		//�X�P�[���̐ݒ�
		this->transform->SetScale(this->scale);
	}

	/*�t���[���v��*/
	this->frameCount++;
	//�J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��
	if (this->frameCount < this->startFrame)return;

	auto& enemy = Singleton<EnemyManager>::GetInstance();
	VECTOR facePosition = MV1GetFramePosition(enemy.GetModelHandle(), this->useFrameIndex1);
	/*�G�t�F�N�g�̍X�V*/
	//�Đ��G�t�F�N�g�̃n���h��
	if (this->frameCount == this->startFrame || this->frameCount % 3 == 0)
	{
		if (this->useEffectNum != this->maxEffectNum - 1)
		{
			//�Đ�����G�t�F�N�g�̃n���h�����擾
			this->playingEffectHandle[this->useEffectNum] = PlayEffekseer3DEffect(this->effectResourceHandle);
			//��]���A�g�嗦�A���W��ݒ�
			SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[this->useEffectNum], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
			VECTOR nosePosition = MV1GetFramePosition(enemy.GetModelHandle(), this->useFrameIndex2);
			VECTOR direction = VSub(nosePosition, facePosition);
			direction.y = 0.0f;
			this->velocity[this->useEffectNum] = VScale(VNorm(direction), this->moveSpeed);
			this->position[this->useEffectNum] = facePosition;
			this->isPlay[this->useEffectNum] = true;
			this->useEffectNum++;
		}
	}
	//�G�t�F�N�g�̈ړ�
	for (int i = 0; i < this->maxEffectNum; i++)
	{
		if (this->isPlay[i])
		{
			//�����ʒu����ǂꂾ������Ă��邩�����߂�
			this->position[i] = VAdd(this->position[i], this->velocity[i]);
			float distance = VSquareSize(VSub(this->position[i], facePosition));
			//�ő勗���𒴂��Ă����珉���ʒu�ɖ߂�
			if (distance >= this->maxDistance)
			{
				this->position[i] = facePosition;
			}
			SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->position[i].x, this->position[i].y, this->position[i].z);
		}
	}
	//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
	UpdateEffekseer3D();

	//�I���t���[���𒴂��Ă����珉����
	if (this->frameCount >= this->endFrame)
	{
		this->isPlayEffect = false;
		this->frameCount = 0;
		this->useEffectNum = 0;
		for (int i = 0; i < this->maxEffectNum; i++)
		{
			StopEffekseer3DEffect(this->playingEffectHandle[i]);
			this->playingEffectHandle[i] = -1;
			this->isPlay[i] = false;
		}
	}
}