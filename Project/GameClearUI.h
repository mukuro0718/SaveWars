//===============================================
// @brief ���U���gUI�N���X
//===============================================
#pragma once

class Image;
class SceneUI;
class GameClearUI : public SceneUI
{
public:
	GameClearUI();
	~GameClearUI();

		  void Initialize()		 override;
		  void Update	 ()		 override;
	const void Draw		 ()const override;
	const bool IsEnd	 ()const override;
private:
	/*�ÓI�萔*/
	static constexpr int INDEX = 2;
	static constexpr int TEXT_COLOR = 0xdfffff;

	enum class FontType
	{
		HEADER = 0,
		MAIN = 1,
	};
	enum class StarType
	{
		NONE = -1,
		SILVER = 0,
		GOLD = 1,
	};

	bool IsPressButton();
	void CalcRanking();

	int nowRanking;//���̏���
	int fontHandle;
	int imageHandle;
	int alpha;
	bool isEnd;
	bool isPrevPressButton;
};

