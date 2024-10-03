#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include <vector>
#include "UseJson.h"
#include "Effect.h"
#include "BossSlashEffect.h"
#include "BossRotatePunchEffect.h"
#include "BossEntryEffect.h"
#include "BossImpactEffect.h"
#include "PlayerEntryEffect.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "BossAttackManager.h"
#include "PlayerManager.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectManager::EffectManager()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	this->effect.emplace_back(new BossSlashEffect		 (asset.GetEffect(LoadingAsset::EffectType::BOSS_SLASH)));
	this->effect.emplace_back(new BossRotatePunchEffect(asset.GetEffect(LoadingAsset::EffectType::BOSS_ROTATE_PUNCH)));
	this->effect.emplace_back(new BossEntryEffect		 (asset.GetEffect(LoadingAsset::EffectType::BOSS_ENTRY)));
	this->effect.emplace_back(new BossImpactEffect		 (asset.GetEffect(LoadingAsset::EffectType::BOSS_IMPACT)));
	this->effect.emplace_back(new PlayerEntryEffect	 (asset.GetEffect(LoadingAsset::EffectType::PLAYER_ENTRY)));
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectManager::~EffectManager()
{

}

/// <summary>
/// �X�V
/// </summary>
void EffectManager::Initialize()
{
	for (int i = 0; i < this->effect.size(); i++)
	{
		this->effect[i]->Initialize();
	}
}

/// <summary>
/// �X�V
/// </summary>
void EffectManager::Update()
{
	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
	for (int i = 0; i < this->effect.size(); i++)
	{
		this->effect[i]->Update();
	}
}
/// <summary>
/// �`��
/// </summary>
const void EffectManager::Draw()const
{
	for (int i = 0; i < this->effect.size(); i++)
	{
		this->effect[i]->Draw();
	}
}
void EffectManager::OnIsEffect(const EffectType _type)
{
	this->effect[static_cast<int>(_type)]->OnIsPlayEffect();
}
void EffectManager::SetPosition(const EffectType _type, const VECTOR _position)
{
	this->effect[static_cast<int>(_type)]->SetPosition(_position);
}
/// <summary>
/// vector<float>��VECTOR�ɕϊ�
/// </summary>
const VECTOR EffectManager::Convert(std::vector<float> _in)const
{
	VECTOR out = VGet(0.0f, 0.0f, 0.0f);
	out.x = _in[0];
	out.y = _in[1];
	out.z = _in[2];
	return out;
}
