//=============================================================
// @brief �v���C���[���݃A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerStagger : public PlayerAction
{
public:
	 PlayerStagger();	//�R���X�g���N�^
	~PlayerStagger();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	int frameCount;
};

