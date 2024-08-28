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
		BOSS_HP_CENTER,
		BOSS_HP_RIGHT,
		BOSS_HP_LEFT,
		BOSS_HP_CENTER_HOLDER,
		BOSS_HP_RIGHT_HOLDER,
		BOSS_HP_LEFT_HOLDER,
		BOSS_HP_ICON_HOLDER,
		PLAYER_HP_CENTER,
		PLAYER_HP_RIGHT,
		PLAYER_HP_LEFT,
		PLAYER_HP_CENTER_HOLDER,
		PLAYER_HP_RIGHT_HOLDER,
		PLAYER_HP_LEFT_HOLDER,
		PLAYER_HP_ICON_HOLDER,
		STAMINA_CENTER,
		STAMINA_RIGHT,
		STAMINA_LEFT,
		STAMINA_CENTER_HOLDER,
		STAMINA_RIGHT_HOLDER,
		STAMINA_LEFT_HOLDER,
		STAMINA_ICON_HOLDER,
		HP_ICON,
		STAMINA_ICON,
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
	};
	//�摜�̃^�C�v
	enum class SoundType
	{

	};

	/*getter*/
	const int GetModel(const ModelType _index) { return this->modelHandle[static_cast<int>(_index)]; }//���f���̎擾
	const int GetImage(const ImageType _index) { return this->imageHandle[static_cast<int>(_index)]; }//�摜�̎擾
	const int GetFont (const FontType _index)  { return this->fontHandle [static_cast<int>(_index)]; }//�t�H���g�̎擾
	const int GetSound(const SoundType _index) { return this->soundHandle[static_cast<int>(_index)]; }//�T�E���h�n���h���̎擾
private:
	/*���������֐�*/
			 LoadingAsset();//�R���X�g���N�^
	virtual ~LoadingAsset();//�f�X�g���N�^

	std::vector<int> modelHandle;//���f���n���h��
	std::vector<int> imageHandle;//�摜�n���h��
	std::vector<int> fontHandle ;//�t�H���g�n���h��
	std::vector<int> soundHandle;//�T�E���h�n���h��
};

