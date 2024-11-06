//===========================================================================
//@brief �A�j���[�V�����N���X
//===========================================================================

#pragma once

class Animation
{
public:
    Animation();    //�R���X�g���N�^
    ~Animation();   //�f�X�g���N�^

    void Add        (const int _animationHandle, const int _animatinoIndex);                          //�A�j���[�V�����̒ǉ�
    void Attach     (int* _modelHandle);                                                              //�A�j���[�V�����̃A�^�b�`
    void Play       (int* _modelHandle, const int _nextAnimation, const float _animationPlayTime);;   //�A�j���[�V�����̍Đ�����
    void SetAddRate (const float _animationRate) { this->addAnimationRate = _animationRate; }         //�A�j���[�V�����u�����h�����ʂ̐ݒ�
    
    /*getter*/
    const bool GetIsChangeAnim()const { return isChange; }//�A�j���[�V�����̕ύX�t���O��getter
private:
    /*�ÓI�萔*/
    const float ADD_ANIMATION_RATE = 0.05f;//�A�j���[�V�����u�����h������

    /*�����o�ϐ�*/
    std::vector<int> animationHandle;           //�A�j���[�V�����̃��f���̃n���h��
    std::vector<int> animationIndex;            //�g�p�������A�j���[�V�����̔ԍ�
    float            animationTotalTime;        //�A�j���[�V�������Đ�����
    float            animationPlayTime;         //�A�j���[�V�����Đ�����
    float            animationRate;             //�A�j���[�V�����̃u�����h��
    float            addAnimationRate;          //�A�j���[�V�����u�����h��������
    bool             isChange;                  //�A�j���[�V�����ύX�t���O�itrue:�ύX�\ false:�s�j
    int              animationAttachIndex;      //�A�j���[�V�����A�^�b�`�ԍ�
    int              prevAnimationAttachIndex;  //�ȑO�̃A�j���[�V�����A�^�b�`�ԍ�
    int              prevAnimation;             //�O�̃A�j���[�V����
    int              nowAnimation;              //���݂̃A�j���[�V����
};