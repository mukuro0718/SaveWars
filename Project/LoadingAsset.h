//================================================================
// @brief �A�Z�b�g�̃��[�h�N���X
//================================================================
#pragma once
#include "Singleton.h"

class LoadingAsset : public Singleton<LoadingAsset>
{
public:
	friend class Singleton<LoadingAsset>;

	/*�񋓑�*/
	//���f���̎��
	enum class ModelType
	{
		FINALY_BOSS_STAGE,
		PLAYER,
		ENEMY,
		BOT,
		SKYDOME,
	};
	//�摜�̎��
	enum class ImageType
	{
		EMPTY_ORB,
		HEAL_ICON,
		HP_ORB,
		PAUSE_ICON,
		
		BOSS_HP_BAR,
		PLAYER_HP_BAR,
		PLAYER_STAMINA_BAR,
		BAR_TABLE,

		GAME_CLEAR,
		GAME_OVER,
		PRESS_A,
		TITLE_LOGO,
		WINDOW,
		CONTINUE,
		END,
		NONE_STAR,
		SILVER_STAR,
		GOLD_STAR,

		B_BUTTON,
		X_BUTTON,
		Y_BUTTON,
		LT_BUTTON,
		PRESS_B_BUTTON,
		PRESS_X_BUTTON,
		PRESS_Y_BUTTON,
		PRESS_LT_BUTTON,
		SHADOW,
		BACK_GROUND,
		POTION,
		POTION_TABLE,
		HP_BAR_FRAME,
		A_BUTTON,
		PRESS_A_BUTTON,

		MUTANT_NEW_TEXTURE,
		MUTANT_PREV_TEXTURE,
	};
	//�t�H���g�̃^�C�v
	enum class FontType
	{
		MINTYO_80_32,
		MINTYO_50_32,
		MINTYO_100_32,
	};
	enum class FontName
	{
		batman,
		Aihara,
	};
	//���̃^�C�v
	enum class SoundType
	{

	};
	//�G�t�F�N�g
	enum class EffectType
	{
		BOSS_IMPACT,
		BOSS_FLAME,
		PLAYER_GUARD_HIT,
		PLAYER_HEAL,
		PLAYER_IMPACT,
		PLAYER_JUST_GUARD,
	};

	/*getter*/
	const int GetModel(const ModelType _index) { return this->modelHandle[static_cast<int>(_index)]; }//���f���̎擾
	const int GetImage(const ImageType _index) { return this->imageHandle[static_cast<int>(_index)]; }//�摜�̎擾
	const int GetFont (const FontType _index)  { return this->fontHandle [static_cast<int>(_index)]; }//�t�H���g�̎擾
	const int GetSound(const SoundType _index) { return this->soundHandle[static_cast<int>(_index)]; }//�T�E���h�n���h���̎擾
	const int GetEffect(const EffectType _index) { return this->effectHandle[static_cast<int>(_index)]; }//�G�t�F�N�g�n���h���̎擾
private:
	/*���������֐�*/
			 LoadingAsset();//�R���X�g���N�^
	virtual ~LoadingAsset();//�f�X�g���N�^

	std::vector<int> modelHandle;//���f���n���h��
	std::vector<int> imageHandle;//�摜�n���h��
	std::vector<int> fontHandle ;//�t�H���g�n���h��
	std::vector<int> soundHandle;//�T�E���h�n���h��
	std::vector<int> effectHandle;//�G�t�F�N�g�n���h��
};

