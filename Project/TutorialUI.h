//===============================================
// @brief �`���[�g���A��UI�N���X
//===============================================
#pragma once

class Image;
class SceneUI;
class TutorialUI : public SceneUI
{
public:
	TutorialUI();
	~TutorialUI();

	void Initialize()		 override;
	void Update()		 override;
	const void Draw()const override;
	const bool IsEnd()const override;
private:
	/*�񋓑�*/
	enum class FontType
	{
		MAIN = 0,
		SUB = 1
	};

	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xdfffff;

	/*���������֐�*/
	std::vector<int>  ReturnAddOffset(const std::vector<int> _position, const std::vector<int> _offset);
	bool			  IsPressButton();
	void			  ExtendWindow();
	void			  ReducedWindow();

	/*�����o�ϐ�*/
	//Image*		window;				//window�摜
	bool		isPrevPressButton;	//�O�Ƀ{�^�������͂���Ă��邩
	bool		isExtendWindow;		//�摜���g�傷�邩
	bool		isReducedWindow;	//�k�����邩
	bool		isEnd;				//�I�����邩
	vector<int> fontHandle;			//�t�H���g
};

