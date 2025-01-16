//=====================================
// @brief�@�v���C���[�̍U���q�b�g�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerImpactEffect : public Effect
{
public:
	PlayerImpactEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerImpactEffect() {}//�f�X�g���N�^

		  void Update() override;		//�X�V
	const void Draw	 () const override;	//�`��
private:
	/*�萔*/
	const int MAX_EFFECT_NUM;//�G�t�F�N�g�̍ő吔

	/*�����o�ϐ�*/
	VECTOR		 scale;				 //�g�嗦
	vector<int>	 playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
	vector<int>	 frameCount;//�Đ��G�t�F�N�g�n���h��
	vector<bool> isPlay;			 //�Đ��t���O
};
