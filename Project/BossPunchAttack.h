//===========================================
// @brief �p���`�U��
//===========================================
#pragma once

class BossAttack;
class BossPunchAttack : public BossAttack
{
public:
	BossPunchAttack(const int _attackIndex);//�R���X�g���N�^
	~BossPunchAttack();//�f�X�g���N�^

	void Initialize() override;		//������
	void Update(const float _playTime) override;		//�X�V
	const void Draw() const override;	//�`��
};

