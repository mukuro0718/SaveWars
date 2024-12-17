//=====================================================
// @brief �u���X�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_Breath : public ActionNode
{
public:
	 Beast_Breath();//�R���X�g���N�^
	~Beast_Breath();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:
	/*�\����*/
	//�A�j���[�V�����̒i�K
	enum class AnimationStage
	{
		START,
		LOOP,
		END,
	};

	/*�����o�ϐ�*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;//�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;//���̃X�e�[�W�̃Z�b�g
};

