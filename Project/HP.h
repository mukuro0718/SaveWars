//======================================================
// @brief HP�N���X
// �{�X�ƃv���C���[��HP�̕\�����s��
// �iHP�̒��ڊǗ��͍s��Ȃ��j
//======================================================

#pragma once
class HP
{
public:
	HP			   ();		//�R���X�g���N�^
	~HP			   ();		//�f�X�g���N�^
	void Update();
	const void Draw()const;	//�`��
private:
	/*�ÓI�萔*/
	static const	 int BOSS_HP_COLOR;	 //�{�XHP�̐F
	static const	 int PLAYER_HP_COLOR;//�v���C���[HP�̐F
	static const	 int PREV_HP_COLOR;	 //HP�̍��̐F
	static constexpr int HP_BAR_HEIGHT		= 20;
	static constexpr int PLAYER_INDEX_BASE	= 300;//�w�����̂���
	static constexpr int BOSS_INDEX_BASE	= 1000;//�w�����̂���

	/*�\����*/
	struct RangeNum
	{
	public:
		void SetRange(const int _max, const int _min, const int _now);
		void Add(const int _add);
		const int GetMax()const { return this->max; }
		const int GetNow()const { return this->now; }
	private:
		int max, min, now;
	};
	struct Vec2d
	{
		void Set(const int _x, const int _y) { this->x = _x; this->y = _y; }
		int x, y;
	};
	struct Box
	{
		int lx, ly, rx, ry;
	};

	/*�����o�ϐ�*/
	RangeNum playerHP;	//�v���C���[HP
	RangeNum bossHP;	//�{�XHP
	Vec2d playerHPPosition;
	Vec2d bossHPPosition;
};

