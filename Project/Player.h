//===============================================
// @brief �v���C���[�N���X
//===============================================
#pragma once

class Model;
class BitFlag;
class Satellite;
class Shield;
class Collider;
class Player
{
public:
	Player();//�R���X�g���N�^
	~Player();//�f�X�g���N�^

	void		Initialize();		//������
	void		Action	  ();		//�A�N�V����
	void		Update	  ();		//�X�V
	const void	Draw	  ()const;	//�`��

	/*getter*/
	const VECTOR	GetPosition		  ()const;									  //���W�̎擾
	const VECTOR	GetDirection	  ()const { return this->direction; }		  //�ړ��x�N�g���̎擾
	const bool		IsMove			  ()const;									  //�ړ��t���O�̎擾
	const bool		IsAttack		  ()const;									  //�V���b�g�t���O�̎擾
	const bool		IsLightAttack	  ()const;									  //�y�U��
	const bool		IsHeavyAttack	  ()const;									  //�d�U��
	const void		FixMoveVector	  (const VECTOR _fixVector);				  //�ړ��x�N�g���̏C��
	const Collider	GetCollider		  ();										  //�R���C�_�[�̎擾
	const int		GetHP			  ()const { return this->hp; }				  //HP�̎擾
private:
	/*�ÓI�萔*/
	//�v���C���[�̏��
	static constexpr unsigned int IDLE	   = (1 << 0); //�ҋ@
	static constexpr unsigned int ATTACK   = (1 << 1); //�U��
	static constexpr unsigned int REACTION = (1 << 2); //���A�N�V�����i�U�����󂯂����j
	static constexpr unsigned int DEATH    = (1 << 3); //���S
	//�����A����
	static constexpr unsigned int STAND  = (1 << 0); //����
	static constexpr unsigned int CROUCH = (1 << 1); //����
	//�ړ�
	static constexpr unsigned int IDLE	  = (1 << 0); //�ҋ@
	static constexpr unsigned int RUN	  = (1 << 1); //����
	static constexpr unsigned int WALK	  = (1 << 2); //����
	static constexpr unsigned int LOCK_ON = (1 << 3); //���b�N�I��
	//�A�N�V����
	static constexpr unsigned int AVOID	= (1 << 0); //���
	static constexpr unsigned int JUMP  = (1 << 1); //�W�����v
	static constexpr unsigned int BLOCK = (1 << 2); //�u���b�N
	//���A�N�V����
	static constexpr unsigned int BIG_IMPACT   = (1 << 0); //��Ռ�
	static constexpr unsigned int SMALL_IMPACT = (1 << 1); //���Ռ�
	//�U��
	static constexpr unsigned int CASTING				 = (1 << 2); //�r��
	static constexpr unsigned int COMBO_ATTACK			 = (1 << 3); //�R���{
	static constexpr unsigned int CROUCH_SLASH			 = (1 << 4); //���Ⴊ�ݐ؂�
	static constexpr unsigned int JUMP_ATTACK			 = (1 << 5); //�W�����v�U��
	static constexpr unsigned int JUMP_ROTATION_ATTACK	 = (1 << 6); //�W�����v��]�U��
	static constexpr unsigned int KICK					 = (1 << 6); //�L�b�N
	static constexpr unsigned int PUNCH					 = (1 << 6); //�p���`
	static constexpr unsigned int ROTATION_ATTACK		 = (1 << 6); //��]�U��
	static constexpr unsigned int SLASH_1				 = (1 << 6); //�؂�P
	static constexpr unsigned int SLASH_2				 = (1 << 6); //�؂�Q

	/*�񋓑�*/
	//�t���[���J�E���g�̎��
	enum class FrameCountType
	{
		ATTACK_INTERVAL = 0,//�U���ҋ@
	};
	//�A�j���[�V�����̎��
	enum class AnimationType
	{
		CROUCH_IDLE			 = 0,//����ҋ@
		STANDING_IDLE		 = 1,//�����ҋ@
		STANDING_IDLE_PLAY	 = 2,//�����ҋ@�V��
		WALK				 = 3,//����
		WALK_180_TURN		 = 4,//����180�^�[��
		RUN					 = 5,//����
		RUN_180_TURN		 = 6,//����180�^�[��
		LOCK_ON_RUN_BACK	 = 7,//���b�N�I����둖��
		LOCK_ON_RUN_LEFT	 = 8,//���b�N�I��������
		LOCK_ON_RUN_RIGHT	 = 9,//���b�N�I���E����
		LOCK_ON_WALK_BACK	 = 10,//���b�N�I��������
		LOCK_ON_WALK_LEFT	 = 11,//���b�N�I��������
		LOCK_ON_WALK_RIGHT	 = 12,//���b�N�I���E����
		BLOCK				 = 13,//�u���b�N
		IDLE_JUMP			 = 14,//�ҋ@�W�����v
		MOVE_JUMP			 = 15,//�ړ��W�����v
		ROAR				 = 16,//���K
		BIG_IMPACT			 = 17,//��Ռ�
		SMALL_IMPACT		 = 18,//���Ռ�
		DEATH				 = 19,//�f�X
		CASTING				 = 20,//�r��
		COMBO				 = 21,//�R���{
		CROUCH_SLASH		 = 22,//���Ⴊ�ݐ؂�
		JUMP_ATTACK			 = 23,//�W�����v�U��
		JUMP_ROTATION_ATTACK = 24,//�W�����v��]�U��
		KICK				 = 25,//�R��
		PUNCH				 = 26,//����
		ROTATION_ATTACK		 = 27,//��]�U��
		SLASH_1				 = 28,//�؂�P
		SLASH_2				 = 29,//�؂�Q
	};

	/*���������֐�*/
		  void UpdateVelocity	 ();		//���x�̍X�V
		  void UpdateMoveVector	 ();		//�ړ��x�N�g���̍X�V
		  void UpdateRotation	 ();		//��]���̍X�V
		  void Attack			 ();		//�U��
		  void Move				 ();		//�ړ�
		  void Jump				 ();		//�W�����v
		  void UpdateAnimation	 ();		//���݂̃A�j���[�V�����̍X�V
	const bool CanMove			 ()const;//�ړ��ł��邩
	const bool CanAttack		 ()const;//�U���ł��邩
	const bool CanJump			 ()const;//�W�����v�ł��邩


	/*�����o�ϐ�*/

	Model*				model;						//���f��
	BitFlag*			state;						//�A�N�V�����̏��
	Collider*			collider;					//�R���C�_�[�N���X
	VECTOR				moveVector;					//�ړ��x�N�g��
	VECTOR				direction;					//�����Ă������
	VECTOR				fixVector;					//�␳�x�N�g��
	VECTOR				moveVectorRotation;			//�ړ��x�N�g���p��]�l
	VECTOR				wasd;						//wasd����
	VECTOR				lStick;						//lStick����
	float				velocity;					//���x
	std::vector<int>	frameCount;					//�t���[���J�E���g
	std::vector<bool>	isCount;					//�J�E���g�����邩
	std::map<unsigned int, int> animationMap;		//�A�j���[�V����
	std::map<int, unsigned int> attackTypeMap;
	bool				isShot;						//���@����������
	float				jumpPower;					//�W�����v��
	int					nowAnimation;				//�A�j���[�V����
	float				animationPlayTime;			//�A�j���[�V�����Đ�����
	int					hp;
};

