//===========================================
// @brief ��]�p���`�U��
//===========================================
#pragma once
class BossAttack;
class BossRotatePunch : public BossAttack
{
public:
	BossRotatePunch(const int _attackIndex);//�R���X�g���N�^
	~BossRotatePunch();//�f�X�g���N�^

		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override; //�`��
protected:
};

