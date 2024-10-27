//===============================================
// @brief �v���C���[�N���X
//===============================================
#pragma once

class Character;
class HitStop;
class Player : public Character
{
public:
	Player();//�R���X�g���N�^
	~Player();//�f�X�g���N�^

	void		Initialize		 () override;		//������
	void		Finalize		 () override;		//�㏈��
	void		Update			 () override;		//�X�V
	const void	DrawCharacterInfo()const override;	//�`��

	/*getter*/
	const bool	IsMove		 ()const;								//�ړ��t���O�̎擾
	const bool	GetIsAttack	 ()const override;						//�V���b�g�t���O�̎擾
	const bool  GetIsLockOn  ()const { return this->isLockOn; }		//���b�N�I���t���O�̎擾
	const int	GetStamina	 ()const;								//�X�^�~�i�̎擾
	const int	GetHealOrbNum()const { return this->healOrbNum; }	//�񕜃I�[�u�̐�(���͉񕜉񐔂ɂȂ��Ă���)
private:
	/*�ÓI�萔*/
	static constexpr float	SHADOW_HEIGHT	= 10.0f;//�e�𓊉e���鍂��
	static constexpr float	SHADOW_SIZE		= 6.0f;	//�����̃T�C�Y
	static constexpr int	COUNT_NUM		= 5;	//�t���[���J�E���g�̐�
	//�v���C���[�̏��
	static constexpr unsigned int IDLE			  = (1 << 0);  //�ҋ@
	static constexpr unsigned int AVOID			  = (1 << 1);  //���
	static constexpr unsigned int DEATH			  = (1 << 2);  //�f�X
	static constexpr unsigned int BLOCK			  = (1 << 3);  //�u���b�N
	static constexpr unsigned int REACTION		  = (1 << 4);  //���A�N�V����
	static constexpr unsigned int BLOCK_REACTION  = (1 << 5);  //�u���b�N���A�N�V����
	static constexpr unsigned int WALK_FRONT	  = (1 << 6);  //����
	static constexpr unsigned int WALK_BACK		  = (1 << 7);  //������
	static constexpr unsigned int WALK_LEFT		  = (1 << 8);  //������
	static constexpr unsigned int WALK_RIGHT	  = (1 << 9);  //�E����
	static constexpr unsigned int RUN_FRONT		  = (1 << 10); //�O�_�b�V��
	static constexpr unsigned int RUN_BACK		  = (1 << 11); //���_�b�V��
	static constexpr unsigned int RUN_LEFT		  = (1 << 12); //���_�b�V��
	static constexpr unsigned int RUN_RIGHT		  = (1 << 13); //�E�_�b�V��
	static constexpr unsigned int RUN_180_TURN	  = (1 << 14); //180�����ύX
	static constexpr unsigned int COMBO_1		  = (1 << 15); //�U��
	static constexpr unsigned int HEAL			  = (1 << 16); //��
	static constexpr unsigned int COMBO_2		  = (1 << 17); //��
	static constexpr unsigned int COMBO_3		  = (1 << 18); //��
	static constexpr unsigned int SKILL			  = (1 << 19); //��
	
	//�}�X�N
	static constexpr unsigned int MASK_REACTION				 = BLOCK_REACTION | REACTION;						//���A�N�V����
	static constexpr unsigned int MASK_WALK					 = WALK_FRONT | WALK_BACK | WALK_LEFT | WALK_RIGHT; //�����}�X�N
	static constexpr unsigned int MASK_RUN					 = RUN_FRONT | RUN_BACK | RUN_LEFT | RUN_RIGHT;		//�_�b�V���}�X�N
	static constexpr unsigned int MASK_MOVE					 = MASK_WALK | MASK_RUN;							//�ړ��}�X�N
	static constexpr unsigned int MASK_AVOID				 = AVOID;											//����}�X�N
	static constexpr unsigned int MASK_ATTACK				 = COMBO_1 | COMBO_2 | COMBO_3 | SKILL;				//�U���}�X�N
	static constexpr unsigned int MASK_CANT_RECOVERY_STAMINA = MASK_AVOID | BLOCK;				//�X�^�~�i���񕜂ł��Ȃ����
	static constexpr unsigned int MASK_ALWAYS_TURN_OFF		 = MASK_MOVE | IDLE;								//���t���[����Ԃ������}�X�N
	static constexpr unsigned int MASK_ALL					 = MASK_MOVE | IDLE | MASK_ATTACK | MASK_REACTION |
															   BLOCK | MASK_AVOID | HEAL;						//���ׂĂ̏�ԃ}�X�N
	static constexpr unsigned int MASK_CAN_VELOCITY			 = MASK_MOVE | MASK_AVOID;							//VELOCITY���o����}�X�N
	/*�񋓑�*/
	//�R���C�_�[�̎��
	enum class ColliderType
	{
		CHARACTER	= 0,//�L�����N�^�[
		ATTACK		= 1,//�U��
	};
	//�t���[���J�E���g�̎��
	enum class FrameCountType
	{
		JUST_AVOID	= 0,//�W���X�g���
		HEAL		= 1,//��
		AVOID		= 2,//���
		LOCK_ON		= 3,//���b�N�I��
		ATTACK		= 4,//�U��
	};
	//�A�j���[�V�����̎��
	enum class AnimationType
	{
		IDLE			= 0,
		AVOID			= 1,
		DEATH			= 2,
		BLOCK			= 3,
		REACTION		= 4,
		BLOCK_REACTION  = 5,
		WALK_FRONT		= 6,
		WALK_BACK		= 7,
		WALK_LEFT		= 8,
		WALK_RIGHT		= 9,
		RUN_FRONT		= 10,
		RUN_BACK		= 11,
		RUN_LEFT		= 12,
		RUN_RIGHT		= 13,
		RUN_180_TURN	= 14,
		COMBO_1			= 15,
		HEAL			= 16,
		COMBO_2			= 17,
		COMBO_3			= 18,
		SKILL			= 19,
	};

	/*���������֐�*/
	void UpdateSpeed	 ();//���x�̍X�V
	void UpdateMoveVector();//�ړ��x�N�g���̍X�V
	void UpdateRotation	 ();//��]���̍X�V
	void Move			 ();//�ړ�
	void Rolling		 ();//���
	void Reaction		 ();//���A�N�V����
	void Attack			 ();//�U��
	void Death			 ();//�f�X
	void Block			 ();//�u���b�N
	void Heal			 ();//��
	void LockOn			 ();//���b�N�I��

	//���t���O
	const bool CanRotation		()const;							//��]�ł��邩
	const bool CanRolling		()const;							//����ł��邩
	const bool CanAttack		()const;							//�U���ł��邩
	const bool CanBlock			()const;							//�K�[�h�ł��邩
	const bool CanHeal			()const;							//�񕜂ł��邩
	const bool DontAnyAction	()const;							//�ق��ɃA�N�V�������s���Ă��邩
	const bool CanAction		(const float _staminaConsumed)const;//�A�N�V�������ł��邩

		  void UpdateAnimation	();										//���݂̃A�j���[�V�����̍X�V
		  void CalcStamina		(const float _staminaConsumed);			//�X�^�~�i�̉񕜏���
		  bool FrameCount		(const int _index,const int _maxFrame);	//�t���[���J�E���g
	/*�����o�ϐ�*/
	HitStop*							 hitStop;			//�q�b�g�X�g�b�v
	VECTOR								 moveVectorRotation;//�ړ��x�N�g���p��]�l
	std::vector<int>					 frameCount;		//�t���[���J�E���g
	std::vector<bool>					 isCount;			//�J�E���g�����邩
	std::map<unsigned int, int>			 animationMap;		//�A�j���[�V�����}�b�v
	std::map<int, unsigned int>			 reactionMap;		//���A�N�V�����}�b�v
	std::map<unsigned int, unsigned int> whenRunMoveState;	//�_�b�V�����̏��
	VECTOR								 nextRotation;		//���̉�]��
	int									 nowAnimation;		//�A�j���[�V����
	float								 animationPlayTime;	//�A�j���[�V�����Đ�����
	int									 attackComboCount;	//�U���R���{�J�E���g
	int									 healOrbNum;		//�񕜃I�[�u�̐�
	float								 dot;				//����
	bool								 isLockOn;			//���b�N�I��
	int									 attackMaxFrame;	//�U���ő�t���[��
	float								 deg;				//�x��
	bool								 isDecSpeed;		//�����t���O
};

