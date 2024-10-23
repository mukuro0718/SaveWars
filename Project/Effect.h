//========================================
// @brief �G�t�F�N�g�x�[�X�N���X
//========================================
#pragma once

class Transform;
class Effect abstract
{
public:
	Effect(const int _effectResourceHandle);//�R���X�g���N�^
	virtual ~Effect();//�f�X�g���N�^

	void Initialize(); 
	void Update();//
	const void Draw()const;
	void OnIsPlayEffect() { this->isPlayEffect = true; }
	void SetPosition(const VECTOR _position);
	
	/*�������z�֐�*/
	virtual void SetInfoToPlayTheEffect() abstract;
protected:
	const VECTOR Convert(std::vector<float> _in)const;

	int effectResourceHandle;
	vector<int> playingEffectHandle;
	int frameCount;
	bool isPlayEffect;//�G�t�F�N�g���Đ����邩
	int startFrame;
	int endFrame;
	int firstFrame;
	Transform* transform;
	
};

