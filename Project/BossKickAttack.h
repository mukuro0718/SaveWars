//===========================================
// @brief �p���`�R���{�Q�U��
//===========================================
#pragma once
class BossAttack;
class BossKickAttack : public BossAttack
{
public:
	BossKickAttack(const int _attackIndex);//�R���X�g���N�^
	~BossKickAttack();//�f�X�g���N�^
		  void Initialize() override;		//������
		  void Update	 () override;		//�X�V
	const void Draw		 () const override;	//�`��
protected:
};

