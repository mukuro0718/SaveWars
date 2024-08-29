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

	void Initialize(GoriLib::Physics* _physics); //������
	void Finalize(GoriLib::Physics* _physics);
	void Update(GoriLib::Physics* _physics); //�X�V
	void OnCollide(const GoriLib::Collidable& _colider); //�ړ��x�N�g���̏C��
	const void Draw	()const;//�`��

private:
	/*���������֐�*/
	 MapManager();//�R���X�g���N�^
	~MapManager();//�f�X�g���N�^

	/*�����o�ϐ�*/
	BossMap* ground;
	Skydome* skydome;
};

