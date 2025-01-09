//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class BeastBehaviorTree;
class CharacterData;
class CharacterColliderData;
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
		LONG_FRIGHT_START			= 8, //��������(�J�n104)
		LONG_FRIGHT_LOOP			= 9, //��������(�r��102)
		LONG_FRIGHT_END				= 10,//��������(�I��100)
		SHORT_FRIGHT_START			= 11,//�Z������(�J�n107)
		SHORT_FRIGHT_LOOP			= 12,//�Z������(�J�n106)
		SHORT_FRIGHT_END			= 13,//�Z������(�I��105)
		REST_START					= 14,//�x�e(�J�n89)
		REST_LOOP					= 15,//�x�e(�r��90)
		REST_END					= 16,//�x�e(�I��91)
		STEP_BACK					= 17,//��щ�����(8)
		RAISE_LEVEL					= 18,//�`���[�W���x���̏㏸(14)
		BACKING_BREATH				= 19,//������Ȃ���u���X(10)
		RUSH_START					= 20,//�ːi(�J�n75)
		RUSH_LOOP					= 21,//�ːi(�r��76)
		RUSH_ROTATE_END				= 22,//�ːi(��]���Ď~�܂�77)
		RUSH_NORMAL_END				= 23,//�ːi(���ʂɎ~�܂�78)
		CHARGE_RIGHT_ATTACK			= 24,//�E�O�����ߍU��(52)
		CHARGE_BOTH_ATTACK_START	= 25,//���O�����ߍU��(�J�n50)
		CHARGE_BOTH_ATTACK_END		= 26,//���O�����ߍU��(�I��51)
		BREATH_START				= 27,//�u���X(�J�n45)
		BREATH_LOOP					= 28,//�u���X(�r��46)
		BREATH_END					= 29,//�u���X(�I��47)
		SUPER_NOVA_START			= 30,//�X�[�p�[�m���@(�J�n27)
		SUPER_NOVA_LOOP				= 31,//�X�[�p�[�m���@(�r��28)
		SUPER_NOVA_END				= 32,//�X�[�p�[�m���@(�I��29)
		WEAK_BREATH					= 33,//��u���X(80)
		SMALL_EXPLOSION				= 34,//������(9)
		RIGHT_FOOT_ATTACK			= 35,//�E���U��(40)
		EXPLOSION					= 36,//�����U���i�X�[�p�[�m���@�����������j(65)
		COMBO_ATTACK				= 37,//�E���ōU���������]�U��(22)
	};
	enum class Parts
	{
		HEAD,
		BODY,
		LEFT_HAND,
		RIGHT_HAND,
		LEFT_FOOT,
		RIGHT_FOOT,
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
	vector<CharacterColliderData*>  partsCollider;				//���ʂ��Ƃ̃R���C�_�[
	short							maxPartsColliderNum;		//�R���C�_�[�̐�
	VECTOR							moveTarget;					//�ړ��ڕW
	float							animationPlayTime;			//�A�j���[�V�����Đ�����
	int								nowAnimation;				//���݂̃A�j���[�V����
	vector<vector<short>>			frameIndexUsePartsColider;	//���ʂ��Ƃ̃R���C�_�[�̎w��Ɏg�p����t���[���ԍ�
	vector<VECTOR>					pos1;						//���ʂ��Ƃ̃R���C�_�[�̍��W�����Ă���
	vector<VECTOR>					pos2;						//HACK:�Ȃ����������������Ȃ��Ƃ��܂�����Ȃ��A�Ȃ�
	vector<float>					prevPartsHp;				//�O�̕��ʂ��Ƃ�HP
	float							maxHp;						//���ʂ̍ő�HP
};

