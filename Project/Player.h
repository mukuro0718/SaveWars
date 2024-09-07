//===============================================
// @brief �v���C���[�N���X
//===============================================
#pragma once

class BitFlag;
class Physics;
class Collidable;
class Animation;
class Player : public GoriLib::Collidable
{
public:
	Player();//�R���X�g���N�^
	~Player();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics);			//������
	void		Finalize  (GoriLib::Physics* _physics);			//�㏈��
	void		Update	  (GoriLib::Physics* _physics);			//�X�V
	void		OnCollide (const Collidable& _colider)override;	//�Փ˂����Ƃ�
	const void	Draw	  ()const;								//�`��

	/*getter*/
	const int		GetHitNumber()const { return this->attackNumber; }//�U���ԍ��̎擾
	const int		GetDamage	()const;							  //�_���[�W�̎擾
	const VECTOR	GetPosition	()const;							  //���W�̎擾
	const VECTOR	GetDirection()const { return this->direction; }	  //�ړ��x�N�g���̎擾
	const bool		IsMove		()const;							  //�ړ��t���O�̎擾
	const bool		GetIsSlash ()const;							  //�V���b�g�t���O�̎擾
	const int		GetHP		()const;							  //HP�̎擾
	const float		GetStamina	()const;
private:
	

	/*�ÓI�萔*/
	static constexpr int COUNT_NUM = 4;//�t���[���J�E���g�̐�
	static constexpr int COLLIDER_NUM = 2;//�R���C�_�[�̐�
	//�v���C���[�̏��
	static constexpr unsigned int IDLE			 = (1 << 0); //�ҋ@
	static constexpr unsigned int ROLL			 = (1 << 1); //���
	static constexpr unsigned int DEATH			 = (1 << 2); //�u���b�N
	static constexpr unsigned int BLOCK			 = (1 << 3); //�u���b�N
	static constexpr unsigned int JUMP			 = (1 << 4); //�u���b�N
	static constexpr unsigned int REACTION		 = (1 << 5); //�u���b�N
	static constexpr unsigned int BLOCK_REACTION = (1 << 6); //�u���b�N
	static constexpr unsigned int RUNNING		 = (1 << 7); //����
	static constexpr unsigned int WALK_BACK		 = (1 << 8); //����
	static constexpr unsigned int WALK_FRONT	 = (1 << 9); //����
	static constexpr unsigned int WALK_LEFT		 = (1 << 10); //����
	static constexpr unsigned int WALK_RIGHT	 = (1 << 11); //����
	static constexpr unsigned int SLASH			 = (1 << 12); //����
	//�}�X�N
	static constexpr unsigned int MASK_CANT_RECOVERY_STAMINA = ROLL | REACTION;
	static constexpr unsigned int MASK_ATTACK = SLASH;
	static constexpr unsigned int MASK_REACTION = BLOCK_REACTION | REACTION;
	static constexpr unsigned int MASK_MOVE = WALK_BACK | WALK_FRONT | WALK_LEFT | WALK_RIGHT | RUNNING; //�ړ��}�X�N
	static constexpr unsigned int MASK_ALL = MASK_MOVE | IDLE ;
	static constexpr unsigned int MASK_CAN_VELOCITY = MASK_MOVE | ROLL | JUMP;
	/*�񋓑�*/
	//�R���C�_�[�̎��
	enum class ColliderType
	{
		CHARACTER = 0,
		ATTACK = 1,
	};
	//�t���[���J�E���g�̎��
	enum class FrameCountType
	{
		AVOID = 0,//���
	};
	//�A�j���[�V�����̎��
	enum class AnimationType
	{
		IDLE			= 0,
		ROLL			= 1,
		DEATH			= 2,
		BLOCK			= 3,
		JUMP			= 4,
		REACTION		= 5,
		BLOCK_REACTION  = 6,
		RUNNING			= 7,
		WALK_BACK		= 8,
		WALK_FRONT		= 9,
		WALK_LEFT		= 10,
		WALK_RIGHT		= 11,
		SLASH			= 12,
	};

	/*���������֐�*/
	void UpdateSpeed	 ();//���x�̍X�V
	void UpdateMoveVector();//�ړ��x�N�g���̍X�V
	void UpdateRotation	 ();//��]���̍X�V
	void Move			 ();//�ړ�
	void LockOn			 ();//���b�N�I��
	void Rolling		 ();//���
	void Reaction		 ();//���A�N�V������
	void Attack			 ();//�U��
	void Death			 ();//�f�X
	void Block			 ();//�u���b�N
	void Jump			 ();//�W�����v
	//���t���O
	const bool CanRotation()const;
	const bool CanRolling()const;
	const bool CanAttack()const;
	const bool CanBlock()const;
	const bool CanJump()const;
	const bool DontAnyAction()const;

		  void UpdateAnimation	 ();		//���݂̃A�j���[�V�����̍X�V
		  bool FrameCount(const int _index,const int _maxFrame);
	const bool CanAction(const float _staminaConsumed)const;
	void CalcStamina(const float _staminaConsumed);//�X�^�~�i�̉񕜏���
	/*�����o�ϐ�*/

	BitFlag*			state;						//���
	Animation*			animation;					//�A�j���[�V����
	VECTOR				direction;					//�����Ă������
	VECTOR				moveVectorRotation;			//�ړ��x�N�g���p��]�l
	std::vector<int>	frameCount;					//�t���[���J�E���g
	std::vector<bool>	isCount;					//�J�E���g�����邩
	int attackType;
	std::map<unsigned int, int> animationMap;
	float				jumpPower;					//�W�����v��
	int					nowAnimation;				//�A�j���[�V����
	float				animationPlayTime;			//�A�j���[�V�����Đ�����
	int damage;
	int attackComboCount;
	int attackNumber;
	bool isDraw;


	int modelHandle;
	int prevHitNum;
	float speed;
	bool isGround;
	float stamina;
	bool isLockOn;
};

