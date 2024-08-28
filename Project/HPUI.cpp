#include <DxLib.h>
#include "UseSTL.h"
#include "UseJson.h"
#include "LoadingAsset.h"
#include "HPUI.h"
#include "PlayerManager.h"
#include "EnemyManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
HPUI::HPUI()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& json = Singleton<JsonManager>::GetInstance();

	this->backgroundColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BACKGROUND_COLOR"]);
	this->playerHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_COLOR"]);
	this->bossHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_HP_COLOR"]);
	this->prevPlayerHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["PLAYER_PREV_HP_COLOR"]);
	this->prevBossHPColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["BOSS_PREV_HP_COLOR"]);
	this->staminaColor = ConvertColor(json.GetJson(JsonManager::FileType::UI)["STAMINA_COLOR"]);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
HPUI::~HPUI()
{

}

/// <summary>
/// ������
/// </summary>
void HPUI::Initialize()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();
	/*������*/
	int playerHP = player.GetHP();
	int bossHP = enemy.GetHP();

	this->playerHP.SetRange(playerHP, 0, playerHP);
	this->bossHP.SetRange(bossHP, 0, bossHP);
	this->playerStamina.SetRange(500, 0, 500);
}

/// <summary>
/// �X�V
/// </summary>
void HPUI::Update()
{
	/*�V���O���g���N���X�̃C���X�^���X�̎擾*/
	auto& player = Singleton<PlayerManager>::GetInstance();
	auto& enemy = Singleton<EnemyManager>::GetInstance();

	/*HP�v�Z*/
	this->playerHP.SetNow(player.GetHP());
	this->bossHP.SetNow(enemy.GetHP());

	this->playerHP.PrevDecrease();
	this->bossHP.PrevDecrease();
}

/// <summary>
/// �`��
/// </summary>
const void HPUI::Draw()const
{
	auto& json = Singleton<JsonManager>::GetInstance();
	//HP
	{
		Vec2d position;
		Box box;
		int height;
		int indexBase = json.GetJson(JsonManager::FileType::UI)["PLAYER_INDEX_BASE"];
		int nowHP = static_cast<int>(this->playerHP.GetNow() / this->playerHP.GetMax() * indexBase);
		int prevHP = static_cast<int>(this->playerHP.GetPrev() / this->playerHP.GetMax() * indexBase);
		position.Set(json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_POSITION"]);
		height = json.GetJson(JsonManager::FileType::UI)["PLAYER_HP_HEIGHT"];

		box.lx = position.x;
		box.ly = position.y;
		box.rx = box.lx + indexBase;
		box.ry = box.ly + height;

		DrawBox(box.lx, box.ly, box.lx + indexBase, box.ry, this->backgroundColor, TRUE);
		DrawBox(box.lx, box.ly, box.lx + prevHP, box.ry, this->prevPlayerHPColor, TRUE);
		DrawBox(box.lx, box.ly, box.lx + nowHP, box.ry, this->playerHPColor, TRUE);
	}
	//STAMINA
	{
		Vec2d position;
		Box box;
		int height;
		int indexBase = json.GetJson(JsonManager::FileType::UI)["PLAYER_INDEX_BASE"];
		int nowStamina =/* static_cast<int>(this->playerHP.GetNow() / this->playerHP.GetMax() * indexBase)*/indexBase;
		position.Set(json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_POSITION"]);
		height = json.GetJson(JsonManager::FileType::UI)["PLAYER_STAMINA_HEIGHT"];

		box.lx = position.x;
		box.ly = position.y;
		box.rx = box.lx + indexBase;
		box.ry = box.ly + height;

		DrawBox(box.lx, box.ly, box.lx + indexBase, box.ry, this->backgroundColor, TRUE);
		DrawBox(box.lx, box.ly, box.lx + nowStamina, box.ry, this->staminaColor, TRUE);
	}
	//BOSS
	{
		Vec2d position;
		Box box;
		int height;
		int indexBase = json.GetJson(JsonManager::FileType::UI)["BOSS_INDEX_BASE"];
		int nowHP = static_cast<int>(this->bossHP.GetNow() / this->bossHP.GetMax() * indexBase);
		int prevHP = static_cast<int>(this->bossHP.GetPrev() / this->bossHP.GetMax() * indexBase);
		position.Set(json.GetJson(JsonManager::FileType::UI)["BOSS_HP_POSITION"]);
		height = json.GetJson(JsonManager::FileType::UI)["BOSS_HP_HEIGHT"];

		box.lx = position.x;
		box.ly = position.y;
		box.rx = box.lx + indexBase;
		box.ry = box.ly + height;

		DrawBox(box.lx, box.ly, box.lx + indexBase, box.ry, this->backgroundColor, TRUE);
		DrawBox(box.lx, box.ly, box.lx + prevHP, box.ry, this->prevBossHPColor, TRUE);
		DrawBox(box.lx, box.ly, box.lx + nowHP, box.ry, this->bossHPColor, TRUE);
	}
}
/// <summary>
/// �͈͂̐ݒ�
/// </summary>
void HPUI::RangeNum::SetRange(const int _max, const int _min, const int _now)
{
	this->max = static_cast<float>(_max);
	this->min = static_cast<float>(_min);
	this->now = static_cast<float>(_now);
	this->prev = this->now;
}

/// <summary>
/// �͈͂̐ݒ�
/// </summary>
void HPUI::RangeNum::Add(const int _add)
{
	this->now += _add;
	if (this->now > this->max)
	{
		this->now = this->max;
	}
	if (this->now < this->min)
	{
		this->now = this->min;
	}
}
/// <summary>
/// HP�̃Z�b�g
/// </summary>
void HPUI::RangeNum::SetNow(const int _hp)
{
	this->now = static_cast<float>(_hp);
	if (this->now < this->min)
	{
		this->now = this->min;
	}
	if (this->now > this->max)
	{
		this->now = this->max;
	}
}
/// <summary>
/// �ߋ���HP������
/// </summary>
void HPUI::RangeNum::PrevDecrease()
{
	if (this->now < this->prev)
	{
		this->prev -= 0.5f;
	}
}

/// <summary>
/// �F�擾
/// </summary>
/// <param name="_color"></param>
int HPUI::ConvertColor(const std::vector<int> _color)
{
	return GetColor(_color[0], _color[1], _color[2]);
}