//========================================
// @brief �}�b�v�}�l�[�W���[�N���X
// �}�b�v�̊Ǘ����s��
//========================================
#pragma once
#include "Singleton.h"

class BossMap;
class Skydome;
class MapManager : public Singleton<MapManager>
{
public:
	friend class Singleton<MapManager>;

	void Initialize(); //������
	void Finalize();
	void Update(); //�X�V
	const void Draw	()const;//�`��

private:
	/*���������֐�*/
	 MapManager();//�R���X�g���N�^
	~MapManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	BossMap* ground;
	Skydome* skydome;
};

