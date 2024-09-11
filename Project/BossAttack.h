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
protected:
	/*���������֐�*/
	const VECTOR Convert(std::vector<float> _in);
	/*�����o�ϐ�*/
	ColliderData* collider;
	int			  frameCount;			 //�t���[���J�E���g
	int			  attackIndex;		 //�U���ԍ�
	bool		  isNotOnHit;
	bool		  isStartHitCheck;	 //�����蔻����s����
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};
