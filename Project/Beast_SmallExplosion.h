//=====================================================
// @brief �O���ɏ������A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_SmallExplosion : public ActionNode
{
public:
	 Beast_SmallExplosion();//�R���X�g���N�^
	~Beast_SmallExplosion();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

