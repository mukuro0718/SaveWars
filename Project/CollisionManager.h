//===============================================================
// @brief �����A�Փ˔��肷��I�u�W�F�N�g��o�^���A
// �����ړ��A�Փ˂�ʒm����
//===============================================================
#pragma once
#include "Singleton.h"

class ColliderData;
class CollisionManager : public Singleton<CollisionManager>
{
public:
	friend class Singleton<CollisionManager>;
	virtual ~CollisionManager(){}//�f�X�g���N�^


	void Entry(ColliderData& _data);//�Փ˕��̓o�^
	void Exit(ColliderData& _data);//�o�^����
	void Update();//�X�V(�o�^�I�u�W�F�N�g�̕����ړ��A�Փ˒ʒm)
private:
	static constexpr float GRAVITY = -0.01f;//�d��
	static constexpr float MAX_GRAVITY_ACCEL = -0.15f;//�ő�d�͉����x
	static constexpr int BEFORE_FIX_INFO_COLOR = 0x0000ff;//�␳�O���F
	static constexpr int AIM_INFO_COLOR = 0x0000aa;//�␳�O�\����F
		static constexpr int AFTER_FIX_INFO_COLOR = 0xff00ff;//�␳����F
	static constexpr int DIV_NUM = 16;
	static constexpr int MAX_CHECK_COUNT = 1000;

	std::list<ColliderData*> collidables;//�o�^���ꂽCollidable�̃��X�g

	struct OnCollideInfo
	{
		ColliderData& owner;
		ColliderData& colider;
	};

	/*���������֐�*/
	CollisionManager(){}//�R���X�g���N�^
	std::vector<OnCollideInfo> CheckColide();
	bool IsCollide(ColliderData& _objectA, ColliderData& _objectB);
	//�ʒu�␳�A����
	void FixNextPosition(ColliderData& _primary, ColliderData& _secondary);
	void FixPosition();
};

