//===============================================
// @brief ���U���gUI�N���X
//===============================================
#pragma once

class Image;
class SceneUI;
class GameOverUI : public SceneUI
{
public:
	GameOverUI();
	~GameOverUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
	const bool IsContinue()const;
private:
	/*�񋓑�*/
	//�摜�̎��
	enum class ImageType
	{
		CONTINUE,
		END,
	};

	static constexpr int TEXT_COLOR = 0xffffff;
	static constexpr int PRESS_TEXT_COLOR = 0xffff88;

	void SetType();
	bool IsPressButton();

	int imageHandle;
	int fontHandle;
	int alpha;

	bool isPrevPressButton;
	bool isContinue;
	bool isEnd;
	int  type;//�Q�[���I�[�o�[���̃^�C�v�iContinue/End�j
};

