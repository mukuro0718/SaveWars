//=====================================================
// @brief �X�[�p�[�m���@�A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_SuperNova : public ActionNode
{
public:
	 Beast_SuperNova();//�R���X�g���N�^
	~Beast_SuperNova();//�f�X�g���N�^

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

