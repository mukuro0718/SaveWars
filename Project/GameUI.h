//===============================================
// @brief �Q�[��UI�N���X
//===============================================
#pragma once

class Image;
class SceneUI;
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
	int imageHandle;
	bool isEnd;
	int alpha;
	int fontHandle;
	int frameCount;
	int type;
};

