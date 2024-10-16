#include <cassert>
#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "DeleteInstance.h"
#include "ReactionType.h"
#include "Rigidbody.h"
#include "ColliderData.h"
#include "AttackData.h"
#include "BossAttackData.h"
#include "AttackCapsuleColliderData.h"
#include "BossAttack.h"
#include "BossMeleeAttack.h"
#include "EnemyManager.h"
#include "Debug.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossMeleeAttack::BossMeleeAttack(const int _attackIndex)
	: BossAttack()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*この攻撃がボスの何番目の攻撃かを保存しておく*/
	this->attackIndex = _attackIndex;

	/*コライダーデータの作成*/
	AttackData* data = new BossAttackData();
	this->collider = new AttackCapsuleColliderData(ColliderData::Priority::STATIC, GameObjectTag::BOSS_ATTACK, data);
}

/// <summary>
/// デストラクタ
/// </summary>
BossMeleeAttack::~BossMeleeAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossMeleeAttack::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossAttackData&>(*collider.data);
	collider.radius		= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	data.damage			= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	data.playerReaction = static_cast<int>(Gori::PlayerReactionType::NORMAL);
	//ここでのヒットストップ系の変数は、キャラクター側に与えるものになる
	data.hitStopTime	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	data.hitStopType	= static_cast<int>(HitStop::Type::STOP);
	data.hitStopDelay	= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	data.slowFactor		= json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];
	data.isHitAttack	= false;

	/*変数の初期化*/
	this->frameCount	  = 0;
	this->hitCheckCount	  = 0;
	this->isStartHitCheck = false;
	this->isStartHitCheck = false;
	this->isNotOnHit	  = false;

	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// 更新
/// </summary>
void BossMeleeAttack::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json   = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossAttackData&>(*collider.data);

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//変数の準備
		const int	START_HIT_CHECK_FRAME = json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"][this->attackIndex];
		const int	END_HIT_CHECK_FRAME	  = json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"]	[this->attackIndex];
		const float POSITION_OFFSET		  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"]		[this->attackIndex];
		const float Y_OFFSET			  = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"]		[this->attackIndex];

		//フレームを増やす
		this->frameCount++;
		//フレームが定数を超えていなかったら早期リターン
		if (this->frameCount < START_HIT_CHECK_FRAME)return;

		//今回の攻撃中に当たり判定フラグが一度もたっていなかったら
		if (!this->isNotOnHit)
		{
			this->hitCheckCount = json.GetJson(JsonManager::FileType::ENEMY)["HURRICANE_KICK_HIT_COUNT"];
			data.isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//当たり判定位置の更新
		collider.rigidbody.SetPosition(MV1GetFramePosition(enemy.GetModelHandle(), 14));
		collider.topPositon = MV1GetFramePosition(enemy.GetModelHandle(), 19);
		//フレームが定数を超えている、当たり判定フラグが降りていたら当たり判定開始フラグを下す
		if (this->frameCount > END_HIT_CHECK_FRAME)
		{
			this->isStartHitCheck = false;
			data.isDoHitCheck = false;
			this->frameCount = 0;
			data.isHitAttack = false;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void BossMeleeAttack::Draw()const
{
	auto& debug = Singleton<Debug>::GetInstance();
	//if (debug.CheckEnemyFlag())
	//{
		if (this->isStartHitCheck)
		{
			auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
			auto& data = dynamic_cast<BossAttackData&>(*collider.data);

			DrawCapsule3D(collider.rigidbody.GetPosition(), collider.topPositon, collider.radius, 16, GetColor(100, 100, 150), GetColor(100, 100, 150), FALSE);
		}
		VECTOR position = this->collider->rigidbody.GetPosition();
		printfDx("HURRICANE_KICK X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	//}
}