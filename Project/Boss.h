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
	void		Move();		//�ړ�
	void		Update();	//�X�V
	const void	Draw()const;//�`��

	const VECTOR GetPosition()const;
	const Collider GetCollider();
private:
	/*�ÓI�萔*/
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
	static constexpr unsigned int STRONG_TWO_COMBO			 = (1 << 11);//�����P�Q�R���{���؂�
	static constexpr unsigned int THREE_COMBO				 = (1 << 12);//�P�Q�R�R���{
	static constexpr unsigned int REACTION					 = (1 << 13);
	static constexpr unsigned int DEATH						 = (1 << 14);

	static constexpr unsigned int MASK_ATTACK = VERTICAL_SLASH | HORIZONTAL_SLASH | ROTATION_SLASH |
												KNOCK_UP_SLASH | STRONG_HORIZONTAL_SLASH |
												TWO_COMBO | STRONG_TWO_COMBO | THREE_COMBO;
	/*�񋓑�*/
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
		STRONG_TWO_COMBO		= 11,//�����P�Q�R���{���؂�
		THREE_COMBO				= 12,//�P�Q�R�R���{
		REACTION = 13,
		DEATH = 14,
	};

	/*���������֐�*/
	void Taunt();//���K
	void UpdateRotation();								//��]���̍X�V
	void UpdateMoveVector();
	void Attack();								//�U��
	const bool CanMove()const;						//�ړ��ł��邩
	const bool CanAttack()const;						//�U���ł��邩
	void DecideOfAttack();//�U�������߂�
	void RushAttack();//�ːi�U��
	void JumpAttack();//�W�����v�U��
	void SetState();
	bool IsRest();

	const bool CanRotation()const;
	
	/*�����o�ϐ�*/
	std::map<unsigned int, std::function<void(void)>>	attackFunctionMap;//��Ԃ��Ƃ̏���X�^�~�i
	std::map<int, unsigned int> attackTypeMap;
	Model*						model;					//���f��
	BitFlag*					state;					//���
	Collider*					collider;				//�R���C�_�[
	float						velocity;				//���x
	VECTOR						moveVector;				//�ړ��x�N�g��
	VECTOR						moveTarget;				//�ړ��ڕW
	int							stamina;				//�X�^�~�i
	float targetRange;
	int waitingCountBeforeAttack;//�U���O�ҋ@�J�E���g
	int restCount;//�x�e�J�E���g
	int attackCount;//�U���J�E���g
	bool isRest;
	int attackType;
	float jumpPower;

};

