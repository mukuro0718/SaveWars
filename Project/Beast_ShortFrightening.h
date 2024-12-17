//=====================================================
// @brief �Z�����݃A�N�V����
//=====================================================
#pragma once

class ActionNode;
class Beast_ShortFrightening : public ActionNode
{
public:
	 Beast_ShortFrightening();//�R���X�g���N�^
	~Beast_ShortFrightening();//�f�X�g���N�^

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
	AnimationStage							 stage;			//�A�j���[�V�����̒i�K
	std::map<AnimationStage, int>			 animationSet;	//�A�j���[�V�����̃Z�b�g
	std::map<AnimationStage, AnimationStage> nextStageSet;	//���̃X�e�[�W�̃Z�b�g
};

