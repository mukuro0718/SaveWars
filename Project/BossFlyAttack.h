//===========================================
// @brief �X���b�V���U��
//===========================================
#pragma once
class BossAttack;
class BossFlyAttack : public GoriLib::BossAttack
{
public:
	BossFlyAttack(const int _attackIndex);//�R���X�g���N�^
	~BossFlyAttack();//�f�X�g���N�^

	void Initialize(GoriLib::Physics* _physics)override;//������
	void Update(GoriLib::Physics* _physics)override;//�X�V
	const void Draw()const;							  //�`��
protected:
	/*���������֐�*/
	void		SetRadius(const float _radius) override;//���a�̃Z�b�g
	const float GetRadius() const			   override;//���a�̎擾
};

