//===============================================
// @brief �^�C�g��UI�N���X
//===============================================
#pragma once

class Image;
class SceneUI;
class TitleUI : public SceneUI
{
public:
	TitleUI();
	~TitleUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*�񋓑�*/
	enum class Type
	{
		TITLE,//�^�C�g�����S
		PRESS,//�uPressA�v���S
	};

	/*���������֐�*/
	bool IsPressButton();

	/*�����o�ϐ�*/
	std::vector<Image*> image;
	bool isPrevPressButton;
	bool isFadeOut;
};

