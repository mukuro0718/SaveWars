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
		COLL_ARENA,
		PLAYER,
		ENEMY,
		BOT,
		SKYDOME,
		BEAST,
		DRAGON,
	};
	//�摜�̎��
	enum class ImageType
	{		
		BOSS_HP_BAR_FRAME	= 0,
		HP_TABLE			= 1,
		BOSS_HP_BAR			= 2,
		PLAYER_HP_BAR		= 3,
		PLAYER_STAMINA_BAR	= 4,
		PRESS_BUTTON_LOGO	= 5,
		TITLE_LOGO			= 6,
		A_BUTTON			= 7,
		B_BUTTON			= 8,
		X_BUTTON			= 9,
		Y_BUTTON			= 10,
		LEFT_STICK			= 11,
		RIGHT_STICK			= 12,
		PUSH_RIGHT_STICK	= 13,
		RIGHT_TRIGGER		= 14,
		RIGHT_BUMPER		= 15,
		POTION				= 16,
		POTION_TABLE		= 17,
		BACK_GROUND			= 18,
		SHADOW				= 19,
		SELECT_BACK			= 20,
		SELECT_LOGO			= 21,
		QUEST_ICON_1		= 22,
		QUEST_ICON_2		= 23,
		QUEST_ICON_3		= 24,
		QUEST_ICON_4		= 25,
		QUEST_IMAGE_1		= 26,
		QUEST_IMAGE_2		= 27,
		QUEST_IMAGE_3		= 28,
		QUEST_IMAGE_4		= 29,
		QUEST_SUMMARY_1		= 30,
		QUEST_SUMMARY_2		= 31,
		QUEST_SUMMARY_3		= 32,
		QUEST_SUMMARY_4		= 33,
		SELECT_ICON_FRAME	= 34,
		SELECT_PROV_DECIDE = 35,
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
		BOSS_ROAR,
		BOSS_FLAME,
		PLAYER_GUARD_HIT,
		PLAYER_HEAL,
		PLAYER_IMPACT,
		PLAYER_JUST_GUARD,
		PLAYER_CHARGE_ATTACK,
		TMP_EFFECT,
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

