#pragma once
class Effect
{
public:
	Effect(const int _effectResourceHandle);//�R���X�g���N�^
	~Effect(){}//�f�X�g���N�^

	void OnIsPlayEffect() { this->isPlayEffect = true; }
	void Update(const int _startFrame, const int _endFrame, const VECTOR _position, const VECTOR _rotation, const VECTOR _scale);//
	const void Draw()const;
private:
	int effectResourceHandle;
	int playingEffectHandle;
	int frameCount;
	bool isPlayEffect;//�G�t�F�N�g���Đ����邩
};

