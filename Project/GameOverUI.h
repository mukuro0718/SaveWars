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
	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xdfffff;

	/*�񋓑�*/
	//�摜�̎��
	enum class ImageType
	{
		CONTINUE,
		END,
	};
	//�t�H���g�̎��
	enum class FontType
	{
		MAIN,
		SUB,
	};

	void SetType();
	bool IsPressButton();

	std::vector<Image*> image;
	std::vector<int> fontHandle;
	bool isPrevPressButton;
	bool isContinue;
	bool isEnd;
	bool isEndExtend;
	int  type;//�Q�[���I�[�o�[���̃^�C�v�iContinue/End�j
};

