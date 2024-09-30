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

	void AddDesireValue(const int _addValue);
	bool CalcInterval  ();
	
	/*getter/setter*/
	void	  SetInterval(const int _set);
	const int GetWeight	 (const int _sum)const;

	/*�ÓI�萔*/
	static constexpr float MAX_PARAMETER		 = 255.0f;	//�ő�p�����[�^�[
	static constexpr float BASE_ADD_DESIRE_VALUE = 1.0f;	//
	static constexpr float BASE_WEIGHT			 = 100.0f;	//
	
	/*�����o�ϐ�*/
	float	desireValue;//�~���l
	int		interval;	//�C���^�[�o��
};

