//===============================================================
// @brief �����A�Փ˔��肷��I�u�W�F�N�g��o�^���A
// �����ړ��A�Փ˂�ʒm����
//===============================================================
#pragma once

class Collidable;
class Physics final
{
public:
	void Entry(Collidable* collidable);//�Փ˕��̓o�^
	void Exit(Collidable* collidable);//�o�^����
	void Update();//�X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)
private:
	std::list<Collidable*> collidables;//�o�^���ꂽCollidable�̃��X�g

};

