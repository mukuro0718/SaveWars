#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "Sound.h"
#include "Bgm.h"
#include "SoundEffect.h"
#include "SoundManager.h"
#include "LoadingAsset.h"
#include "DeleteInstance.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundManager::SoundManager()
{
	/*�T�E���h�N���X�̍쐬*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	this->bgms.emplace_back(new Bgm(asset.GetSound(LoadingAsset::SoundType::TITLE_BGM), json.GetJson(JsonManager::FileType::SOUND)["TITLE_BGM_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CONFIRMATION), json.GetJson(JsonManager::FileType::SOUND)["CONFIRMATION_SOUND_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CANCEL), json.GetJson(JsonManager::FileType::SOUND)["CANCEL_SOUND_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::CHANGE_SELECT_ICON), json.GetJson(JsonManager::FileType::SOUND)["CHANGE_SELECT_ICON_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_1_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_1_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_2), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_2_VOLUME"]));
	this->effects.emplace_back(new SoundEffect(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_3), json.GetJson(JsonManager::FileType::SOUND)["SWORD_SLASH_3_VOLUME"]));
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundManager::~SoundManager()
{
	/*�����o�C���X�^���X�̊J��*/
	for (auto& bgm : this->bgms)
	{
		DeleteMemberInstance(bgm);
	}
	for (auto& effect : this->effects)
	{
		DeleteMemberInstance(effect);
	}
	this->effects.clear();
	this->bgms.clear();
}

/// <summary>
/// �X�V
/// </summary>
void SoundManager::Initialize()
{
}

/// <summary>
/// �X�V
/// </summary>
void SoundManager::Update()
{
	//int startTime = GetNowCount();
	
	/*BGM�̍X�V*/
	for (auto& bgm : this->bgms)
	{
		bgm->Update();
	}

	/*���ʉ��i�T�E���h�G�t�F�N�g�j�̍X�V*/
	for (auto& effect : this->effects)
	{
		effect->Update();
	}

	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;


	/*HACK:StopSoundMem(soundHandle)�����t�@�C���̒�~*/
	/*HACK:SetVolumeoundMem(0~255,soundHandle)���ʂ̒���*/
	/*HACK:GetCurrentSoundMem(soundHandle)�Đ��ʒu�̎擾�i�����ł����Đ��ʒu�͉����t�@�C���̂ǂ̈ʒu���j*/
	/*HACK:SetCurrentSoundMem(second(1s==1000),soundHandle)�Đ��ʒu��ݒ�*/
}

/// <summary>
/// �`��
/// </summary>
const void SoundManager::Draw()const
{
	//printfDx("EFFEK_FRAMETIME:%d\n", this->frameTime);
}

/// <summary>
/// �G�t�F�N�g�Đ��t���O�𗧂Ă�
/// </summary>
void SoundManager::OnIsPlayBgm(const BgmType _type)
{
	int type = static_cast<int>(_type);
	this->bgms[type]->OnIsPlay();
}
void SoundManager::OnIsPlayEffect(const EffectType _type)
{
	int type = static_cast<int>(_type);
	this->effects[type]->OnIsPlay();
}

/// <summary>
/// �G�t�F�N�g�Đ��t���O������
/// </summary>
void SoundManager::OffIsPlayBgm(const BgmType _type)
{
	int type = static_cast<int>(_type);
	this->bgms[type]->OffIsPlay();
}
void SoundManager::OffIsPlayEffect(const EffectType _type)
{
	int type = static_cast<int>(_type);
	this->effects[type]->OffIsPlay();
}