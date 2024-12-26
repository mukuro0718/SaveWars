#pragma once
#include "Singleton.h"

class SoundManager : public Singleton<SoundManager>
{
public:
	/*EffectManager��Singleton���t�����h��*/
	friend class Singleton<SoundManager>;


	/*�񋓑�*/
	//�G�t�F�N�g�̎��
	enum class EffectType
	{
		PLAYER_COMBO_1,
		PLAYER_COMBO_2,
		PLAYER_COMBO_3,
		PLAYER_S_ATTACK,
	};

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��
	
	void OnIsPlay(const EffectType _type, const bool isTop);							//�G�t�F�N�g�Đ��t���O�𗧂Ă�
private:
	/*���������֐�*/
	 SoundManager();//�R���X�g���N�^
	~SoundManager();//�f�X�g���N�^
		
	/*�����o�ϐ�*/
	std::vector<int> sound;//�T�E���h�n���h��
	std::vector<bool> isPlay;//�Đ����邩
	std::vector<int> playType;//�Đ��g��
	std::vector<bool> isTopPosition;//�Đ��ʒu�����f�[�^�̐擪�Ɉړ����邩�ǂ���

	int frameTime;//�����܂łɂ�����������
};

