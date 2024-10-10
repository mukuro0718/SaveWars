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
	const bool	IsMove		 ()const;//�ړ��t���O�̎擾
	const bool	GetIsAttack	 ()const override;//�V���b�g�t���O�̎擾
	const int	GetStamina	 ()const;
	const int	GetHealOrbNum()const { return this->healOrbNum; }
private:
	/*�ÓI�萔*/
		/*�ÓI�萔*/
	const float SHADOW_HEIGHT = 10.0f;
	const float SHADOW_SIZE = 6.0f;
	static constexpr int COUNT_NUM = 3;//�t���[���J�E���g�̐�
	//�v���C���[�̏��
	static constexpr unsigned int IDLE			  = (1 << 0);  //�ҋ@
	static constexpr unsigned int AVOID			  = (1 << 1);  //���
	static constexpr unsigned int DEATH			  = (1 << 2);  //�f�X
	static constexpr unsigned int BLOCK			  = (1 << 3);  //�u���b�N
	static constexpr unsigned int REACTION		  = (1 << 4);  //���A�N�V����
	static constexpr unsigned int BLOCK_REACTION  = (1 << 5);  //�u���b�N���A�N�V����
	static constexpr unsigned int WALK_FRONT	  = (1 << 6); //����
	static constexpr unsigned int SLASH			  = (1 << 7); //�U��
	static constexpr unsigned int HEAL			  = (1 << 8); //�U��
	//�}�X�N
	static constexpr unsigned int MASK_REACTION				 = BLOCK_REACTION | REACTION;//���A�N�V����
	static constexpr unsigned int MASK_MOVE					 = WALK_FRONT; //�ړ��}�X�N
	static constexpr unsigned int MASK_AVOID				 = AVOID; //�ړ��}�X�N
	static constexpr unsigned int MASK_CANT_RECOVERY_STAMINA = MASK_AVOID | BLOCK;
	static constexpr unsigned int MASK_ATTACK				 = SLASH;
	static constexpr unsigned int MASK_ALWAYS_TURN_OFF		 = MASK_MOVE | IDLE;
	static constexpr unsigned int MASK_ALL					 = MASK_MOVE | IDLE | MASK_ATTACK | MASK_REACTION | BLOCK | MASK_AVOID | HEAL;
	static constexpr unsigned int MASK_CAN_VELOCITY			 = MASK_MOVE | MASK_AVOID;
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
		INVINCIBLE = 0,//���G����
		HEAL = 1,//��
		AVOID = 2,//���
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
		SLASH			= 7,
		HEAL			= 8,
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
	void Heal			 ();
	//���t���O
	const bool CanRotation()const;
	const bool CanRolling()const;
	const bool CanAttack()const;
	const bool CanBlock()const;
	const bool CanHeal()const;
	const bool DontAnyAction()const;

		  void UpdateAnimation	 ();		//���݂̃A�j���[�V�����̍X�V
		  bool FrameCount(const int _index,const int _maxFrame);
	const bool CanAction(const float _staminaConsumed)const;
	void CalcStamina(const float _staminaConsumed);//�X�^�~�i�̉񕜏���
	/*�����o�ϐ�*/
	HitStop* hitStop;
	VECTOR				moveVectorRotation;			//�ړ��x�N�g���p��]�l
	std::vector<int>	frameCount;					//�t���[���J�E���g
	std::vector<bool>	isCount;					//�J�E���g�����邩
	std::map<unsigned int, int> animationMap;//�A�j���[�V�����}�b�v
	std::map<int, unsigned int> reactionMap;//���A�N�V�����}�b�v
	int					nowAnimation;				//�A�j���[�V����
	float				animationPlayTime;			//�A�j���[�V�����Đ�����
	int					attackComboCount;
	int					healOrbNum;					//�񕜃I�[�u�̐�
};

