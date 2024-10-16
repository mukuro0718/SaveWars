#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "SceneBase.h"
#include "Image.h"
#include "SceneUI.h"
#include "TutorialUI.h"
#include "CameraManager.h"
#include "MapManager.h"
#include "UIManager.h"
#include "InputManager.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
TutorialUI::TutorialUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>	 ::GetInstance();
	auto& asset = Singleton<LoadingAsset>::GetInstance();

	/*�摜�N���X�C���X�^���X�̍쐬*/
	const int WINDOW = asset.GetImage(LoadingAsset::ImageType::WINDOW);
	this->window = new Image(WINDOW);
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::MINTYO_50_32));
	this->fontHandle.emplace_back(asset.GetFont(LoadingAsset::FontType::MINTYO_50_32));

	/*������*/
	Initialize();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
TutorialUI::~TutorialUI()
{
}

/// <summary>
/// ������
/// </summary>
void TutorialUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json   = Singleton<JsonManager>	 ::GetInstance();

	this->window->SetPosition(json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_FIRST_DRAW_RECT"]);
	this->window->alpha		 = Image::MAX_ALPHA;
	this->window->isAddAlpha = false;
	this->isExtendWindow	 = true;
	this->isReducedWindow	 = false;
	this->isEnd				 = false;
	this->isPrevPressButton  = false;
}


/// <summary>
/// �X�V
/// </summary>
void TutorialUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& json = Singleton<JsonManager>	::GetInstance();
	bool isPressButton = IsPressButton();

	if (isPressButton)
	{
		this->isReducedWindow = true;
	}

	/*window�̊g��E�k��*/
	ExtendWindow();
	ReducedWindow();
}

/// <summary>
/// �`��
/// </summary>
const void TutorialUI::Draw()const
{
	vector<int> position = this->window->position;
	DrawExtendGraph(position[0], position[1], position[2], position[3], this->window->imageHandle, TRUE);
	/*�g����k�������Ă��Ȃ����*/
	if (!this->isExtendWindow && !this->isReducedWindow)
	{
		//�V���O���g���N���X�̃C���X�^���X���擾
		auto& json = Singleton<JsonManager>::GetInstance();
		{
			string drawText = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TITLE_TEXT"];
			vector<int> fontPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TITLE_POSITION"];
			DrawStringToHandle(fontPosition[0], fontPosition[1], drawText.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::MAIN)]);
		}
		{
			vector<string>	drawText = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_TEXT"];
			vector<int>		fontPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_FONT_POSITION"];
			int				fontSize = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_FONT_SIZE"];
			//�����̕`��
			for (int i = 0; i < drawText.size(); i++)
			{
				DrawStringToHandle(fontPosition[0], fontPosition[1], drawText[i].c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::MAIN)]);
				fontPosition[1] += fontSize;
			}
			fontPosition.clear();
			drawText.clear();
		}
		{
			string drawText = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_CLOSE_WINDOW_TEXT"];
			vector<int> fontPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_CLOSE_WINDOW_POSITION"];
			DrawStringToHandle(fontPosition[0], fontPosition[1], drawText.c_str(), this->TEXT_COLOR, this->fontHandle[static_cast<int>(FontType::SUB)]);
		}
	}
}

/// <summary>
/// �V�[�����I�����邩
/// </summary>
const bool TutorialUI::IsEnd()const
{
	return this->isEnd;
}

/// <summary>
/// �I�t�Z�b�g��ǉ������l��Ԃ�
/// </summary>
std::vector<int> TutorialUI::ReturnAddOffset(const std::vector<int> _position, const std::vector<int> _offset)
{
	std::vector<int> out = _position;

	for (int i = 0; i < out.size(); i++)
	{
		out[i] += _offset[i];
	}
	return out;
}


/// <summary>
/// window�̊g��
/// </summary>
void TutorialUI::ExtendWindow()
{
	if (this->isExtendWindow)
	{
		/*�V���O���g���N���X�̃C���X�^���X���擾*/
		auto& json = Singleton<JsonManager>	::GetInstance();
		std::vector<int> targetPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_EXTEND_DRAW_RECT"];
		std::vector<int> addValue = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_ADD_VALUE"];

		/*�g�又��*/
		bool isEndExtend = false;
		isEndExtend = this->window->ExtendGraph(targetPosition, addValue);
		targetPosition = ReturnAddOffset(targetPosition, json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_OFFSET"]);
		/*�g�債�Ă�����*/
		if (isEndExtend)
		{
			this->isExtendWindow = false;
		}
	}
}

/// <summary>
/// window�̏k��
/// </summary>
void TutorialUI::ReducedWindow()
{
	if (this->isReducedWindow)
	{
		/*�V���O���g���N���X�̃C���X�^���X���擾*/
		auto& json = Singleton<JsonManager>	::GetInstance();
		std::vector<int> targetPosition = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_FIRST_DRAW_RECT"];
		std::vector<int> decreaseValue = json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_DECREASE_VALUE"];

		/*�k������*/
		bool isEndReduced = false;
		isEndReduced = this->window->ExtendGraph(targetPosition, decreaseValue);
		targetPosition = ReturnAddOffset(targetPosition, json.GetJson(JsonManager::FileType::UI)["TUTORIAL_WINDOW_OFFSET"]);
		
		/*�k�����Ă�����*/
		if (isEndReduced)
		{
			this->isReducedWindow = false;
			this->isEnd = true;
		}
	}
}

/// <summary>
/// �{�^���̓���
/// </summary>
bool TutorialUI::IsPressButton()
{
	/*�V���O���g���N���X�̃C���X�^���X���擾*/
	auto& input = Singleton<InputManager>::GetInstance();
	auto& json = Singleton<JsonManager>	 ::GetInstance();

	/*pad����*/
	int pad = input.GetPadState();
	bool isPressButton = false;

	/*window���ړ����Ă��Ȃ�/�g������Ă��Ȃ����*/
	if (!this->isExtendWindow && !this->isReducedWindow)
	{
		isPressButton = (pad & PAD_INPUT_4);
		//�O�Ƀ{�^�����͂��Ȃ�&���{�^�����͂�����
		if (!this->isPrevPressButton && isPressButton)
		{
			this->isPrevPressButton = true;
		}
		//�O�Ƀ{�^�����͂�����
		else if (this->isPrevPressButton)
		{
			//���{�^�����͂��Ȃ�
			if (!isPressButton)
			{
				this->isPrevPressButton = false;
			}
			isPressButton = false;
		}
	}

	return isPressButton;
}