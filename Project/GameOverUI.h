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
	void Update()		 override;
	const void Draw()const override;
	const bool IsEnd()const override;
	const bool IsContinue()const;
private:
	/*�񋓑�*/
	enum class ImageType
	{
		CONTINUE,
		END,
		WINDOW,//�E�B���h�E
	};

	void SetType();
	bool IsPressButton();

	std::vector<Image*> image;
	bool isPrevPressButton;
	bool isContinue;
	bool isGameClear;
	bool isEnd;
	bool isEndExtend;
	int type;//�Q�[���I�[�o�[���̃^�C�v�iContinue/End�j
};

