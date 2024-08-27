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
	void Initialize();//������
	void Update();
	const void Draw()const;	//�`��
private:
	/*�ÓI�萔*/
	static const	 int BOSS_HP_COLOR;	 //�{�XHP�̐F
	static const	 int PLAYER_HP_COLOR;//�v���C���[HP�̐F
	static const	 int PREV_HP_COLOR;	 //HP�̍��̐F
	static constexpr int HP_BAR_HEIGHT		= 20;
	static constexpr float PLAYER_INDEX_BASE	= 300;//�w�����̂���
	static constexpr float BOSS_INDEX_BASE	= 1000;//�w�����̂���

	/*�\����*/
	struct RangeNum
	{
	public:
		void SetRange(const int _max, const int _min, const int _now);
		void Add(const int _add);
		void PrevDecrease();
		const float GetMax()const { return this->max; }
		const float GetNow()const { return this->now; }
		const float GetPrev()const { return this->prev; }
		void SetNow(const int _hp);
	private:
		float max, min, now, prev;
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

