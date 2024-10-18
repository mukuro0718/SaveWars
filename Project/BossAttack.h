//===========================================
// @brief �{�X�U��
//===========================================
#pragma once

class ColliderData;
class BossAttack abstract
{
public:
	BossAttack();//�R���X�g���N�^
	virtual ~BossAttack();//�f�X�g���N�^

	virtual void		Initialize () abstract;		//������
	virtual void		Finalize   ();				//�㏈��
	virtual void		Update	   () abstract;		//�X�V
	virtual const void	Draw	   () const abstract;//�`��

	/*getter/setter*/
	void OnIsStart() { this->isStartHitCheck = true; this->isNotOnHit = false; }	//�����蔻��J�n�t���O�𗧂Ă�
	const VECTOR GetPosition() const;					//���W�̎擾
	const bool GetIsHitAttack();
	void OffIsHitAttack();

	/*�U���̎��*/
	enum class AttackType
	{
		NONE			= -1,
		SLASH_1			= 0, //�X���b�V��
		SLASH_2			= 1, //�X���b�V��
		STAB			= 2, //�˂��h���U��
		ROTATE_SLASH	= 3, //��]�X���b�V��
		PUNCH			= 4, //�p���`
		SLASH_COMBO_1	= 5, //�X���b�V���R���{�P
		SLASH_COMBO_2	= 6, //�X���b�V���R���{�Q
		JUMP_ATTACK		= 7, //�W�����v�A�^�b�N
	};
protected:
	/*���������֐�*/
	const VECTOR Convert(std::vector<float> _in);
	/*�����o�ϐ�*/
	ColliderData* collider;			//�R���C�_�[
	int			  frameCount;		//�t���[���J�E���g
	int			  attackIndex;		//�U���ԍ�
	bool		  isNotOnHit;		//�������ɓ����蔻�肪�I���ɂȂ�����
	bool		  isStartHitCheck;	//�����蔻����s����
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};
