#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "UseSTL.h"
#include "UseJson.h"
#include "Effect.h"
#include "BossImpactEffect.h"
#include "BossRoarEffect.h"
#include "PlayerGuardHitEffect.h"
#include "PlayerHealEffect.h"
#include "PlayerImpactEffect.h"
#include "PlayerChargeAttackEffect.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "PlayerManager.h"
#include "LoadingAsset.h"
#include "DeleteInstance.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectManager::EffectManager()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& asset = Singleton<LoadingAsset>::GetInstance();
	auto& json = Singleton<JsonManager>::GetInstance();

	this->effect.emplace_back(new BossImpactEffect		(asset.GetEffect(LoadingAsset::EffectType::BOSS_IMPACT)));
	this->effect.emplace_back(new BossRoarEffect		(asset.GetEffect(LoadingAsset::EffectType::BOSS_ROAR)));
	this->effect.emplace_back(new PlayerGuardHitEffect	(asset.GetEffect(LoadingAsset::EffectType::PLAYER_GUARD_HIT)));
	this->effect.emplace_back(new PlayerHealEffect		(asset.GetEffect(LoadingAsset::EffectType::TMP_EFFECT)));
	this->effect.emplace_back(new PlayerImpactEffect	(asset.GetEffect(LoadingAsset::EffectType::PLAYER_IMPACT)));
	this->effect.emplace_back(new PlayerChargeAttackEffect(asset.GetEffect(LoadingAsset::EffectType::PLAYER_CHARGE_ATTACK)));
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectManager::~EffectManager()
{
	for (int i = 0; i < this->effect.size(); i++)
	{
		DeleteMemberInstance(this->effect[i]);
	}
	this->effect.clear();
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
	//this->bossFlame->Update();
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