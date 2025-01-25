#pragma once
#include "Singleton.h"

class Sound;
class SoundEffect;
class Bgm;
class SoundManager : public Singleton<SoundManager>
{
public:
	/*EffectManager��Singleton���t�����h��*/
	friend class Singleton<SoundManager>;


	/*�񋓑�*/
	//BGM�̎��
	enum class BgmType
	{
		TITLE_BGM,
	};
	//�G�t�F�N�g�̎��
	enum class EffectType
	{
		CONFIRMATION_SOUND,
		CANCEL_SOUND,
		CHANGE_SELECT_ICON,
		PLAYER_COMBO_1,
		PLAYER_COMBO_2,
		PLAYER_COMBO_3,
		PLAYER_S_ATTACK,
	};

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��
	
	void OnIsPlayBgm(const BgmType _type);		//�G�t�F�N�g�Đ��t���O�𗧂Ă�
	void OnIsPlayEffect(const EffectType _type);
	void OffIsPlayBgm(const BgmType _type);		//�G�t�F�N�g�Đ��t���O������
	void OffIsPlayEffect(const EffectType _type);
private:
	/*���������֐�*/
	 SoundManager();//�R���X�g���N�^
	~SoundManager();//�f�X�g���N�^
		
	/*�����o�ϐ�*/
	vector<Bgm*>		bgms;	//BGM
	vector<SoundEffect*>effects;	//�T�E���h�G�t�F�N�g

	int frameTime;//�����܂łɂ�����������
};

