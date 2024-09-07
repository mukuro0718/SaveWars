#pragma once
#include "Singleton.h"

class Effect;
class EffectManager : public Singleton<EffectManager>
{
public:
	friend class Singleton<EffectManager>;

	/*�G�t�F�N�g�̕`��p�֐�*/
	void OnIsBossSlashEffect();
	void OnIsBossRotatePunchEffect();
	void OnIsBossJumpAttackEffect();
	void Update();
	const void Draw()const;
private:
	/*�ÓI�萔*/
	static constexpr int PLAYER_EFFECT = 1;
	/*���������֐�*/
	EffectManager();//�R���X�g���N�^
	~EffectManager();//�f�X�g���N�^
	const VECTOR Convert(std::vector<float> _in)const;
	/*�񋓑�*/
	enum class BossEffect
	{
		SLASH,
		ROTATE_PUNCH,
		JUMP_ATTACK
	};

	/*�����o�ϐ�*/
	std::vector<Effect*> boss;
};

