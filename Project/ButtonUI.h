//======================================================
// @brief ButtonUI�N���X
//======================================================

#pragma once
class ButtonUI
{
public:
	ButtonUI();		//�R���X�g���N�^
	~ButtonUI();		//�f�X�g���N�^
	void Initialize();//������
	void Update();
	void Draw();	//�`��
private:

	/*�\����*/
	struct Vec2d
	{
		void Set(const int _x, const int _y) { this->x = _x; this->y = _y; }
		void Set(const std::vector<int> _pos) { this->x = _pos[0]; this->y = _pos[1]; }
		int x, y;
	};
	struct Box
	{
		void Set(const std::vector<int> _pos) { this->lx = _pos[0]; this->ly = _pos[1]; this->rx = _pos[2]; this->ry = _pos[3];}
		int lx, ly, rx, ry;
	};

	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xc0ffff;
	static constexpr int PRESS_TEXT_COLOR = 0xffff88;
	static constexpr int PRESS_OFFSET = 4;

	/*���������֐�*/
	int ConvertColor(const std::vector<int> _color);
	void DrawIcon();
	void DrawFont();
	void DrawButton();

	/*�����o�ϐ�*/
	int healIcon;
	int healOrb;
	int emptyOrb;
	int iconFont, operationFont;
	vector<int> button;
	int buttonFont;
};