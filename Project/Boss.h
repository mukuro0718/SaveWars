//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class BitFlag;
class Physics;
class Collidable;
class Animation;
class Boss : public GoriLib::Collidable
{
public:
	Boss();//�R���X�g���N�^
	~Boss();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics);		//������
	void		Finalize(GoriLib::Physics* _physics);		//�㏈��
	void		Update(GoriLib::Physics* _physics);		//�X�V
	void		OnCollide(const Collidable& _colider)override;//�Փ˂����Ƃ�
	const void	Draw()const;//�`��

	/*getter*/
	const int	 GetDamage		()const;									  //�_���[�W�̎擾
	const int	 GetHP			()const;				  //HP�̎擾
	const VECTOR GetDirection	()const;
	const VECTOR GetRotation	()const;
	const VECTOR GetHeadPosition()const;
	const VECTOR GetPosition	()const;
	const int	 GetHitNumber	()const { return this->attackNumber; }
	const bool	 IsAttack		()const;
		  void	 OnIsHitAttack	()		{ this->isHitAttack = true; }
	const int	 GetModelHandle	()const { return this->modelHandle; }
	const VECTOR GetVelocity	()const { return this->GetVelocity(); }

	/*setter*/
	void SetHitNumber(const int _attackNumber)	{ this->attackNumber = _attackNumber; }
	void SetVelocity (const VECTOR _velocity)	{ this->rigidbody.SetVelocity(_velocity); }
	void SetRotation (const VECTOR _rotation)	{ this->rigidbody.SetRotation(_rotation); }
	void SetSpeed	 (const float _speed)		{ this->speed = _speed; }
	/*��[�Պ֐�*/
	float  Lerp(const float _start, const float _end, const float _percent);	//��[�Պ֐�
	VECTOR Lerp(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//��[�Պ֐�
private:
	/*�t�@���N�V����*/
	typedef std::function<void(void)> FlagsState;//�t���O���Ƃ̎��s�������֐��i�����A�Ԃ�l�����j

	/*�ÓI�萔*/
	static constexpr int COUNT_NUM = 6;
	//��{���
	static constexpr unsigned int DYING	= (1 << 0);//�f�X
	static constexpr unsigned int IDLE	= (1 << 1);//�ҋ@
	static constexpr unsigned int ROAR	= (1 << 2);//���K
	static constexpr unsigned int WALK	= (1 << 3);//����
	static constexpr unsigned int REST	= (1 << 4);//�x�e
	//�U��
	static constexpr unsigned int SLASH			 = (1 << 5);//�p���`
	static constexpr unsigned int FLY_ATTACK	 = (1 << 6);//�n�ʂ�������
	static constexpr unsigned int HURRICANE_KICK = (1 << 7);//��]�R��
	static constexpr unsigned int JUMP_ATTACK	 = (1 << 8);//����
	static constexpr unsigned int ROTATE_PUNCH	 = (1 << 9);//�X���b�V��

	static constexpr unsigned int MASK_ATTACK = SLASH | ROTATE_PUNCH | JUMP_ATTACK |HURRICANE_KICK | FLY_ATTACK;
	static constexpr unsigned int MASK_ALL = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
	/*�N���X*/
	class FlagsStateSet
	{
	public:
		FlagsState update;
	};
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
		SLASH = 0,
		FLY_ATTACK = 1,
		HURRICANE_KICK = 2,
		JUMP_ATTACK = 3,
		ROTATE_PUNCH = 4,
		REST = 5,
	};
	enum class Phase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
	};
	enum class AttackType
	{
		NONE			 = -1,
		SLASH			 = 0,//�p���`
		FLY_ATTACK		 = 1,//�n�ʂ�������
		HURRICANE_KICK	 = 2,//��]�R��
		JUMP_ATTACK		 = 3,//�˂��h���U��
		ROTATE_PUNCH	 = 4,//��]�p���`
	};
	enum class AnimationType
	{
		DYING			= 0,//�f�X
		IDLE			= 1,//�ҋ@
		ROAR			= 2,//���K
		WALK			= 3,//����

		SLASH			= 4,//�X���b�V��
		FLY_ATTACK		= 5,//��эU��
		HURRICANE_KICK	= 6,//��]�R��
		JUMP_ATTACK		= 7,//�˂��h���U��
		ROTATE_PUNCH	= 8,//��]�p���`
	};

	/*���������֐�*/
		  void Roar				();//���K
		  void UpdateRotation	();//��]���̍X�V
		  void UpdateMoveVector	();//�ړ��x�N�g���̍X�V
		  void UpdateSpeed		();//�ړ��x�N�g���̍X�V
		  void Move				();//�ړ�
		  void Attack			();//�U��
		  void Rest				();
		  void Death			();
		  void Reaction			();
		  void ChangeState		();
		  bool FrameCount		(const int _index, const int _maxFrame);
	const bool CanAttack		()const;//�U���ł��邩
	const bool CanRotation		()const;//��]
	const bool CanMove			()const;//�ړ�
	const bool CanRest			()const;//�x�e�ł��邩
	const bool CanRoar			()const;//���K�ł��邩
		  void SetAttackComboCount();
		  void SetAttackCombo();
		  void SetPhase();
		  void AddItemFunction(const unsigned int _item, const FlagsState _update);//���ڂ��Ƃ̊֐��̒ǉ�
		  void OnEffectFlag(const int _attack);
		  void SetAttackFlag(const int _attack);
		  void SlowAnimationPlayTime(const FrameCountType _type, const int _targetCount, const float _maxTime);
		  void AddAnimationPlayTime(const FrameCountType _type, const int _targetCount, const float _maxTime);
		  void AttackSpeed(const int _type, const float _speed);

	/*�����o�ϐ�*/
	std::map<unsigned int, int>	stateAnimationMap;		//���ڂ��Ƃ̃A�j���[�V����
	std::vector<int>			frameCount;				//�t���[���J�E���g
	std::vector<bool>			isCount;				//�J�E���g�����邩
	std::vector<int>			attackCombo;			//�U���R���{
	Animation*					animation;				//�A�j���[�V����
	BitFlag*					state;					//���
	VECTOR						moveTarget;				//�ړ��ڕW
	float						speed;					//���x
	float						animationPlayTime;		//�A�j���[�V�����Đ�����
	bool						isGround;				//�n�ʂƓ���������
	bool						isHitAttack;			//�U��������������
	bool						isDraw;					//�`�悷�邩
	int							attackComboCount;		//�U���R���{��
	int							attackComboIndexOffset;	//�U���R���{�񐔂�ݒ肷�邽�߂̃C���f�b�N�X�I�t�Z�b�g
	int							attackNumber;			//�U���̔ԍ�
	int							nowAnimation;			//���݂̃A�j���[�V����
	int							attackType;				//�U���̎��
	int							nowPhase;				//���݂̃t�F�[�Y
	int							prevPhase;				//�O�̃t�F�[�Y
	int							modelHandle;			//���f���n���h��
};

