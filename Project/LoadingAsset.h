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
		SATELLITE,
		SHIELD,
		ENEMY,
		ENEMY_BODY,
		SKYDOME,
	};
	//�摜�̎��
	enum class ImageType
	{
		B,
		X,
		Y,
		LB,
		LS,
		LT,
		RB,
		RS,
		GAME_CLEAR,
		GAME_OVER,
		PRESS_A,
		TITLE_LOGO
	};
	//�t�H���g�̃^�C�v
	enum class FontType
	{
		ICON_UI,
		OPERATION_UI,
	};
	enum class FontName
	{
		Honoka,
		batman
	};
	//���̃^�C�v
	enum class SoundType
	{

	};
	//�G�t�F�N�g
	enum class EffectType
	{
		BOSS_PUNCH,
		BOSS_SLASH,
		BOSS_THROW,
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

