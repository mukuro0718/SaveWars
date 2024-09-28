//======================================================
// @brief HPUI�N���X
// �{�X�ƃv���C���[��HP�̕\�����s��
// �iHP�̒��ڊǗ��͍s��Ȃ��j
//======================================================

#pragma once
class HPUI
{
public:
	HPUI();		//�R���X�g���N�^
	~HPUI();		//�f�X�g���N�^
	
		  void Initialize();		//������
		  void Update	 ();		//�X�V
	const void Draw		 ()const;	//�`��
private:
	/*�\����*/
	//�͈�
	struct RangeNum
	{
	public:
		void SetRange	 (const int _max, const int _min, const int _now);	//�͈͂̐ݒ�
		void Add		 (const int _add);									//�ǉ�
		void PrevDecrease();												//�O�̒l������
		
		/*getter/setter*/
		const float GetMax ()const { return this->max; } //�ő�l�̎擾
		const float GetNow ()const { return this->now; } //���̒l�̎擾
		const float GetPrev()const { return this->prev; }//�O�̒l�̎擾
			  void  SetNow (const int _hp);				 //���̒l��ݒ�
	private:
		/*�����o�ϐ�*/
		float max, min, now, prev;//�͈�
	};
	//�񎟌��x�N�g���\����
	struct Vec2d
	{
		void Set(const int _x, const int _y) { this->x = _x; this->y = _y; }
		void Set(const std::vector<int> _pos) { this->x = _pos[0]; this->y = _pos[1]; }
		int x, y;
	};
	//�{�b�N�X�\����
	struct Box
	{
		void Set(const std::vector<int> _pos) { this->lx = _pos[0]; this->ly = _pos[1]; this->rx = _pos[2]; this->ry = _pos[3]; }
		int lx, ly, rx, ry;
	};

	
	/*���������֐�*/
	int ConvertColor(const std::vector<int> _color);

	/*�����o�ϐ�*/
	int		 backgroundColor;	//
	int		 playerHPColor;		//
	int		 bossHPColor;		//
	int		 prevPlayerHPColor;	//
	int		 prevBossHPColor;	//
	int		 staminaColor;		//
	RangeNum playerHP;			//�v���C���[HP
	RangeNum playerStamina;		//�v���C���[HP
	RangeNum bossHP;			//�{�XHP
	Vec2d	 playerHPPosition;	//�v���C���[HP���W
	Vec2d	 bossHPPosition;	//�{�XHP���W

	int	playerHPBar;	//HP�o�[
	int playerHPTable;
	int playerStaminaBar;
	int playerStaminaTable;
	int bossHPBar;
	int bossHPTable;
};