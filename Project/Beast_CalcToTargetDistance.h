//=====================================================
// @brief �v���C���[�Ƃ̋��������߂�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_CalcToTargetDistance : public ActionNode
{
public:
	 Beast_CalcToTargetDistance();//�R���X�g���N�^
	~Beast_CalcToTargetDistance();//�f�X�g���N�^

	NodeState Update()override;//�X�V
};

