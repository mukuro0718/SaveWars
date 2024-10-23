#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Effect.h"
#include "BossFlameEffect.h"
#include "EnemyManager.h"

BossFlameEffect::BossFlameEffect(const int _effectResourceHandle)
	: Effect(_effectResourceHandle)
{
}

/// <summary>
/// �G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�
/// </summary>
void BossFlameEffect::SetInfoToPlayTheEffect()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_SCALE"]);
	VECTOR rotation = enemy.GetRigidbody().GetRotation();

	this->transform->SetScale(scale);
	this->transform->SetRotation(rotation);

	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_START_FRAME"];
	this->endFrame = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_END_FRAME"];
	
}

/// <summary>
/// �X�V
/// </summary>
void BossFlameEffect::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

		int flameNum = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FLAME_EFFECT_NUM"];
	while(this->playingEffectHandle.size() == flameNum)
	{
		for (int i = 0; i < flameNum; i++)
		{
			this->playingEffectHandle.emplace_back(-1);
		}
	}

	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (!this->isPlayEffect)return;

	/*�G�t�F�N�g�̍Đ��ɕK�v�ȏ��̐ݒ�*/
	SetInfoToPlayTheEffect();

	/*�t���[���v��*/
	//�t���[���̑���
	this->frameCount++;
	//�J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��
	if (this->frameCount < this->startFrame)return;

	/*�G�t�F�N�g�̍X�V*/
	//�Đ��G�t�F�N�g�̃n���h��
	vector<int> frameIndex = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_FRAME_INDEX"];
	for (int i = 0; i < flameNum; i++)
	{
		if (this->frameCount == this->startFrame)
		{
			this->playingEffectHandle[i] = PlayEffekseer3DEffect(this->effectResourceHandle);
			SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
			SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
		}
		// �Đ����̃G�t�F�N�g���ړ�����
		VECTOR position = MV1GetFramePosition(enemy.GetModelHandle(), frameIndex[i]);
		SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[i], position.x, position.y, position.z);
		//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();

		//�I���t���[���𒴂��Ă�����Đ��t���O������
		if (this->frameCount >= this->endFrame)
		{
			this->isPlayEffect = false;
			this->frameCount = this->firstFrame;
			StopEffekseer3DEffect(this->playingEffectHandle[i]);
			this->playingEffectHandle[i] = -1;
		}
	}
}

/// <summary>
/// ���W�̐ݒ�
/// </summary>
void BossFlameEffect::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
}