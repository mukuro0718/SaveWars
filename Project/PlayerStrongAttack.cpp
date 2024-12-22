#include <DxLib.h>
#include "UseJson.h"
#include "UseSTL.h"
#include "Rigidbody.h"
#include "Character.h"
#include "Player.h"
#include "UseSTL.h"
#include "VECTORtoUseful.h"
#include "PlayerAction.h"
#include "PlayerStrongAttack.h"
#include "PlayerAttackManager.h"
#include "InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStrongAttack::PlayerStrongAttack()
	: PlayerAction()
{

}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStrongAttack::~PlayerStrongAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerStrongAttack::Initialize()
{
	this->isChangeAction = false;
	this->isEndAction = false;
	this->isPlay = false;
	this->isCharge = true;
	this->frameCount = 0;
}

/// <summary>
/// 後処理
/// </summary>
void PlayerStrongAttack::Finalize()
{

}

/// <summary>
/// 更新
/// </summary>
void PlayerStrongAttack::Update(Player& _player)
{
	/*開始時に一度だけ呼ばれる*/
	auto& json = Singleton<JsonManager>::GetInstance();
	if (this->frameCount == 0)
	{
		auto& attack = Singleton<PlayerAttackManager>::GetInstance();
		attack.OnIsStart();
		attack.SetDamage(json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_DAMAGE"]);
	}

	/*移動処理（移動をしない場合でも、速度の減速が入るので処理を行う）*/
	MoveData data;
	data.Set(_player.GetNextRotation(), 0.0f, true, false);
	Move(_player, data);

	/*アニメーションの再生*/
	int nextAnimation = static_cast<int>(Player::AnimationType::SKILL);
	float playTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][nextAnimation];
	if (this->frameCount == 0) playTime = json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_START_ANIM_PLAY_TIME"];
	_player.PlayAnimation(nextAnimation, playTime);

	this->frameCount++;
	if (this->frameCount >= json.GetJson(JsonManager::FileType::PLAYER)["S_ATTACK_CANCEL_FRAME"])
	{
		this->isChangeAction = true;
	}

	/*アニメーションが終了していたら*/
	if (_player.GetIsChangeAnimation())
	{
		this->isEndAction = true;
	}
}