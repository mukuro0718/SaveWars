//=====================================================
// @brief �o�^���Ă���A�N�V���������s
//=====================================================
#pragma once

class ActionNode;
class Beast_PlayCurrentBattleAction : public ActionNode
{
public:
	 Beast_PlayCurrentBattleAction();//�R���X�g���N�^
	~Beast_PlayCurrentBattleAction();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

