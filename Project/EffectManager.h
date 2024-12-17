#pragma once
#include "Singleton.h"


class Effect;
class BossFlameEffect;
class EffectManager : public Singleton<EffectManager>
{
public:
	/*EffectManager��Singleton���t�����h��*/
	friend class Singleton<EffectManager>;

	virtual ~EffectManager();//�f�X�g���N�^

	/*�񋓑�*/
	//�G�t�F�N�g�̎��
	enum class EffectType
	{
		BOSS_IMPACT		 = 0,//�{�X�̍U���������������̃G�t�F�N�g
		BOSS_ROAR		 = 1,//�{�X�̍U���������������̃G�t�F�N�g
		PLAYER_GUARD_HIT = 2,//�v���C���[�̃K�[�h�����������Ƃ��̃G�t�F�N�g
		PLAYER_HEAL		 = 3,//�v���C���[���񕜂����Ƃ��̃G�t�F�N�g
		PLAYER_IMPACT	 = 4,//�v���C���[�̍U���������������̃G�t�F�N�g
		PLAYER_CHARGE	 = 5,//�v���C���[�̗��ߍU���G�t�F�N�g
	};

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��
	
	void OnIsEffect (const EffectType _type);							//�G�t�F�N�g�Đ��t���O�𗧂Ă�
	void SetPosition(const EffectType _type, const VECTOR _position);	//���W�̐ݒ�
private:
	/*���������֐�*/
	EffectManager();//�R���X�g���N�^
		
	/*�����o�ϐ�*/
	std::vector<Effect*> effect;
};

