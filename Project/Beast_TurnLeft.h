//=====================================================
// @brief ���։��A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_TurnLeft : public ActionNode
{
public:
	 Beast_TurnLeft();//�R���X�g���N�^
	~Beast_TurnLeft();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	short maxFrameCount;
	short frameCount;
};

