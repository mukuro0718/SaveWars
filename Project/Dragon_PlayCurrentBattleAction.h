//=====================================================
// @brief �o�^���Ă���A�N�V���������s
//=====================================================
#pragma once

class ActionNode;
class Dragon_PlayCurrentBattleAction : public ActionNode
{
public:
	 Dragon_PlayCurrentBattleAction();//�R���X�g���N�^
	~Dragon_PlayCurrentBattleAction();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

