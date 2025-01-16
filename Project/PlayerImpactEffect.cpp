#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "Effect.h"
#include "PlayerImpactEffect.h"
#include "PlayerManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerImpactEffect::PlayerImpactEffect(const int _effectResourceHandle)
	: Effect		(_effectResourceHandle)
	, MAX_EFFECT_NUM(Singleton<JsonManager>::GetInstance().GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_NUM"])
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->scale		 = Gori::Convert(json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_SCALE"]);
	this->startFrame = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_START_FRAME"];
	this->endFrame	 = json.GetJson(JsonManager::FileType::EFFECT)["PLAYER_IMPACT_END_FRAME"];

	for (int i = 0; i < this->MAX_EFFECT_NUM; i++)
	{
		this->isPlay.emplace_back(false);
		this->playingEffectHandle.emplace_back(-1);
		this->frameCount.emplace_back(0);
	}
}

/// <summary>
/// �X�V
/// </summary>
void PlayerImpactEffect::Update()
{
	/*�Đ��t���O�������Ă��Ȃ���Α������^�[��*/
	if (this->isPlayEffect)
	{
		int count = 0;
		for (count = 0; count < this->MAX_EFFECT_NUM; count++)
		{
			if (!this->isPlay[count])
			{
				this->isPlay[count] = true;
				break;
			}
		}
		this->isPlayEffect = false;
	}

	for (int i = 0; i < this->MAX_EFFECT_NUM; i++)
	{
		if (!this->isPlay[i])continue;
		/*�����̊J�n���Ɉ�x�����Ă�*/
		if (this->frameCount[i] == 0)
		{
			//�g�����X�t�H�[���̐ݒ�
			auto& player = Singleton<PlayerManager>::GetInstance();
			VECTOR rotation = player.GetRigidbody().GetRotation();
			this->transform->SetScale(this->scale);
			this->transform->SetRotationOfRadian(rotation);
		}

		/*�t���[���v��*/
		this->frameCount[i]++;
		//�J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��
		if (this->frameCount[i] < this->startFrame)return;

		/*�G�t�F�N�g�̍X�V*/
		//�Đ��G�t�F�N�g�̃n���h��
		if (this->frameCount[i] == this->startFrame)
		{
			if (this->isPlay[i])
			{
				//�Đ�����G�t�F�N�g�̃n���h�����擾
				this->playingEffectHandle[i] = PlayEffekseer3DEffect(this->effectResourceHandle);
				//��]���A�g�嗦�A���W��ݒ�
				SetRotationPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetRotation().x, this->transform->GetRotation().y, this->transform->GetRotation().z);
				SetScalePlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetScale().x, this->transform->GetScale().y, this->transform->GetScale().z);
				SetPosPlayingEffekseer3DEffect(this->playingEffectHandle[i], this->transform->GetPosition().x, this->transform->GetPosition().y, this->transform->GetPosition().z);
			}
		}
		//Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();

		//�I���t���[���𒴂��Ă����珉����
		if (this->frameCount[i] >= this->endFrame)
		{
			this->isPlayEffect = false;
			this->frameCount[i] = 0;
			this->isPlay[i] = false;
			StopEffekseer3DEffect(this->playingEffectHandle[i]);
			this->playingEffectHandle[i] = -1;
		}
	}
}

/// <summary>
/// �`��
/// </summary>
const void PlayerImpactEffect::Draw()const
{
	for (int i = 0; i < this->MAX_EFFECT_NUM; i++)
	{
		/*�Đ��t���O�������Ă��Ȃ��܂��͊J�n�t���[���𒴂��Ă��Ȃ���Α������^�[��*/
		if (!this->isPlay[i] || this->frameCount[i] < this->startFrame)return;
		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer3D();
	}
}
