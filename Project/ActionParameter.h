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
	bool CalcInterval();
	void SetInterval(const int _set);
	const int GetWeight(const int _sum)const;

	static constexpr float MAX_PARAMETER = 255.0f;
	static constexpr float BASE_ADD_DESIRE_VALUE = 1.0f;
	static constexpr float BASE_WEIGHT = 100.0f;
	float desireValue;//�~���l
	int interval;	//�C���^�[�o��
};

