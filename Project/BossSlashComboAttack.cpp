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
#include "BossSlashComboAttack.h"
#include "EnemyManager.h"
#include "Debug.h"
#include "HitStop.h"

/// <summary>
/// コンストラクタ
/// </summary>
BossSlashComboAttack::BossSlashComboAttack(const int _attackIndex)
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
BossSlashComboAttack::~BossSlashComboAttack()
{

}

/// <summary>
/// 初期化
/// </summary>
void BossSlashComboAttack::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*コライダーの初期化*/
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossAttackData&>(*collider.data);
	collider.radius = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_RADIUS"][this->attackIndex];
	data.damage = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_DAMAGE"][this->attackIndex];
	data.playerReaction = static_cast<int>(Gori::PlayerReactionType::NORMAL);
	//ここでのヒットストップ系の変数は、キャラクター側に与えるものになる
	data.hitStopTime = json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_TIME"][this->attackIndex];
	data.hitStopType = static_cast<int>(HitStop::Type::STOP);
	data.hitStopDelay = json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_HIT_STOP_DELAY"][this->attackIndex];
	data.slowFactor = json.GetJson(JsonManager::FileType::ENEMY)["DEFENSE_SLOW_FACTOR"][this->attackIndex];
	data.isHitAttack = false;

	/*変数の初期化*/
	this->frameCount			= 0;
	this->isStartHitCheck		= false;
	this->isStartHitCheck		= false;
	this->isNotOnHit			= false;

	/*物理挙動の初期化*/
	this->collider->rigidbody.Initialize(false);
}
/// <summary>
/// 更新
/// </summary>
void BossSlashComboAttack::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	auto& collider = dynamic_cast<AttackCapsuleColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossAttackData&>(*collider.data);

	/*当たり判定の確認が開始している*/
	if (this->isStartHitCheck)
	{
		//変数の準備
		const int	START_HIT_CHECK_FRAME	= json.GetJson(JsonManager::FileType::ENEMY)["START_HIT_CHECK_FRAME"][this->attackIndex];
		const int	END_HIT_CHECK_FRAME		= json.GetJson(JsonManager::FileType::ENEMY)["END_HIT_CHECK_FRAME"][this->attackIndex];
		const float POSITION_OFFSET			= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET"][this->attackIndex];
		const float Y_OFFSET				= json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_OFFSET_Y"][this->attackIndex];

		//フレームを増やす
		this->frameCount++;
		//フレームが定数を超えていなかったら早期リターン
		if (this->frameCount < START_HIT_CHECK_FRAME)return;

		if (!this->isNotOnHit)
		{
			data.isDoHitCheck = true;
			this->isNotOnHit = true;
		}

		//35-40 57-62
		int onDoHitFrame = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_1_ON_DO_HIT_FRAME"];
		int offDoHitFrame = json.GetJson(JsonManager::FileType::ENEMY)["SLASH_COMBO_1_OFF_DO_HIT_FRAME"];
		//指定フレームの時に、当たり判定フラグをもう一度立てる
		if (this->frameCount == onDoHitFrame)
		{
			data.isDoHitCheck = true;
		}
		//指定フレームの時に当たり判定フラグを下す
		else if (this->frameCount == onDoHitFrame)
		{
			data.isDoHitCheck = false;
		}

		//ひじの座標
		VECTOR elbowPosition = MV1GetFramePosition(enemy.GetModelHandle(), 9);
		//手の一番外の座標
		VECTOR topPositionBase = MV1GetFramePosition(enemy.GetModelHandle(), 11);
		//ひじから手へ伸びるベクトル
		VECTOR underToTopBaseVector = VNorm(VSub(topPositionBase, elbowPosition));
		//ひじから手へ伸びるベクトルを定数でスケーリングしたものをひじの座標に足したものを爪の先端座標とする
		VECTOR crowTopPosition = VScale(underToTopBaseVector, json.GetJson(JsonManager::FileType::ENEMY)["CROW_SIZE"]);
		crowTopPosition = VAdd(crowTopPosition, elbowPosition);
		//ひじの座標をカプセル下座標とする
		collider.rigidbody.SetPosition(elbowPosition);
		//爪先の座標をカプセル上座標とする
		collider.topPositon = crowTopPosition;

		//フレームが定数を超えている、当たり判定フラグが降りていたら当たり判定開始フラグを下す
		if (this->frameCount > END_HIT_CHECK_FRAME)
		{
			this->isStartHitCheck		= false;
			data.isDoHitCheck			= false;
			this->frameCount			= 0;
			data.isHitAttack			= false;
		}
	}
}

/// <summary>
/// 描画
/// </summary>
const void BossSlashComboAttack::Draw()const
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