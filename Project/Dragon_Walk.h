//=====================================================
// @brief 歩きアクション
//=====================================================
#pragma once

class ActionNode;
class Dragon_Walk : public ActionNode
{
public:
	 Dragon_Walk();//コンストラクタ
	~Dragon_Walk();//デストラクタ

	NodeState Update()override;//更新
};

