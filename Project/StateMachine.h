//==========================================================================
// @brief �X�e�[�g�}�V��
//==========================================================================
#pragma once

class State;
class StateMachine
{
public:
			 StateMachine();//�R���X�g���N�^
	virtual ~StateMachine();//�f�X�g���N�^

	void ChangeState(State* _newState);//��Ԃ̕ύX
	void AddFactory(int _id, std::function<State* (void)> _factory);//�֐��̒ǉ�
	void Update();//�X�V
	void Transition(int _nextId);//����ID
private:
	State* currentState;//���݂̏��
	std::map<int, std::function<State* (void)>> stateFactoryMap;//��ԕΈڃ}�b�v
};

