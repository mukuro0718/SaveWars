//=====================================
// @brief�@�{�X���G�t�F�N�g
//=====================================
#pragma once

class Effect;
class BossFlameEffect : public Effect
{
public:
	BossFlameEffect(const int _effectResourceHandle);//�R���X�g���N�^
	~BossFlameEffect() {}//�f�X�g���N�^

	void Update();
	void SetInfoToPlayTheEffect() override;
	void SetPosition(const VECTOR _position);
private:
};

