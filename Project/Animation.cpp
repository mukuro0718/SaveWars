#include <DxLib.h>
#include "UseSTL.h"
#include "Animation.h"

/// <summary>
/// コンストラクタ
/// </summary>
Animation::Animation()
	: animationAttachIndex(0)
	, prevAnimation(0)
	, nowAnimation(0)
	, animationRate(0.0f)
	, prevAnimationAttachIndex(0)
	, animationTotalTime(0.0f)
	, animationPlayTime(0.0f)
	, isChange(false)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Animation::~Animation()
{
	this->animationHandle.clear();
	this->animationIndex.clear();
}

/// <summary>
/// アニメーションの追加
/// </summary>
void Animation::Add(const int _animationHandle,const int _animatinoIndex)
{
	this->animationHandle.emplace_back(_animationHandle);
	this->animationIndex.emplace_back(_animatinoIndex);
}

/// <summary>
/// アニメーションのアタッチ
/// </summary>
void Animation::Attach(int* modelHandle)
{
	/*アニメーションのデタッチ*/
	MV1DetachAnim(*modelHandle, this->prevAnimationAttachIndex);
	MV1DetachAnim(*modelHandle, this->animationAttachIndex);

	/*アニメーションのアタッチ*/
	this->prevAnimationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->prevAnimation], this->animationHandle[this->prevAnimation], FALSE);
	this->animationAttachIndex = MV1AttachAnim(*modelHandle, this->animationIndex[this->nowAnimation], this->animationHandle[this->nowAnimation], FALSE);
	
	/*アニメーションの総再生時間を設定*/
	this->animationTotalTime = MV1GetAttachAnimTotalTime(*modelHandle, this->animationAttachIndex);
	
	/*アニメーションのブレンド率を初期化*/
	this->animationRate = 0.0f;

	/*アニメーション再生時間を設定*/
	MV1SetAttachAnimTime(*modelHandle, this->animationAttachIndex, this->animationPlayTime);
}
/// <summary>
/// アニメーションの再生
/// </summary>
void Animation::Play(int* modelHandle, VECTOR& _position, const int _nextAnimation, const float _animationPlayTime)
{
	/*もし今までアタッチしていたアニメーションと次のアニメーションが違うなら*/
	if (this->nowAnimation != _nextAnimation)
	{
		this->prevAnimation = this->nowAnimation;
		this->nowAnimation = _nextAnimation;
		this->isChange = false;
		this->animationPlayTime = 0.0f;
		//アニメーションのアタッチ
		Attach(modelHandle);
	}

	/*アニメーションのブレンド率をセット*/
	MV1SetAttachAnimBlendRate(*modelHandle, this->prevAnimationAttachIndex, 1.0f - this->animationRate);
	MV1SetAttachAnimBlendRate(*modelHandle, this->animationAttachIndex, this->animationRate);

	/*ブレンド率が1以上だったら*/
	if (this->animationRate >= 1.0f)
	{
		//前のアタッチしていたアニメーションをデタッチする
		MV1DetachAnim(*modelHandle, this->prevAnimationAttachIndex);
	}
	else
	{
		//ブレンド率を増加
		this->animationRate += 0.1f;
	}

		/*アニメーション再生時間を進める*/
		this->animationPlayTime += _animationPlayTime;
		MV1SetAttachAnimTime(*modelHandle, this->animationAttachIndex, this->animationPlayTime);
	/*再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す*/
	if (this->animationPlayTime >= this->animationTotalTime)
	{
		this->animationPlayTime = 0.0f;
		this->isChange = true;
	}
}
