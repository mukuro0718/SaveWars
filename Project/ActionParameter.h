//========================================
// @brief �A�N�V�����p�����[�^�[�N���X
// �������̃p�����[�^�[���炻�̃A�N�V�����ɑ΂���
// �~���l�����߁A��������ƂɊ��Ғl���o��
//========================================
#pragma once

class ActionParameter
{
public:
	ActionParameter();//�R���X�g���N�^
	~ActionParameter(){}//�f�X�g���N�^

	bool CalcInterval();//�C���^�[�o���̌v�Z
	
	/*getter/setter*/
		  void SetInterval	(const int _set);		//�C���^�[�o���̃Z�b�g
	const int  GetWeight	(const int _sum)const;	//�d�݂��擾

	/*�ÓI�萔*/
	static constexpr float BASE_WEIGHT = 100.0f;//�x�[�X�̏d��
	
	/*�����o�ϐ�*/
	float	desireValue;//�~���l
	int		interval;	//�C���^�[�o��
};

