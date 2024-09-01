#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "GoriLib.h"
#include <vector>
#include "UseJson.h"
#include "Effect.h"
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

	this->boss.emplace_back(new Effect(asset.GetEffect(LoadingAsset::EffectType::BOSS_PUNCH)));
	this->boss.emplace_back(new Effect(asset.GetEffect(LoadingAsset::EffectType::BOSS_SLASH)));
	this->boss.emplace_back(new Effect(asset.GetEffect(LoadingAsset::EffectType::BOSS_THROW)));
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
void EffectManager::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& bossAttack = Singleton<BossAttackManager>::GetInstance();

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
	{
		VECTOR position = enemy.GetPosition();
		position.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_Y_OFFSET"];
		VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_SCALE"]);
		VECTOR rotation = enemy.GetRotation();
		const int START_FRAME = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_START_FRAME"];
		const int END_FRAME = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_PUNCH_END_FRAME"];
		this->boss[static_cast<int>(BossEffect::PUNCH)]->Update(START_FRAME, END_FRAME, position, rotation, scale);
	}
	{
		/*�g�p����l�̏���*/
		VECTOR position = enemy.GetPosition();
		VECTOR rotation = enemy.GetRotation();
		VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_SCALE"]);
		position.y += json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_Y_OFFSET"];
		const int START_FRAME = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_START_FRAME"];
		const int END_FRAME = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_SLASH_END_FRAME"];
		this->boss[static_cast<int>(BossEffect::SLASH)]->Update(START_FRAME, END_FRAME, position, rotation, scale);
	}
	{
		/*�g�p����l�̏���*/
		VECTOR position = bossAttack.GetThrowPosition();
		if (position.y >= 500.0f)
		{
			position = MV1GetFramePosition(enemy.GetModelHandle(), 15);
		}
		VECTOR rotation = { 0.0f,0.0f,0.0f };		 //��]��
		VECTOR scale = Convert(json.GetJson(JsonManager::FileType::EFFECT)["BOSS_THROW_SCALE"]);
		const int START_FRAME = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_THROW_START_FRAME"];
		const int END_FRAME = json.GetJson(JsonManager::FileType::EFFECT)["BOSS_THROW_END_FRAME"];
		this->boss[static_cast<int>(BossEffect::THROW)]->Update(START_FRAME, END_FRAME, position, rotation,scale);
	}
}
/// <summary>
/// �`��
/// </summary>
const void EffectManager::Draw()const
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();
	for (int i = 0; i < json.GetJson(JsonManager::FileType::EFFECT)["BOSS_EFFECT_NUM"]; i++)
	{
		this->boss[i]->Draw();
	}
}
void EffectManager::OnIsBossPunchEffect()
{
	this->boss[static_cast<int>(BossEffect::PUNCH)]->OnIsPlayEffect();
}
void EffectManager::OnIsBossSlashEffect()
{
	this->boss[static_cast<int>(BossEffect::SLASH)]->OnIsPlayEffect();
}
void EffectManager::OnIsBossThrowEffect()
{
	this->boss[static_cast<int>(BossEffect::THROW)]->OnIsPlayEffect();
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
