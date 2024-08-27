//===============================================
// @brief �v���C���[�N���X
//===============================================
#pragma once

class Model;
class BitFlag;
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
	const int GetAttackNumber()const { return this->attackNumber; }
	const int GetHitNumber()const { return this->hitNumber; }
	void SetHitNumber(const int _attackNumber) { this->hitNumber = _attackNumber; }
	void CalcDamage(const int _damage);
	const int		GetDamage	  ()const;									  //�_���[�W�̎擾
	const VECTOR	GetPosition		  ()const;									  //���W�̎擾
	const VECTOR	GetDirection	  ()const { return this->direction; }		  //�ړ��x�N�g���̎擾
	const bool		IsMove			  ()const;									  //�ړ��t���O�̎擾
	const bool		IsAttack		  ()const;									  //�V���b�g�t���O�̎擾
	void		FixMoveVector	  (const VECTOR _fixVector);				  //�ړ��x�N�g���̏C��
	const Collider	GetCharacterCollider		  ();										  //�R���C�_�[�̎擾
	const Collider	GetAttackCollider		  ();										  //�R���C�_�[�̎擾
	const int		GetHP			  ()const { return this->hp; }				  //HP�̎擾
private:
	

	/*�ÓI�萔*/
	static constexpr int COUNT_NUM = 4;//�t���[���J�E���g�̐�
	static constexpr int COLLIDER_NUM = 2;//�R���C�_�[�̐�
	//�v���C���[�̏��
	static constexpr unsigned int IDLE	   = (1 << 0); //�ҋ@
	static constexpr unsigned int REACTION = (1 << 1); //���A�N�V�����i�U�����󂯂����j
	static constexpr unsigned int DEATH    = (1 << 2); //���S
	static constexpr unsigned int LOCK_ON  = (1 << 3); //���b�N�I��
	//�ړ�
	static constexpr unsigned int RUN	  = (1 << 6); //����
	static constexpr unsigned int WALK	  = (1 << 7); //����
	//�A�N�V����
	static constexpr unsigned int AVOID	= (1 << 8); //���
	static constexpr unsigned int BLOCK = (1 << 9); //�u���b�N
	//���A�N�V����
	static constexpr unsigned int BIG_IMPACT   = (1 << 10); //��Ռ�
	static constexpr unsigned int SMALL_IMPACT = (1 << 11); //���Ռ�
	//�U��
	static constexpr unsigned int MAIN_ATTACK_1				 = (1 << 12); //��{�U���P
	static constexpr unsigned int MAIN_ATTACK_2				 = (1 << 13); //��{�U���P
	static constexpr unsigned int MAIN_ATTACK_3				 = (1 << 14); //��{�U���P
	static constexpr unsigned int SPECIAL_ATTACK			 = (1 << 15); //�R���{
	//�}�X�N
	static constexpr unsigned int MASK_MOVE = RUN | WALK; //�ړ��}�X�N
	static constexpr unsigned int MASK_REACTION = BIG_IMPACT | SMALL_IMPACT;//���A�N�V�����}�X�N
	static constexpr unsigned int MASK_ATTACK = MAIN_ATTACK_1 | MAIN_ATTACK_2 | MAIN_ATTACK_3 | SPECIAL_ATTACK;//�U���}�X�N
	static constexpr unsigned int MASK_ALWAYS_INITIALIZE = BLOCK | IDLE;
	static constexpr unsigned int MASK_CANT_AVOID = MASK_ATTACK | MASK_REACTION | BLOCK | LOCK_ON | AVOID;
	static constexpr unsigned int MASK_CANT_MAIN_ATTACK = MASK_ATTACK | MASK_REACTION | BLOCK | AVOID ;
	static constexpr unsigned int MASK_CANT_IDLE = MASK_ATTACK | MASK_REACTION | AVOID | BLOCK | DEATH | RUN | WALK;
	static constexpr unsigned int MASK_CANT_JUMP = MASK_ATTACK | MASK_REACTION | AVOID | BLOCK | DEATH;

	static constexpr unsigned int MASK_ALL = MASK_MOVE | MASK_ATTACK | AVOID | IDLE | LOCK_ON;
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
		SWITCH_MOVE_STATE = 0,
		ATTACK_INTERVAL = 1,//�U���ҋ@
		SWITCH_LOCK_ON = 2,//�U���ҋ@
		SWITCH_AVOID = 3,//���
	};
	enum class AttackType
	{
		NONE			 = -1,
		MAIN_1			 = 0,//�r��
		MAIN_2			 = 1,//�r��
		MAIN_3			 = 2,//�r��
		SPECIAL			 = 3,//�R���{
	};
	//�A�j���[�V�����̎��
	enum class AnimationType
	{
		IDLE				 = 0,//�����ҋ@
		IDLE_PLAY			 = 1,//�����ҋ@�V��
		WALK				 = 2,//����
		RUN					 = 3,//����
		LOCK_ON_RUN_BACK	 = 4,//���b�N�I����둖��
		LOCK_ON_RUN_LEFT	 = 5,//���b�N�I��������
		LOCK_ON_RUN_RIGHT	 = 6,//���b�N�I���E����
		LOCK_ON_WALK_BACK	 = 7,//���b�N�I��������
		LOCK_ON_WALK_LEFT	 = 8,//���b�N�I��������
		LOCK_ON_WALK_RIGHT	 = 9,//���b�N�I���E����
		BLOCK				 = 10,//�u���b�N
		AVOID				 = 11,//���
		DEATH				 = 12,//�f�X
		BIG_IMPACT			 = 13,//��Ռ�
		SMALL_IMPACT		 = 14,//���Ռ�
		MAIN_1				 = 15,//�r��
		MAIN_2				 = 16,//�r��
		SPECIAL				 = 17,//�R���{
	};

	/*���������֐�*/
		  void UpdateVelocity	 ();		//���x�̍X�V
		  void UpdateMoveVector	 ();		//�ړ��x�N�g���̍X�V
		  void UpdateRotation	 ();		//��]���̍X�V
		  void Move				 ();		//�ړ�
		  void LockOn			 ();		//���b�N�I��
		  void Block			 ();		//�u���b�N
		  void Avoid			 ();		//���
		  void Attack			 ();		//�U��
		  void Reaction();
		  void UpdateAnimation	 ();		//���݂̃A�j���[�V�����̍X�V
		  bool FrameCount(const int _index,const int _maxFrame);
	const bool CanRotation()const;
	const bool CanBlock()const;
	const bool CanAvoid()const;
	const bool CanAttack()const;
	const bool DontAnyAction()const;
	/*�����o�ϐ�*/

	Model*				model;						//���f��
	BitFlag*			state;						//���
	Collider*			collider[COLLIDER_NUM];		//�R���C�_�[�N���X
	VECTOR				moveVector;					//�ړ��x�N�g��
	VECTOR				direction;					//�����Ă������
	VECTOR				fixVector;					//�␳�x�N�g��
	VECTOR				moveVectorRotation;			//�ړ��x�N�g���p��]�l
	VECTOR				wasd;						//wasd����
	VECTOR				lStick;						//lStick����(��:Z+ ��:Z- ��:x- �E:x+)
	float				velocity;					//���x
	std::vector<int>	frameCount;					//�t���[���J�E���g
	std::vector<bool>	isCount;					//�J�E���g�����邩
	std::map<unsigned int, int> attackAnimationMap;//�U���A�j���[�V�����}�b�v
	std::map<int, unsigned int> attackComboStateMap;//�R���{�ɉ����čU���̃r�b�g�t���O��Ԃ�
	float				jumpPower;					//�W�����v��
	int					nowAnimation;				//�A�j���[�V����
	float				animationPlayTime;			//�A�j���[�V�����Đ�����
	int					hp;
	int damage;
	int attackComboCount;
	int hitNumber;
	int attackNumber;
	bool isDraw;
};

