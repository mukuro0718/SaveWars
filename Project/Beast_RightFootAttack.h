//=====================================================
// @brief 右足攻撃アクション
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_RightFootAttack : public ActionNode
{
public:
	 Beast_RightFootAttack();//コンストラクタ
	~Beast_RightFootAttack();//デストラクタ

	NodeState  Update()override;//更新
	const void Draw	 ()const;   //描画
private:
	short						attackStartCount;				//攻撃開始フレーム
	short						attackEndCount;					//攻撃終了フレーム
	short						frameCount;						//フレームカウント
	short						frameIndexUsedCapsuleDirection1;//カプセルを決めるためのフレーム番号
	short						frameIndexUsedCapsuleDirection2;//カプセルを決めるためのフレーム番号
	AttackCapsuleColliderData* collider;						//攻撃コライダー
};

