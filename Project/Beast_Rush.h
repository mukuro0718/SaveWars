//=====================================================
// @brief �ˌ��A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_Rush : public ActionNode
{
public:
	 Beast_Rush();//�R���X�g���N�^
	~Beast_Rush();//�f�X�g���N�^

	NodeState Update()override;//�X�V
private:	
	/*�\����*/
	//�A�j���[�V�����̒i�K
	enum class AnimationStage
	{
		START,
		LOOP,
		NORMAL_END,
		ANGRY_END,
	};

	/*�����o�ϐ�*/
	AnimationStage stage;
	std::map<AnimationStage, int>			 animationSet;//�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;//���̃X�e�[�W�̃Z�b�g
};

