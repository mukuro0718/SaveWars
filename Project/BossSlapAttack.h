//===========================================
// @brief �X���b�v�U��
//===========================================
#pragma once

class BossAttack;
class BossSlapAttack : public BossAttack
{
public:
	BossSlapAttack(const int _attackIndex);//�R���X�g���N�^
	~BossSlapAttack();//�f�X�g���N�^

	void Initialize() override;		//������
	void Update() override;		//�X�V
	const void Draw() const override;	//�`��
};

