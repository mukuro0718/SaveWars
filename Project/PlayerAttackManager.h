//===============================================
// @brief �v���C���[�U���}�l�[�W���[�N���X
//===============================================
#pragma once

class PlayerAttack;
class PlayerAttackManager
{
public:
	 PlayerAttackManager();//�R���X�g���N�^
	~PlayerAttackManager();//�f�X�g���N�^

	void		Initialize();					//������
	void		Finalize  ();					//�㏈��
	void		Update	  ();					//�X�V
	const void	Draw	  ()const;				//�`��

	/*setter*/
	void OnIsStart ();					 //�J�n�t���O�𗧂Ă�
	void SetDamage (const float _damage);//�_���[�W�̃Z�b�g
private:
	/*�����o�ϐ�*/
	PlayerAttack* attack;//�U���N���X
};

