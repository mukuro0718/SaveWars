//=====================================
// @brief�@�v���C���[�̗��ߍU���G�t�F�N�g
//=====================================
#pragma once

class Effect;
class PlayerChargeAttackEffect : public Effect
{
public:
	PlayerChargeAttackEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~PlayerChargeAttackEffect() {}//�f�X�g���N�^

	void SetInfoToPlayTheEffect() override;
private:
};

