//=============================================
// @�q�b�g�X�g�b�v�N���X
// �X�g�b�v�����邩�A�X���[�ɂ��邩
//=============================================

#pragma once
class HitStop final
{
public:
	 HitStop();//�R���X�g���N�^
	~HitStop();//�f�X�g���N�^

	void Initialize	();//������
	void Update		();//�X�V
	bool IsHitStop	();//�q�b�g�X�g�b�v����

	/*setter*/
	void SetHitStop(const float _time, const int _type, const int _delay, const float _slowFactor);//�q�b�g�X�g�b�v���Ԃ�ݒ�

	/*�񋓑�*/
	//���
	enum class Type
	{
		NONE = -1,
		STOP = 0,
		SLOW = 1
	};
private:
	/*�ÓI�萔*/
	static constexpr float SLOW_MOTION_FACTOR = 0.3f;//�X���[�W��

	/*�����o�ϐ�*/
	float	slowFactor;	//�X���[�W��
	int		type;		//���
	int		time;		//�q�b�g�X�g�b�v����
	int		delay;		//�҂�����
};

