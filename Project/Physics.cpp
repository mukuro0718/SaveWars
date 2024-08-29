#include <cassert>
#include <DxLib.h>
#include <list>
#include "Mylib.h"

/// <summary>
/// �Փ˕��̓o�^
/// </summary>
void Physics::Entry(Collidable* _collidable)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*�o�^����Ă��Ȃ�������*/
	if (!found)
	{
		collidables.emplace_back(_collidable);
	}
	/*�o�^����Ă�����G���[��f��*/
	else
	{
		assert(0 && "�w���collidable�͓o�^�ς݂ł�");
	}
}

/// <summary>
/// �Փ˕��̓o�^����
/// </summary>
void Physics::Exit(Collidable* _collidable)
{
	/*�o�^����Ă��邩���ׂ�*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*�o�^����Ă�����*/
	if (!found)
	{
		collidables.remove(_collidable);
	}
	/*�o�^����Ă��Ȃ�������G���[��f��*/
	else
	{
		assert(0 && "�w���collidable�͓o�^����Ă��܂���");
	}

}

/// <summary>
/// �X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)
/// </summary>
void Physics::Update()
{
	// TODO:�Փˏ��̍X�V�ƒʒm
	for (auto& item : collidables)
	{
		//if(�Փ˂�����)
		item->OnCollide();
	}
}