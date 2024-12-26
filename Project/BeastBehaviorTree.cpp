#include "UseSTL.h"
#include "UseJson.h"
#include "BeastBehaviorTreeHeader.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BeastBehaviorTree::BeastBehaviorTree()
	: Selector_DeathOrReactionOrBreakOrBattle(nullptr)
	, state(BeastState::NORMAL)
	, downValue(0)
	, prevHp(0)
	, damage(0)
	, level(0)
{
	/*�C���^�[�o����������*/
	this->intervalSet = { 0 };

	/*�匳�̃c���[�̍쐬*/
	this->Selector_DeathOrReactionOrBreakOrBattle = new SelectorNode();

	/*HP���Ȃ��Ƃ��̃A�N�V����*/
	{
		//�f�X����m�[�h
		BehaviorTreeNode* Sequencer_DeathIfHpIsLessThanZero = new SequencerNode();
		Sequencer_DeathIfHpIsLessThanZero->AddChild(new CheckDyingCondition());
		Sequencer_DeathIfHpIsLessThanZero->AddChild(new Beast_Dying());
	}


	/*��ԕΈڗp�c���[�̍쐬*/
	BehaviorTreeNode* stateTransitionTree = new SelectorNode();
	{
		//��ԕΈڃm�[�h�̍쐬
		BehaviorTreeNode* stateTransitionNode = new SelectorNode();
		//�{��ւ̕Έ�
		BehaviorTreeNode* changeAngryNode = new SequencerNode();
		changeAngryNode->AddChild(new CheckAngryCondition());
		changeAngryNode->AddChild(new Beast_Roar());
		//��ԕΈڃm�[�h�ւ̒ǉ�
		stateTransitionNode->AddChild(changeDeathNode);
		stateTransitionNode->AddChild(changeAngryNode);

		//�񕜃m�[�h

		//�c���[�ɒǉ�
		stateTransitionTree->AddChild(stateTransitionNode);
	}

	/*�퓬�p�c���[�̍쐬*/
	BehaviorTreeNode* battleTree = new ExecuteAllNode();
	{
		//�������U���͈͊O�Ƀv���C���[�����鎞�̃m�[�h
		BehaviorTreeNode* moreFarTree = new SequencerNode();
		BehaviorTreeNode* moreFarNode = new SelectorNode();
		{
			//�ːi�U���m�[�h
			BehaviorTreeNode* rushNode = new SequencerNode();
			rushNode->AddChild(new CheckRushCondition());
			rushNode->AddChild(new Beast_Rush());
			//�����m�[�h
			BehaviorTreeNode* walkNode = new SequencerNode();
			walkNode->AddChild(new CheckWalkCondition());
			rushNode->AddChild(new Beast_Walk());
			//�m�[�h�ɒǉ�
			moreFarNode->AddChild(rushNode);		 //�ˌ�
			moreFarNode->AddChild(walkNode);		 //����
			moreFarNode->AddChild(new Beast_Run());//�_�b�V��
		}
		moreFarTree->AddChild(new CheckMoreFarCondition());
		moreFarTree->AddChild(moreFarNode);

		//����U���\���̃m�[�h
		BehaviorTreeNode* specialAttackNode = new SequencerNode();
		{
			//�X�[�p�[�m���@
			BehaviorTreeNode* superNovaNode = new SequencerNode();
			superNovaNode->AddChild(new CheckSuperNovaCondition());
			superNovaNode->AddChild(new Beast_SuperNova());
			//����m�[�h�ɒǉ�
			BehaviorTreeNode* specialNode = new SelectorNode();
			specialNode->AddChild(superNovaNode);
			specialNode->AddChild(new Beast_RaiseLevel());
			//����U���m�[�h�ɒǉ�
			specialAttackNode->AddChild(new CheckSpecialNodeCondition());
			specialAttackNode->AddChild(specialNode);
		}

		//�ߐڍU���\���̃m�[�h
		BehaviorTreeNode* nearActionNode = new SequencerNode();
		{
			//�E���U��
			BehaviorTreeNode* rightFootAttackNode = new SequencerNode();
			rightFootAttackNode->AddChild(new CheckRightAttackCondition());
			rightFootAttackNode->AddChild(new Beast_RightFootAttack());
			//�R���{�A�^�b�N�P
			BehaviorTreeNode* comboAttack1 = new SequencerNode();
			comboAttack1->AddChild(new CheckCombo1AttackCondition());
			comboAttack1->AddChild(new Beast_FootComboAttack());
			//�E�����ߍU��
			BehaviorTreeNode* chargeRightAttack = new SequencerNode();
			chargeRightAttack->AddChild(new CheckChargeRightAttackCondition());
			chargeRightAttack->AddChild(new Beast_ChargeRightFootAttack());
			//�������ߍU��
			BehaviorTreeNode* chargeBothAttack = new SequencerNode();
			chargeBothAttack->AddChild(new CheckChargeBothAttackCondition());
			chargeBothAttack->AddChild(new Beast_ChargeBothFootAttack());
			//�R���{�A�^�b�N�Q
			BehaviorTreeNode* comboAttack2 = new SequencerNode();
			comboAttack2->AddChild(new CheckCombo2AttackCondition());
			comboAttack2->AddChild(new Beast_RightFootAndRotAttack());
			//Normal���ߐڍU��
			BehaviorTreeNode* nearNormalAttackNode = new SelectorNode();
			nearNormalAttackNode->AddChild(rightFootAttackNode);
			nearNormalAttackNode->AddChild(comboAttack1);
			//Angry���ߐڍU��
			BehaviorTreeNode* nearAngryAttackNode = new SelectorNode();
			nearAngryAttackNode->AddChild(chargeRightAttack);
			nearAngryAttackNode->AddChild(chargeBothAttack);
			nearAngryAttackNode->AddChild(comboAttack2);
			//Normal���ߐڃZ�b�g
			BehaviorTreeNode* nearNormalAttackNodeSet = new SequencerNode();
			//nearNormalAttackNodeSet->AddChild();
		}



		//�o�g���m�[�h�̍쐬
		BehaviorTreeNode* battleNode = new SelectorNode();
		battleNode->AddChild(new Beast_Idle());

		//�c���[�ɒǉ�
		battleTree->AddChild(new Beast_CalcToTargetDistance());
		battleTree->AddChild(moreFarNode);
		battleTree->AddChild(battleNode);
	}

	/*root�ɒǉ�*/
	this->root->AddChild(stateTransitionTree);
	this->root->AddChild(battleTree);

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
BeastBehaviorTree::~BeastBehaviorTree()
{

}

/// <summary>
/// ������
/// </summary>
void BeastBehaviorTree::Initialize()
{
	auto& json = Singleton<JsonManager>::GetInstance();
	this->prevHp = json.GetJson(JsonManager::FileType::BEAST)["HP"];
	this->state = BeastState::NORMAL;
	this->downValue = 0;
	this->damage = 0;
	this->level = 0;
}

/// <summary>
/// �X�V
/// </summary>
void BeastBehaviorTree::Update()
{
	/*�C���^�[�o���̌v�Z*/
	for (int& interval : this->intervalSet)
	{
		if (interval != 0)
		{
			interval--;
		}
	}

	/*�ڕW�܂ł̋��������߂�*/

	/*�c���[�̎��s*/
	this->root->Update();
}