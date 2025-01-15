//=====================================================
// @brief セレクターノード
// 子を順番に評価し、最初に成功した子ノードを実行する
// すべての子が失敗した場合、失敗として結果を返す
//=====================================================
#pragma once

class CompositeNode;
class SelectorNode : public CompositeNode
{
public:
	 SelectorNode();//コンストラクタ
	~SelectorNode();//デストラクタ

	NodeState Update() override;//更新
};

