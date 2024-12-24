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
	HPUI* hp;
	ButtonUI* button;
	BossNameUI* bossName;
	int imageHandle;
	bool isEnd;
	int alpha;
	int fontHandle;
	int frameCount;
	int type;
};

