//=====================================================
// @brief �V�[�P���X�m�[�h
// �q�����Ԃɕ]�����A���ׂĂ̎q�m�[�h����������܂Ŏ��s����B
// �����ꂩ�̎q�m�[�h�����s�����瑦���Ɏ��s��Ԃ�
//=====================================================
#pragma once

class CompositeNode;
class SequencerNode : public CompositeNode
{
	 SequencerNode();//�R���X�g���N�^
	~SequencerNode();//�f�X�g���N�^

	NodeState Update() override;
};

