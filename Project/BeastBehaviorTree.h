//====================================================
// @brief ���N�X���I�p�r�w�C�r�A�c���[
//====================================================
#pragma once

class BehaviorTree;
class BeastBehaviorTree
{
public:
	/*�񋓑�*/
	//�L�����N�^�[�̏��
	enum class BeastState
	{
		ANGRY		= 0,
		NORMAL		= 1,
		DOWN		= 2,
		FRIGHTENING = 3,
		BREAK		= 4,
	};

	 BeastBehaviorTree();
	~BeastBehaviorTree();

	void Update();
private:
	BehaviorTree* root;
};

