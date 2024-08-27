//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Model;
class BitFlag;
class Collider;
class Boss
{
public:
	Boss();//�R���X�g���N�^
	~Boss();//�f�X�g���N�^

	void Initialize();		//������
	void		Action();
	void		Update();	//�X�V
	const void	Draw()const;//�`��
	void CalcDamage(const int _damage) { this->hp -= _damage; }
	const int		GetDamage()const;									  //�_���[�W�̎擾
	const int		GetHP()const { return this->hp; }				  //HP�̎擾

	const VECTOR GetPosition()const;
	const Collider GetCharacterCollider();
	const Collider GetAttackCollider();
	const int GetAttackNumber()const { return this->attackNumber; }
	const int GetHitNumber()const { return this->hitNumber; }
	void SetHitNumber(const int _attackNumber) { this->hitNumber = _attackNumber; }
	const bool IsAttack()const;
	void OnIsHitAttack() { this->isHitAttack = true; }
private:
	/*�t�@���N�V����*/
	typedef std::function<void(void)> FlagsState;//�t���O���Ƃ̎��s�������֐��i�����A�Ԃ�l�����j

	/*�ÓI�萔*/
	static constexpr int COUNT_NUM = 1;
	static constexpr int COLLIDER_NUM = 2;//�R���C�_�[�̐�

	static constexpr unsigned int IDLE						 = (1 << 0);//�ҋ@
	static constexpr unsigned int TAUNT						 = (1 << 1);//���K
	static constexpr unsigned int WALK_FRONT			 	 = (1 << 2);//�O����
	static constexpr unsigned int WALK_LEFT					 = (1 << 3);//������
	static constexpr unsigned int WALK_RIGHT				 = (1 << 4);//�E����

	static constexpr unsigned int VERTICAL_SLASH			 = (1 << 5);//�c�؂�
	static constexpr unsigned int HORIZONTAL_SLASH			 = (1 << 6);//���؂�
	static constexpr unsigned int ROTATION_SLASH			 = (1 << 7);//��]�؂�
	static constexpr unsigned int KNOCK_UP_SLASH			 = (1 << 8);//�������؂�
	static constexpr unsigned int STRONG_HORIZONTAL_SLASH	 = (1 << 9);//�������؂�
	static constexpr unsigned int TWO_COMBO					 = (1 << 10);//�P�Q�R���{
	static constexpr unsigned int THREE_COMBO				 = (1 << 11);//�P�Q�R�R���{
	static constexpr unsigned int REACTION					 = (1 << 12);//��_���[�W

	static constexpr unsigned int DEATH						 = (1 << 13);//�f�X
	static constexpr unsigned int REST						 = (1 << 14);//�x�e

	static constexpr unsigned int MASK_MOVE = WALK_FRONT | WALK_LEFT | WALK_RIGHT;
	static constexpr unsigned int MASK_ATTACK = VERTICAL_SLASH | HORIZONTAL_SLASH | ROTATION_SLASH |
												KNOCK_UP_SLASH | STRONG_HORIZONTAL_SLASH |
												TWO_COMBO | THREE_COMBO;
	static constexpr unsigned int MASK_ALL = MASK_MOVE | MASK_ATTACK | REACTION | DEATH | REST | TAUNT | IDLE;
	
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
	};
	enum class AttackType
	{
		NONE					= -1,
		VERTICAL_SLASH			= 0,//�c�؂�
		HORIZONTAL_SLASH		= 1,//���؂�
		ROTATION_SLASH			= 2,//��]�؂�
		KNOCK_UP_SLASH			= 3,//�������؂�
		STRONG_HORIZONTAL_SLASH = 4,//�������؂�
		TWO_COMBO				= 5,//�P�Q�R���{
		THREE_COMBO				= 6,//�P�Q�R�R���{
	};
	enum class AnimationType
	{
		IDLE					= 0,//�ҋ@
		TAUNT					= 1,//���K
		WALK_FRONT				= 2,//�O����
		WALK_LEFT				= 3,//������
		WALK_RIGHT				= 4,//�E����
		VERTICAL_SLASH			= 5,//�c�؂�
		HORIZONTAL_SLASH		= 6,//���؂�
		ROTATION_SLASH			= 7,//��]�؂�
		KNOCK_UP_SLASH			= 8,//�������؂�
		STRONG_HORIZONTAL_SLASH = 9,//�������؂�
		TWO_COMBO				= 10,//�P�Q�R���{
		THREE_COMBO				= 11,//�P�Q�R�R���{
		REACTION = 12,
		DEATH = 13,
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
	Model*						model;					//���f��
	BitFlag*					state;					//���
	Collider*					collider[COLLIDER_NUM];	//�R���C�_�[
	float						velocity;				//���x
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

	int nowAnimation;
	float animationPlayTime;
	float dot;

	float targetRange;
	int attackComboCount;//�U���R���{��
	float jumpPower;
	bool isHitAttack;
	int hp;
	int damage;
	int hitNumber;
	int attackNumber;
	bool isDraw;
};

