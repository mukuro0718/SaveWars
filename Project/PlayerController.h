//====================================================
// @brief �v���C���[�R���g���[���[�N���X
// �v���C���[�̏�Ԃ��`���A�s���ׂ��A�N�V���������肷��
//====================================================
#pragma once

class PlayerController
{
public:
	 PlayerController();
	~PlayerController();

	void Initialize();
	void StateChanger(const bool _isEndAction, const CharacterData& _data);
	const int GetNowState()const { return this->nowState; }
	const int GetPrevState()const { return this->prevState; }
	void SynchroState() { this->prevState = this->nowState; }
	enum class PlayerState
	{
		IDLE					= 0,
		AVOID					= 1,
		DEATH					= 2,
		BLOCK					= 3,
		STAGGER					= 4,//�̂�����
		BLOCK_STAGGER			= 5,//�K�[�h���̂�����
		KNOCK_DOWN				= 6,//�m�b�N�_�E��
		HEAL					= 7,
		RUN						= 8,
		RUNNING_OUT_OF_STAMINA	= 9,
		WALK					= 10,
		COMBO_1					= 11,
		COMBO_2					= 12,
		COMBO_3					= 13,
		STRONG_ATTACK			= 14,
	};
private:
	/*�񋓑�*/
	enum class ActionPriority
	{
		VERY_LOW,
		LOW,
		MEDIUM,
		HIGH,
		CRITICAL,
	};

	/*���������֐�*/
	const bool CanAction(const float _stamina, const float _staminaConsumed)const;//�A�N�V�������ł��邩

	/*�����o�ϐ�*/
	int nowState;
	int prevState;
	vector<int> priority;//�A�N�V�������Ƃ̗D�揇��
};

