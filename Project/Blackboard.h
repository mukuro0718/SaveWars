//====================================================
// @brief �u���b�N�{�[�h�N���X
// �r�w�C�r�A�c���[�̏�ԑJ�ڂ̂��߂̕ϐ���ێ�����
//====================================================
#pragma once
class Blackboard
{
public:
	VECTOR			 targetPosition;	//�ڕW�̍��W
	float			 toTargetDistance;	//�ڕW�܂ł̋���
	int				 internalState;		//�{�X�̓��I���
	int				 actionState;		//�A�N�V�����̏��
	int				 downValue;			//�_�E���l
	int				 angryValue;		//�{��l
	int				 currentHp;			//���݂�HP
	std::vector<int> interval;			//�C���^�[�o��
};

