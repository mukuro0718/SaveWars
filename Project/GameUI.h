//===============================================
// @brief �Q�[��UI�N���X
//===============================================
#pragma once

class SceneUI;
class Image;
class HPUI;
class ButtonUI;
class BossNameUI;
class GameUI : public SceneUI
{
public:
	GameUI();
	~GameUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*�񋓑�*/
	enum class Type
	{
		WIN,//�^�C�g�����S
		LOSE,//�uPressA�v���S
	};

	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*���������֐�*/
	void SetType();

	/*�����o�ϐ�*/
	HPUI*		hp;			//�o�[UI
	ButtonUI*	button;		//�{�^��UI
	BossNameUI* bossName;	//�{�X�̖��OUI
	int			imageHandle;//�摜�n���h��
	bool		isEnd;		//�I���t���O
	int			alpha;		//�A���t�@�l
	int			fontHandle;	//�t�H���g�n���h��
	int			pauseFontHandle;//���f�p�t�H���g�n���h��
	int			frameCount;	//�t���[���J�E���g
	int			type;		//���S�̎��
	bool		isPause;	//���f�t���O

	short maxAlpha;//�ő�A���t�@
	short addAlpha;//�����A���t�@
	short logoDrawTime;//���S�`�掞��
	vector<int> tableDrawRect;//�e�[�u���`��͈�
	vector<int> pauseTableDrawRect;//���f�e�[�u���`��͈�
	vector<int> pauseTextPosition;//���f�������W
	vector<int> destroyTextPosition;//�����������W
	vector<int> resultTextPosition;//���ʕ������W
};

