//===========================================
// @brief �X���b�V���U��
//===========================================
#pragma once

class BossAttack;
class BossSlashAttack : public GoriLib::BossAttack
{
public:
	BossSlashAttack(const int _attackIndex);//�R���X�g���N�^
	~BossSlashAttack();//�f�X�g���N�^

		  void Initialize(GoriLib::Physics* _physics)override;//������
		  void Update	 (GoriLib::Physics* _physics)override;//�X�V
	const void Draw		 ()const;							  //�`��
protected:
	/*���������֐�*/
	void		SetRadius(const float _radius) override;//���a�̃Z�b�g
	const float GetRadius() const			   override;//���a�̎擾

	/*�����o�ϐ�*/
};
