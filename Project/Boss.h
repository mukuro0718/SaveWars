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
	const int		GetDamage()const;									  //�_���[�W�̎擾
	const int		GetHP()const;				  //HP�̎擾
	const VECTOR GetHeadPosition()const;
	const VECTOR GetPosition()const;
	const int GetAttackNumber()const { return this->attackNumber; }
	const int GetHitNumber()const { return this->hitNumber; }
	void SetHitNumber(const int _attackNumber) { this->hitNumber = _attackNumber; }
	const bool IsAttack()const;
	void OnIsHitAttack() { this->isHitAttack = true; }
private:
	/*�t�@���N�V����*/
	typedef std::function<void(void)> FlagsState;//�t���O���Ƃ̎��s�������֐��i�����A�Ԃ�l�����j

	/*�ÓI�萔*/
	static constexpr int COUNT_NUM = 2;
	static constexpr int COLLIDER_NUM = 2;//�R���C�_�[�̐�
	//��{���
	static constexpr unsigned int DYING	= (1 << 0);//�f�X
	static constexpr unsigned int IDLE	= (1 << 1);//�ҋ@
	static constexpr unsigned int ROAR	= (1 << 2);//���K
	static constexpr unsigned int WALK	= (1 << 3);//����
	static constexpr unsigned int REST	= (1 << 4);//�x�e
	//�t�F�[�Y�P
	static constexpr unsigned int PUNCH		  = (1 << 5);//�p���`
	static constexpr unsigned int SLASH		  = (1 << 6);//�X���b�V��
	static constexpr unsigned int THROW_STORN = (1 << 7);//����
	//�t�F�[�Y�Q
	static constexpr unsigned int HURRICANE_KICK = (1 << 8);//��]�R��
	static constexpr unsigned int GRAND_SLAM	 = (1 << 9);//�n�ʂ�������
	static constexpr unsigned int FLAME_MAGIC	 = (1 << 10);//�Ή�����
	static constexpr unsigned int LIGHTNING		 = (1 << 11);//�����_���ŗ���
	//�t�F�[�Y�R
	static constexpr unsigned int CONTINUOUS_SLASH = (1 << 12);//�A���X���b�V��
	static constexpr unsigned int COMBO			   = (1 << 12);//�R���{�i�����_���ȍU����g�ݍ��킹��j
	static constexpr unsigned int DARK_FIELD	   = (1 << 13);//�_�[�N�t�B�[���h�i�͈͓��Ƀ_���[�W�j
	static constexpr unsigned int METEO			   = (1 << 14);//覐�

	static constexpr unsigned int MASK_ATTACK = PUNCH | SLASH | THROW_STORN |HURRICANE_KICK | GRAND_SLAM |FLAME_MAGIC | LIGHTNING 
											   | CONTINUOUS_SLASH | COMBO | DARK_FIELD | METEO;
	static constexpr unsigned int MASK_ALL = MASK_ATTACK | WALK | DYING | REST | REST | IDLE;
	
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
		REST = 0,
		ATTACK = 1,
	};
	enum class AttackType
	{
		NONE			 = -1,
		PUNCH			 = 0,//�p���`
		SLASH			 = 1,//�X���b�V��
		THROW_STORN		 = 2,//����
		HURRICANE_KICK	 = 3,//��]�R��
		GRAND_SLAM		 = 4,//�n�ʂ�������
		FLAME_MAGIC		 = 5,//�Ή�����
		LIGHTNING		 = 6,//����
		CONTINUOUS_SLASH = 7,//�A���؂�
		DARK_FIELD		 = 8,//�_�[�N�t�B�[���h
		METEO			 = 9,//覐�
	};
	enum class AnimationType
	{
		DYING			 = 0,//�f�X
		IDLE			 = 1,//�ҋ@
		ROAR			 = 2,//���K
		WALK			 = 3,//����
		PUNCH			 = 4,//�p���`
		SLASH			 = 5,//�X���b�V��
		THROW_STORN		 = 6,//����
		HURRICANE_KICK	 = 7,//��]�R��
		GRAND_SLAM		 = 8,//�n�ʂ�������
		FLAME_MAGIC		 = 9,//�Ή�����
		LIGHTNING		 = 10,//����
		CONTINUOUS_SLASH = 11,//�A���؂�
		DARK_FIELD		 = 12,//�_�[�N�t�B�[���h
		METEO			 = 13,//覐�
	};

	/*���������֐�*/
		  void Idle				(){}
		  void Taunt			();//���K
		  void UpdateRotation	();//��]���̍X�V
		  void UpdateMoveVector	();//�ړ��x�N�g���̍X�V
		  void UpdateVelocity	();//�ړ��x�N�g���̍X�V
		  void Move				();//�ړ�
		  void Attack			();//�U��
		  void Rest				();
		  void Death			();
		  void Reaction			();
		  void DecideOfAttack	();//�U�������߂�
		  void RushAttack		();//�ːi�U��
		  void JumpAttack		();//�W�����v�U��
		  void ChangeState		();
		  bool FrameCount		(const int _index, const int _maxFrame);
	unsigned int GetState		();
	const bool CanAttack		()const;//�U���ł��邩
	const bool CanRotation		()const;//��]
	const bool CanMove			()const;//�ړ�
	const bool CanRest			()const;//�x�e�ł��邩
	void AddItemFunction(const unsigned int _item, const FlagsState _update);//���ڂ��Ƃ̊֐��̒ǉ�
	
	/*�����o�ϐ�*/
	Animation*					animation;					//���f��
	BitFlag*					state;					//���
	float						speed;				//���x
	VECTOR						moveVector;				//�ړ��x�N�g��
	VECTOR						direction;
	VECTOR						moveTarget;				//�ړ��ڕW
	int							stamina;				//�X�^�~�i
	std::map<unsigned int, FlagsStateSet>stateFunctionMap;//���ڂ��Ƃ̊֐�
	std::map<unsigned int, int> stateAnimationMap;//���ڂ��Ƃ̃A�j���[�V����
	std::map<int, unsigned int> phase1AttackMap;
	std::map<int, unsigned int> phase2AttackMap;
	std::vector<int>	frameCount;					//�t���[���J�E���g
	std::vector<bool>	isCount;					//�J�E���g�����邩

	int modelHandle;
	int nowAnimation;
	float animationPlayTime;
	float dot;
	bool isGround;
	float targetRange;
	int attackComboCount;//�U���R���{��
	float jumpPower;
	bool isHitAttack;
	int hitNumber;
	int attackNumber;
	bool isDraw;
	int attackType;

};

