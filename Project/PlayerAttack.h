//===========================================
// @brief �v���C���[�U��
//===========================================
#pragma once

class ColliderData;
class PlayerAttack
{
public:
	PlayerAttack();//�R���X�g���N�^
	~PlayerAttack();//�f�X�g���N�^

		  void Initialize	();													//������
		  void Finalize		();													//�㏈��
		  void Update		(const VECTOR _position, const VECTOR _direction);	//�X�V
	const void	Draw		()const;											//�`��

	/*setter/getter*/
		  void OnIsStart	() { this->isStartHitCheck = true; this->isNotOnHit = false;}//�����蔻��J�n�t���O�𗧂Ă�
		  void SetDamage	(const float _damage);										 //�_���[�W�̐ݒ�
	const bool GetIsStart	()const { return this->isStartHitCheck; }					 //�����蔻��J�n�t���O���擾
private:


	/*�����o�ϐ�*/
	ColliderData* collider;			//�R���C�_�[
	bool		  isStartHitCheck;	//�q�b�g�`�F�b�N
	bool		  isNotOnHit;		//�����蔻��t���O�������Ă��邩
	int			  frameCount;		//�t���[���J�E���g
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};

