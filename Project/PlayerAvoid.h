//=============================================================
// @brief �v���C���[�_�b�V���A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerAvoid : public PlayerAction
{
public:
	 PlayerAvoid();	//�R���X�g���N�^
	~PlayerAvoid();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	bool isPlay;//���̃A�N�V�������s���Ă��邩
	int frameCount;
};

