//===========================================
// @brief ��]�R��U��
//===========================================
#pragma once

class BossAttack;
class BossHurricaneKick : public GoriLib::BossAttack
{
public:
	BossHurricaneKick(const int _attackIndex);//�R���X�g���N�^
	~BossHurricaneKick();//�f�X�g���N�^

	void Initialize(GoriLib::Physics* _physics)override;//������
	void Update(GoriLib::Physics* _physics)override;//�X�V
	const void Draw()const;							  //�`��
protected:
	/*���������֐�*/
	void		SetRadius(const float _radius) override;//���a�̃Z�b�g
	const float GetRadius() const			   override;//���a�̎擾

};

