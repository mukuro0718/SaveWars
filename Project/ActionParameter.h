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

	static constexpr int MAX_PARAMETER = 255;
	int desireValue;//�~���l
	int interval;	//�C���^�[�o��
};

