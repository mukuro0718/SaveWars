#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "VECTORtoUseful.h"
#include "DeleteInstance.h"

#include "Rigidbody.h"

#include "CharacterData.h"
#include "BossData.h"
#include "ColliderData.h"
#include "CharacterColliderData.h"

#include "BitFlag.h"
#include "Animation.h"
#include "Character.h"

#include "BossActionHeader.h"

#include "Boss.h"

#include "LoadingAsset.h"
#include "PlayerManager.h"
#include "CameraManager.h"
#include "BossAttackManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Boss::Boss()
	: animationPlayTime(0.0f)
	, moveTarget			{ 0.0f, 0.0f, 0.0f }
	, attackComboCount		(0)
	, attackComboIndexOffset(0)
	, nowAnimation			(0)
	, attackType			(0)
	, nowPhase				(0)
	, prevPhase				(-1)
	, actionType(0)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json  = Singleton<JsonManager>::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*メンバクラスのインスタンスの作成*/
	this->modelHandle = MV1DuplicateModel(asset.GetModel(LoadingAsset::ModelType::ENEMY));

	/*vectorの追加*/
	for (int i = 0; i < this->COUNT_NUM; i++)
	{
		this->frameCount.emplace_back(0);
		this->isCount.emplace_back(false);
	}

	/*アニメーションの設定*/
	vector<string> animationHandle = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_HANDLE"];
	vector<int> animationIndex = json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_INDEX"];
	this->nowAnimation = static_cast<int>(AnimationType::IDLE);
	this->animationPlayTime = json.GetJson(JsonManager::FileType::PLAYER)["ANIMATION_PLAY_TIME"][this->nowAnimation];
	//アニメーションの追加
	for (int i = 0; i < animationHandle.size(); i++)
	{
		this->animation->Add(MV1LoadModel(animationHandle[i].c_str()), animationIndex[i]);
	}
	//アニメーションのアタッチ
	this->animation->Attach(&this->modelHandle);

	/*アニメーションマップの設定*/
	this->stateAnimationMap.emplace(this->DYING, static_cast<int>(AnimationType::DYING));
	this->stateAnimationMap.emplace(this->IDLE, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->ROAR, static_cast<int>(AnimationType::ROAR));
	this->stateAnimationMap.emplace(this->WALK, static_cast<int>(AnimationType::WALK));
	this->stateAnimationMap.emplace(this->REST, static_cast<int>(AnimationType::IDLE));
	this->stateAnimationMap.emplace(this->SLASH, static_cast<int>(AnimationType::SLASH));
	this->stateAnimationMap.emplace(this->FLY_ATTACK, static_cast<int>(AnimationType::FLY_ATTACK));
	this->stateAnimationMap.emplace(this->HURRICANE_KICK, static_cast<int>(AnimationType::HURRICANE_KICK));
	this->stateAnimationMap.emplace(this->JUMP_ATTACK, static_cast<int>(AnimationType::JUMP_ATTACK));
	this->stateAnimationMap.emplace(this->ROTATE_PUNCH, static_cast<int>(AnimationType::ROTATE_PUNCH));
	/*アクションマップの作成*/
	this->actionTypeMap.emplace(static_cast<int>(ActionType::DYING),this->DYING);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::IDLE),this->IDLE);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROAR),this->ROAR);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::WALK),this->WALK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::REST),this->REST);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::SLASH),this->SLASH);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::FLY_ATTACK),this->FLY_ATTACK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::HURRICANE_KICK),this->HURRICANE_KICK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::JUMP_ATTACK),this->JUMP_ATTACK);
	this->actionTypeMap.emplace(static_cast<int>(ActionType::ROTATE_PUNCH),this->ROTATE_PUNCH);

	/*コライダーデータの作成*/
	CharacterData* data = new BossData();
	this->collider = new CharacterColliderData(ColliderData::Priority::HIGH, GameObjectTag::BOSS, data);

	/*アクションパラメーターの作成*/
	this->parameters.emplace_back(new BossDeathAction());
	this->parameters.emplace_back(new BossIdleAction());
	this->parameters.emplace_back(new BossRoarAction());
	this->parameters.emplace_back(new BossChaseAction());
	this->parameters.emplace_back(new BossRestAction());
	this->parameters.emplace_back(new BossSlashAction());
	this->parameters.emplace_back(new BossFlyAction());
	this->parameters.emplace_back(new BossHurricaneKickAction());
	this->parameters.emplace_back(new BossStabAction());
	this->parameters.emplace_back(new BossRotatePunchAction());
}

/// <summary>
/// デストラクタ
/// </summary>
Boss::~Boss()
{
	this->stateAnimationMap.clear();
	this->frameCount.clear();
	this->isCount.clear();
	this->attackCombo.clear();
}

void Boss::Initialize()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);


	/*変数の初期化*/
	collider.radius = json.GetJson(JsonManager::FileType::ENEMY)["HIT_RADIUS"];
	float height = json.GetJson(JsonManager::FileType::ENEMY)["HIT_HEIGHT"];
	collider.topPositon = VAdd(collider.rigidbody.GetPosition(), VGet(0.0f, height, 0.0f));
	data.hp			= json.GetJson(JsonManager::FileType::ENEMY)["HP"];
	data.isHit		= false;

	/*フェーズの設定*/
	SetPhase();

	//this->prevPhase = this->nowPhase;

	/*攻撃コンボの初期化*/
	this->attackComboCount = -1;

	/*スピードの初期化*/
	this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	this->speed = 0.0f;

	/*物理挙動の初期化*/
	//jsonデータを定数に代入
	const VECTOR POSITION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_POSITION"]);//座標
	const VECTOR ROTATION = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_ROTATION"]);//回転率
	const VECTOR SCALE = Convert(json.GetJson(JsonManager::FileType::ENEMY)["INIT_SCALE"]);	 //拡大率
	//初期化
	this->collider->rigidbody.Initialize(true);
	this->collider->rigidbody.SetPosition(POSITION);
	this->collider->rigidbody.SetRotation(ROTATION);
	this->collider->rigidbody.SetScale(SCALE);

	/*アニメーションのアタッチ*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// 後処理
/// </summary>
void Boss::Finalize()
{
}

/// <summary>
/// 更新
/// </summary>
void Boss::Update()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	/*フェーズの初期化*/
	SetPhase();

	/*ここですべてのパラメータの計算を行う*/
	for (auto& item : this->parameters)
	{
		item->CalcParameter(*this);
	}

	/*状態の切り替え*/
	ChangeState();

	//ここに各アクションごとの更新処理を入れたい
	this->parameters[this->actionType]->Update(*this);

	/*アニメーションの更新*/
	unsigned int nowState = this->state->GetFlag();
	this->nowAnimation = this->stateAnimationMap[nowState];
	
	//アニメーションの再生
	VECTOR position = this->collider->rigidbody.GetPosition();
	this->animation->Play(&this->modelHandle, position, this->nowAnimation, this->animationPlayTime);
	this->collider->rigidbody.SetPosition(position);
}

/// <summary>
/// 咆哮
/// </summary>
void Boss::Roar()
{
	/*咆哮中にアニメーションが終了していたらフラグを下す*/
	//if (this->state->CheckFlag(this->ROAR) && this->animation->GetIsChangeAnim())
	//{
	//	this->state->ClearFlag(this->ROAR);
	//	this->prevPhase = this->nowPhase;
	//}

	///*咆哮できるか*/
	//CanRoar();

	//if (this->nowPhase != this->prevPhase)
	//{
	//	this->state->ClearFlag(this->MASK_ALL);
	//	this->state->SetFlag(this->ROAR);
	//}
}

/// <summary>
/// 移動
/// </summary>
void Boss::Move()
{
	/*回転率を出す*/
	UpdateRotation();

	/*移動速度の更新*/
	UpdateSpeed();

	/*移動ベクトルを出す*/
	UpdateMoveVector();
}

/// <summary>
/// ヒットリアクション
/// </summary>
void Boss::Reaction()
{

}
/// <summary>
/// 休憩
/// </summary>
void Boss::Rest()
{

}
/// <summary>
/// 死亡
/// </summary>
void Boss::Death()
{

}
/// <summary>
/// 攻撃
/// </summary>
void Boss::Attack()
{
	
}
void Boss::ChangeState()
{
	/*選択されているか調べる*/
	bool isSelect		= false;//選択されているか
	int  sumDesireValue = 0;	//欲求値の合計
	for (auto& item : this->parameters)
	{
		isSelect		= item->GetIsSelect();
		sumDesireValue += item->GetDesireValue();
		if (isSelect)return;
	}
	
	/*今立っているフラグを下す*/
	unsigned int clearFlag = this->actionTypeMap[this->actionType];
	this->state->ClearFlag(clearFlag);

	/*選択されていなかったら*/
	if (!isSelect)
	{
		//各行動の期待値を求める
		std::vector<int> actionWeight;//重み
		for (auto& item : this->parameters)
		{
			actionWeight.emplace_back(item->GetWeight(sumDesireValue));
		}
		//重みをランダムで出す
		int randomWeight = GetRand(this->parameters[0]->GetBaseWeight() - 1);
		//forでvectorを回し、重みが０以下になったところのアクションを行う
		for (int i = 0; i < actionWeight.size(); i++)
		{
			randomWeight -= actionWeight[i];
			if (randomWeight < 0)
			{
				this->actionType = i;
				this->parameters[i]->OnIsSelect();
				break;
			}
		}
	}

	unsigned int setFlag = this->actionTypeMap[this->actionType];
	this->state->SetFlag(setFlag);
}

/// <summary>
/// 描画
/// </summary>
const void Boss::DrawCharacterInfo()const
{
	VECTOR position = this->collider->rigidbody.GetPosition();
	VECTOR rotation = this->collider->rigidbody.GetRotation();
	printfDx("Boss_POSITION X:%f,Y:%f,Z:%f\n", position.x, position.y, position.z);
	printfDx("Boss_ROTATION X:%f,Y:%f,Z:%f\n", rotation.x, rotation.y, rotation.z);
	printfDx("%d:DYING					\n", this->state->CheckFlag(this->DYING));
	printfDx("%d:IDLE					\n", this->state->CheckFlag(this->IDLE));
	printfDx("%d:ROAR					\n", this->state->CheckFlag(this->ROAR));
	printfDx("%d:WALK					\n", this->state->CheckFlag(this->WALK));
	printfDx("%d:REST					\n", this->state->CheckFlag(this->REST));
	printfDx("%d:SLASH					\n", this->state->CheckFlag(this->SLASH));
	printfDx("%d:FLY_ATTACK				\n", this->state->CheckFlag(this->FLY_ATTACK));
	printfDx("%d:HURRICANE_KICK			\n", this->state->CheckFlag(this->HURRICANE_KICK));
	printfDx("%d:JUMP_ATTACK			\n", this->state->CheckFlag(this->JUMP_ATTACK));
	printfDx("%d:ROTATE_PUNCH			\n", this->state->CheckFlag(this->ROTATE_PUNCH));
}

/// <summary>
/// 速度の更新
/// </summary>
void Boss::UpdateMoveVector()
{
	/*移動ベクトルを初期化する*/
	VECTOR direction = { 0.0f,0.0f,0.0f };
	VECTOR rotation = this->collider->rigidbody.GetRotation();

	/*回転率をもとに移動ベクトルを出す*/
	direction = VGet(-sinf(rotation.y), 0.0f, -cosf(rotation.y));

	/*移動ベクトルを正規化*/
	direction = VNorm(direction);

	VECTOR aimVelocity = VScale(direction, this->speed);
	VECTOR prevVelocity = this->collider->rigidbody.GetVelocity();
	VECTOR newVelocity = VGet(aimVelocity.x, prevVelocity.y, aimVelocity.z);

	this->collider->rigidbody.SetVelocity(newVelocity);
}
/// <summary>
/// 速度の更新
/// </summary>
void Boss::UpdateSpeed()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	if (this->state->CheckFlag(this->WALK))
	{
		this->speed = json.GetJson(JsonManager::FileType::ENEMY)["SPEED"];
	}
	else if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		AttackSpeed(this->attackType, json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_SPEED"][this->attackType]);
	}
	else
	{
		this->speed = 0.0f;
	}
}

/// <summary>
/// 回転率の更新
/// </summary>
void Boss::UpdateRotation()
{
	/*回転できなければ早期リターン*/
	if (!CanRotation())return;

	/*シングルトンクラスのインスタンスの取得*/
	auto& player = Singleton<PlayerManager>::GetInstance();

	/*使用する値の準備*/
	const VECTOR position	 = this->collider->rigidbody.GetPosition();//座標
		  VECTOR nowRotation = this->collider->rigidbody.GetRotation();//回転率
		  VECTOR nextMoveTarget = player.GetRigidbody().GetPosition();
		  

	if (this->state->CheckFlag(this->MASK_ATTACK))
	{
		if (this->isCount[this->attackType])
		{
			this->moveTarget = nextMoveTarget;
		}
		else
		{
			if (this->state->CheckFlag(this->HURRICANE_KICK))
			{
				this->moveTarget = Lerp(this->moveTarget,nextMoveTarget,VGet(0.01f,0.01f,0.01f));
			}
		}
	}
	else
	{
		this->moveTarget = nextMoveTarget;
	}
	
	/*プレイヤーから自分の座標までのベクトルを出す*/
	VECTOR positonToTargetVector = VSub(position, this->moveTarget);

	/*アークタンジェントを使って角度を求める*/
	nowRotation.y = static_cast<float>(atan2(static_cast<double>(positonToTargetVector.x), static_cast<double>(positonToTargetVector.z)));

	/*回転率を代入*/
	this->collider->rigidbody.SetRotation(nowRotation);
}




/// <summary>
/// 回転できるか
/// </summary>
/// <returns></returns>
const bool Boss::CanRotation()const
{
	//if (this->state->CheckFlag(this->MASK_ATTACK)/* || this->state->CheckFlag(this->REST)*/)return false;
	return true;
}

const bool Boss::CanMove()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return false;
	if (this->state->CheckFlag(this->ROAR))return false;
	return true;
}
/// <summary>
/// 攻撃できるか
/// </summary>
const bool Boss::CanAttack()const
{
	//if (this->state->CheckFlag(this->PUNCH_ATTACK))		 return false;
	//if (this->state->CheckFlag(this->KNOCKGROUND_ATTACK))return false;
	//if (this->state->CheckFlag(this->RUSH_ATTACK))		 return false;
	//if (this->state->CheckFlag(this->BREATH_ATTACK))	 return false;
	return true;
}
/// <summary>
/// 休憩できるか
/// </summary>
const bool Boss::CanRest()const
{
	/*攻撃中かつアニメーションが終了している*/
	if (this->state->CheckFlag(this->MASK_ATTACK) && this->animation->GetIsChangeAnim())
	{
		return true;
	}
	return false;
}
/// <summary>
/// 咆哮できるか
/// </summary>
const bool Boss::CanRoar()const
{
	/*攻撃中かつアニメーションが終了している*/
	if (this->state->CheckFlag(this->REST)) return true;
	return false;
}
/// <summary>
/// 指定したフレームの計測
/// </summary>
bool Boss::FrameCount(const int _index, const int _maxFrame)
{
	/*もしカウントが開始していたら*/
	if (this->isCount[_index])
	{
		//カウントを増加させる
		this->frameCount[_index]++;
		//もし最大を越していたらフラグを下してカウントを初期化する
		if (this->frameCount[_index] >= _maxFrame)
		{
			this->isCount[_index] = false;
			this->frameCount[_index] = 0;
			return true;
		}
	}
	return false;
}

const bool Boss::GetIsAttack()const
{
	if (this->state->CheckFlag(this->MASK_ATTACK))return true;
	return false;
}
const VECTOR Boss::GetHeadPosition()const
{
	return MV1GetFramePosition(this->modelHandle, 7);
}

/// <summary>
/// アニメーション再生時間の取得
/// </summary>
const float Boss::GetAnimationPlayTime()const
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	return json.GetJson(JsonManager::FileType::ENEMY)["ANIMATION_PLAY_TIME"][this->nowAnimation];
}

/// <summary>
/// エフェクトフラグを立てる
/// </summary>
void Boss::OnEffectFlag(const int _attack)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& effect = Singleton<EffectManager>::GetInstance();

	switch (_attack)
	{
	case static_cast<int>(AttackType::NONE):
		break;
	case static_cast<int>(AttackType::SLASH):
		effect.OnIsBossSlashEffect();
		break;
	case static_cast<int>(AttackType::FLY_ATTACK):
		break;
	case static_cast<int>(AttackType::HURRICANE_KICK):
		break;
	case static_cast<int>(AttackType::JUMP_ATTACK):
		effect.OnIsBossJumpAttackEffect();
		break;
	case static_cast<int>(AttackType::ROTATE_PUNCH):
		effect.OnIsBossRotatePunchEffect();
		break;
	default:
		break;
	}
}
/// <summary>
/// 攻撃フラグを立てる
/// </summary>
void Boss::SetAttackFlag(const int _attack)
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& effect = Singleton<EffectManager>::GetInstance();

	switch (_attack)
	{
	case static_cast<int>(AttackType::NONE):
		break;
	case static_cast<int>(AttackType::SLASH):
		this->state->SetFlag(this->SLASH);
		break;
	case static_cast<int>(AttackType::FLY_ATTACK):
		this->state->SetFlag(this->FLY_ATTACK);
		break;
	case static_cast<int>(AttackType::HURRICANE_KICK):
		this->state->SetFlag(this->HURRICANE_KICK);
		break;
	case static_cast<int>(AttackType::JUMP_ATTACK):
		this->state->SetFlag(this->JUMP_ATTACK);
		break;
	case static_cast<int>(AttackType::ROTATE_PUNCH):
		this->state->SetFlag(this->ROTATE_PUNCH);
		break;
	default:
		break;
	}
}
/// <summary>
/// アニメーションの再生時間を遅くする
/// </summary>
void Boss::SlowAnimationPlayTime(const FrameCountType _type,const int _targetCount, const float _maxTime)
{
	int type = static_cast<int>(_type);
	if (this->attackType == type)
	{
		float animationPlayTime = _maxTime;
		FrameCount(type, _targetCount);
		if (this->isCount[type])
		{
			animationPlayTime *= 0.1f;
			this->animationPlayTime = animationPlayTime;
		}
		else
		{
			this->animationPlayTime = _maxTime;
		}
	}
}
/// <summary>
/// アニメーションの再生時間を追加
/// </summary>
void Boss::AddAnimationPlayTime(const FrameCountType _type, const int _targetCount, const float _maxTime)
{
	int type = static_cast<int>(_type);
	if (this->attackType == type)
	{
		if (this->isCount[type])
		{
			if (this->frameCount[type] == 0)
			{
				this->animationPlayTime = 0.0f;
			}
		}
		FrameCount(type, _targetCount);
		this->animationPlayTime += _maxTime / static_cast<float>(_targetCount);
		if (this->animationPlayTime >= _maxTime)
		{
			this->animationPlayTime = _maxTime;
		}
	}
}
/// <summary>
/// 攻撃時のスピードの設定
/// </summary>
void Boss::AttackSpeed(const int _type,const float _speed)
{
	if (this->attackType == _type)
	{
		if (this->isCount[_type])
		{
			this->speed = 0.0f;
		}
		else
		{
			this->speed = _speed;
			VECTOR position = this->collider->rigidbody.GetPosition();
			VECTOR distance = VSub(this->moveTarget, position);
			float distanceSize = VSize(distance);
			if (distanceSize < 5.0f)
			{
				this->speed = 0.0f;
			}
		}
	}
}

void Boss::SetAttackComboCount()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->attackComboCount = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO_COUNT"][this->nowPhase];
	this->attackComboIndexOffset = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO_INDEX_OFFSET"][this->nowPhase];
}

void Boss::SetPhase()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();

	auto& collider = dynamic_cast<CharacterColliderData&>(*this->collider);
	auto& data = dynamic_cast<BossData&>(*collider.data);
	const float HP = data.hp;
	const float MAX_HP = json.GetJson(JsonManager::FileType::ENEMY)["HP"];

	/*HPが最大値の1/3未満だったらフェーズ３*/
	if (HP < (MAX_HP / 3))
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_3);
	}
	/*HPが最大値の2/3未満だったらフェーズ2*/
	else if (HP < (MAX_HP / 3) * 2)
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_2);
	}
	/*フェーズ１*/
	else
	{
		this->nowPhase = static_cast<int>(Phase::PHASE_1);
	}
}

void Boss::SetAttackCombo()
{
	/*シングルトンクラスのインスタンスの取得*/
	auto& json = Singleton<JsonManager>::GetInstance();
	auto& attack = Singleton<BossAttackManager>::GetInstance();

	/*攻撃中だったら早期リターン*/
	if (this->state->CheckFlag(this->MASK_ATTACK))return;

	//コンボカウントが0だったら
	if (this->attackComboCount < 0)
	{
		SetAttackComboCount();
		int type = GetRand(this->attackComboCount) + this->attackComboIndexOffset;
		vector<int> combo = json.GetJson(JsonManager::FileType::ENEMY)["ATTACK_COMBO"][type];
		this->attackCombo = combo;
	}

	this->attackType = this->attackCombo[this->attackComboCount];
	SetAttackFlag(this->attackType);
	attack.OnIsStart(this->attackType);
	OnEffectFlag(this->attackType);
	this->isCount[this->attackType] = true;
	this->attackComboCount--;
}

/// <summary>
/// 回転率の設定
/// </summary>
void Boss::SetRotation(const VECTOR _rotation)
{
	this->collider->rigidbody.SetRotation(_rotation);
}

/// <summary>
/// 移動ベクトルの設定
/// </summary>
void Boss::SetVelocity(const VECTOR _velocity)
{
	this->collider->rigidbody.SetVelocity(_velocity);
}