#include <DxLib.h>
#include "UseSTL.h"
#include "Timer.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Timer::Timer()
{
	auto& asset =  Singleton<LoadingAsset>::GetInstance();
	this->fontHandle = asset.GetFont(LoadingAsset::FontType::MINTYO_50_32);
}

/// <summary>
/// �`��
/// </summary>
const void Timer::Draw()const
{
	int second = this->time / 60;
	int minute = second / 60;
	second -= minute * 60;

	DrawFormatStringToHandle(1650, 10, this->TEXT_COLOR, this->fontHandle, "�o�ߎ���\n %d��%d�b", minute, second);
}