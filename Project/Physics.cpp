#include <cassert>
#include <DxLib.h>
#include <list>
#include "Mylib.h"

/// <summary>
/// 衝突物の登録
/// </summary>
void Physics::Entry(Collidable* _collidable)
{
	/*登録されているか調べる*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*登録されていなかったら*/
	if (!found)
	{
		collidables.emplace_back(_collidable);
	}
	/*登録されていたらエラーを吐く*/
	else
	{
		assert(0 && "指定のcollidableは登録済みです");
	}
}

/// <summary>
/// 衝突物の登録解除
/// </summary>
void Physics::Exit(Collidable* _collidable)
{
	/*登録されているか調べる*/
	bool found = (std::find(collidables.begin(), collidables.end(), _collidable) != collidables.end());

	/*登録されていたら*/
	if (!found)
	{
		collidables.remove(_collidable);
	}
	/*登録されていなかったらエラーを吐く*/
	else
	{
		assert(0 && "指定のcollidableは登録されていません");
	}

}

/// <summary>
/// 更新(登録オブジェクトの物理移動、衝突通知)
/// </summary>
void Physics::Update()
{
	// TODO:衝突情報の更新と通知
	for (auto& item : collidables)
	{
		//if(衝突したら)
		item->OnCollide();
	}
}