//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class BeastBehaviorTree;
class Beast : public Enemy
{
public:
	 Beast();//�R���X�g���N�^
	~Beast();//�f�X�g���N�^

	void		Initialize			() override;		//������
	void		Finalize			() override;		//�㏈��
	void		Update				() override;		//�X�V
	const void	DrawCharacterInfo	() const override;	//�`��
	void		PlayAnimation		();					//�A�j���[�V�����̍Đ�

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		DYING						= 0, //�f�X(97)
		IDLE						= 1, //�ҋ@(111)
		ROAR						= 2, //���K(�ʏ큨�{���Ԃ̕ω�110)
		WALK						= 3, //����(121)
		RUN							= 4, //�_�b�V��(117)
		TURN_LEFT					= 5, //��������(119)
		TURN_RIGHT					= 6, //�E������(120)
		DOWN						= 7, //�_�E��(4)
		LONG_FRIGHT_START			= 8, //��������(�J�n103)
		LONG_FRIGHT_LOOP			= 9, //��������(�r��102)
		LONG_FRIGHT_END				= 10,//��������(�I��100)
		SHORT_FRIGHT_START			= 11,//�Z������(�J�n48)
		SHORT_FRIGHT_END			= 12,//�Z������(�I��49)
		REST_START					= 13,//�x�e(�J�n89)
		REST_LOOP					= 14,//�x�e(�r��90)
		REST_END					= 15,//�x�e(�I��91)
		STEP_BACK					= 16,//��щ�����(8)
		RAISE_LEVEL					= 17,//�`���[�W���x���̏㏸(14)
		BACKING_BREATH				= 18,//������Ȃ���u���X(10)
		RUSH_START					= 19,//�ːi(�J�n75)
		RUSH_LOOP					= 20,//�ːi(�r��76)
		RUSH_NORMAL_END				= 21,//�ːi(���ʂɎ~�܂�77)
		RUSH_ROTATE_END				= 22,//�ːi(��]���Ď~�܂�78)
		CHARGE_RIGHT_ATTACK			= 23,//�E�O�����ߍU��(52)
		CHARGE_BOTH_ATTACK_START	= 24,//���O�����ߍU��(�J�n50)
		CHARGE_BOTH_ATTACK_END		= 24,//���O�����ߍU��(�I��51)
		BREATH_START				= 25,//�u���X(�J�n45)
		BREATH_LOOP					= 26,//�u���X(�r��46)
		BREATH_END					= 27,//�u���X(�I��47)
		SUPER_NOVA_START			= 28,//�X�[�p�[�m���@(�J�n27)
		SUPER_NOVA_LOOP				= 29,//�X�[�p�[�m���@(�r��28)
		SUPER_NOVA_END				= 30,//�X�[�p�[�m���@(�I��29)
		WEAK_BREATH					= 31,//��u���X(80)
		SMALL_EXPLOSION				= 32,//������(9)
		RIGHT_FOOT_ATTACK			= 33,//�E���U��(40)
		COMBO_ATTACK_1				= 34,//�E���ōU���������]�U��(21)
		COMBO_ATTACK_2				= 35,//3�A���U��(22)
	};
	
	/*getter/setter*/
	const bool		 GetIsAttack			()const override;													//�R�E�Q�L������
		  void		 SetRotation			(const VECTOR _rotation);											//��]���̐ݒ�
		  void		 SetVelocity			(const VECTOR _velocity);											//�ړ��x�N�g���̐ݒ�
		  void		 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//���݂̈ړ��ڕW��ݒ�
		  void		 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//�����̐ݒ�
	const float		 GetAnimationPlayTime	()const;															//�A�j���[�V�����Đ����Ԃ̎擾
	const float		 GetNowAnimationPlayTime()const { return this->animationPlayTime; }							//���݂̃A�j���[�V�����Đ����Ԃ̎擾
		  void		 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//�A�j���[�V�����Đ����Ԃ̐ݒ�
		  void		 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//���݂̃A�j���[�V�����̐ݒ�
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 8.0f; //�e�̃T�C�Y
	
	/*�����o�ϐ�*/
	VECTOR				moveTarget;				//�ړ��ڕW
	float				animationPlayTime;		//�A�j���[�V�����Đ�����
	int					nowAnimation;			//���݂̃A�j���[�V����
};

