//===========================================
// @brief �p���`�U���R���{�R�U��
//===========================================
#pragma once

class BossAttack;
class BossMeleeCombo3Attack : public BossAttack
{
public:
	BossMeleeCombo3Attack(const int _attackIndex);//�R���X�g���N�^
	~BossMeleeCombo3Attack();//�f�X�g���N�^

	void Initialize() override;		//������
	void Update() override;		//�X�V
	const void Draw() const override;	//�`��
private:
	int doHitCheckFrameIndex;
};

