//=============================================
// @�q�b�g�X�g�b�v�x�[�X�N���X
// �e�q�b�g�X�g�b�v�N���X�͂��̃N���X���p��
// �������z�֐�
// �X�g�b�v�����邩�A�X���[�ɂ��邩
//=============================================

#pragma once
class HitStop final
{
public:
	HitStop();//�R���X�g���N�^
	~HitStop();//�f�X�g���N�^

	void Initialize();
	void Update();
	bool IsHitStop();//�q�b�g�X�g�b�v����

	enum class Type
	{
		NONE,
		STOP,
		SLOW
	};

	void SetHitStop(const float _time, const int _type, const int _delay, const float _slowFactor);//�q�b�g�X�g�b�v���Ԃ�ݒ�

private:
	static constexpr float SLOW_MOTION_FACTOR = 0.3f;

	/*�����o�ϐ�*/
	int type;
	int time;//�q�b�g�X�g�b�v����
	int delay;//�҂�����
	float slowFactor;
};

