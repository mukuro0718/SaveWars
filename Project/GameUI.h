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
	void Update()		 override;
	const void Draw()const override;
	const bool IsEnd()const override;
private:
	/*�񋓑�*/
	enum class Type
	{
		WIN,//�^�C�g�����S
		LOSE,//�uPressA�v���S
	};

	/*���������֐�*/
	void SetType();

	/*�����o�ϐ�*/
	std::vector<Image*> image;
	bool isPrevPressAButton;
	bool isEndExtend;
	bool isEnd;
	int frameCount;
	int type;
};

