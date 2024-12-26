#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "SoundManager.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundManager::SoundManager()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	this->sound.emplace_back(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1));
	this->sound.emplace_back(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_1));
	this->sound.emplace_back(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_2));
	this->sound.emplace_back(asset.GetSound(LoadingAsset::SoundType::SWORD_SLASH_3));

	for (int i = 0; i < this->sound.size(); i++)
	{
		this->isPlay.emplace_back(false);
	}
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SoundManager::~SoundManager()
{
	this->sound.clear();
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
	
	//for (int i = 0; i < this->sound; i++)
	//{
	//	if (this->isPlay[i])
	//	{
	//		PlaySoundMem(this->sound[i],this->playType[i],this->isTopPosition[i]);
	//	}
	//}
	
	//int endTime = GetNowCount();
	//this->frameTime = endTime - startTime;
}

/// <summary>
/// �`��
/// </summary>
const void SoundManager::Draw()const
{
	//printfDx("EFFEK_FRAMETIME:%d\n", this->frameTime);
}
void SoundManager::OnIsPlay(const EffectType _type, const bool _isTop)
{
	int type = static_cast<int>(_type);
	this->isPlay[type] = true;
	this->isTopPosition[type] = _isTop;
}
