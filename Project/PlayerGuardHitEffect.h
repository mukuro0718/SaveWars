//=====================================
// @brief�@�K�[�h���q�b�g�G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerGuardHitEffect : public Effect
{
public:
	PlayerGuardHitEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerGuardHitEffect() {}//�f�X�g���N�^

	void Update() override;//�X�V
private:
	float	yOffset;			//Y�I�t�Z�b�g
	VECTOR	scale;				//�g�嗦
	int		playingEffectHandle;//�Đ��G�t�F�N�g�n���h��
};

