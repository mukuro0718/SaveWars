#include "UseSTL.h"
#include "State.h"
#include "StateMachine.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
StateMachine::StateMachine()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StateMachine::~StateMachine()
{

}

/// <summary>
/// ��Ԃ̕ύX
/// </summary>
void StateMachine::ChangeState(State* _newState)
{
	//��Ԃ�null����Ȃ����
	if (this->currentState)
	{
		//���݂̏�Ԃ��I��
		this->currentState->Exit();
	}
	currentState  = _newState;
	currentState->Entry();
}

/// <summary>
/// �֐��̒ǉ�
/// </summary>
void StateMachine::AddFactory(int _id, std::function<State* (void)> _factory)
{
	this->stateFactoryMap[_id] = _factory;
}

/// <summary>
/// �X�V
/// </summary>
void StateMachine::Update()
{
	if (this->currentState)
	{
		//���݂̏�ԂɊ�Â����X�V
		this->currentState->Update();
	}
}

/// <summary>
/// �J�ڏ������`�F�b�N���ď�ԑJ��
/// </summary>
void StateMachine::Transition(int _nextId)
{
	/*�}�b�v����ID�����݂��Ă�����*/
	if (this->stateFactoryMap.find(_nextId) != this->stateFactoryMap.end())
	{
		auto nextState = this->stateFactoryMap[_nextId]();
		ChangeState(nextState);
	}
}
