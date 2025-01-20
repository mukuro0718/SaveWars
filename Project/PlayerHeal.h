//=============================================================
// @brief �v���C���[�񕜃A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class PlayerHeal : public PlayerAction
{
public:
	 PlayerHeal();	//�R���X�g���N�^
	~PlayerHeal();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	/*�萔*/
	const int HEAL_VALUE;		//�񕜗�
	const int MAX_HP;			//�ő�HP
	const int DO_HEAL_PLAY_TIME;//�񕜂���t���[������
	const int MAX_HEAL_VALUE;	//�񕜂���t���[������

	/*�����o�ϐ�*/
	int nowTotalHealValue;//���݂̑��񕜗�
};

