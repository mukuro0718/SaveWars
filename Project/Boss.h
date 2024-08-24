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
	void		Move();		//�ړ�
	void		Update();	//�X�V
	const void	Draw()const;//�`��

	const VECTOR GetPosition()const;
	const Collider GetCollider();
private:
	/*�ÓI�萔*/
	static constexpr int IDLE		= (1 >> 0);//�ҋ@
	static constexpr int TAUNT		= (1 >> 1);//���K
	static constexpr int WALK		= (1 >> 2);//����
	static constexpr int MASK_ALL = IDLE | TAUNT | WALK;

	/*�񋓑�*/
	enum class AnimationType
	{
		RUSH = 0,
		TAUNT = 1,
		WALK = 2,
	};

	/*���������֐�*/
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

