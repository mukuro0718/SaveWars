//=====================================
// @brief�@�W���X�g�K�[�h���G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerJustGuardEffect : public Effect
{
public:
	PlayerJustGuardEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerJustGuardEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

