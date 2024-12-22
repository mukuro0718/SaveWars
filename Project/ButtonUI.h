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
	enum class TextType 
	{
		W_ATTACK,
		HEAL,
		S_ATTACK,
		AVOID,
		GUARD,
	};	

	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*���������֐�*/
	int ConvertColor(const std::vector<int> _color);
	void DrawItem();
	void DrawTextImage();
	void DrawButton();

	/*�����o�ϐ�*/
	int table;
	int potion;
	int operationFont;
	int itemFont;
	vector<int> button;
	vector<int>	textList; //�����̃��X�g
	vector<int>	buttonList;//�摜�̃��X�g
	map<int, vector<int>> displayTextMap;
	map<int, vector<int>> displayButtonMap;
	vector<int> changeDisplayIndexType;
	int displayIndex;
};