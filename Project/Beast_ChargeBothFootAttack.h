//=====================================================
// @brief ���O�����ߍU���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_ChargeBothFootAttack : public ActionNode
{
public:
	 Beast_ChargeBothFootAttack();//�R���X�g���N�^
	~Beast_ChargeBothFootAttack();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�\����*/
	//�A�j���[�V�����̒i�K
	enum class AnimationStage
	{
		START,
		END,
	};

	/*�����o�ϐ�*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;//�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;//���̃X�e�[�W�̃Z�b�g

};

