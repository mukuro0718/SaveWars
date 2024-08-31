//===========================================
// @brief �v���C���[�ʏ�U���Q
//===========================================
#pragma once

class Physics;
class Collidable;
class PlayerMain_2Attack : public GoriLib::Collidable
{
public:
	PlayerMain_2Attack();//�R���X�g���N�^
	~PlayerMain_2Attack();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics);							//������
	void		Finalize(GoriLib::Physics* _physics);							//�㏈��
	void		Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction);	//�X�V
	void		OnCollide(const Collidable& _colider)override;					//�Փ˂����Ƃ�
	void		OnIsStart() { this->isStartHitCheck = true; }
	const void	Draw()const;												//�`��
private:
	static constexpr int ATTACK_NUM = 1;
	bool isStartHitCheck;
	int	 frameCount;
	int	 damage;
	bool isDontStartPrevFrame;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};

