#pragma once
#include "Singleton.h"

class Effect;
class EffectManager : public Singleton<EffectManager>
{
public:
	friend class Singleton<EffectManager>;
	/*�񋓑�*/
	enum class EffectType
	{
		BOSS_SLASH,
		BOSS_ROTATE_PUNCH,
		BOSS_ENTRY,
		PLAYER_ENTRY,
	};
	/*�G�t�F�N�g�̕`��p�֐�*/
	void OnIsEffect(const EffectType _type);
	void Update();
	const void Draw()const;
private:
	/*�ÓI�萔*/
	static constexpr int PLAYER_EFFECT = 1;
	/*���������֐�*/
	EffectManager();//�R���X�g���N�^
	~EffectManager();//�f�X�g���N�^
	const VECTOR Convert(std::vector<float> _in)const;
	

	/*�����o�ϐ�*/
	std::vector<Effect*> effect;
};

