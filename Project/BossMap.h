//===========================================
// @brief �n�ʃN���X
// �n�ʂ̕`����s��
//===========================================
#pragma once

class Physics;
class Collidable;
class BossMap : public GoriLib::Collidable
{
public:
	BossMap();//�R���X�g���N�^
	~BossMap();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics);		//������
	void		Finalize(GoriLib::Physics* _physics);		//�㏈��
	void		Update(GoriLib::Physics* _physics);		//�X�V
	void		OnCollide(const Collidable& _colider)override;//�Փ˂����Ƃ�
	const void Draw()const;	//�`��

private:
	/*�����o�ϐ�*/
	int modelHandle;
};

