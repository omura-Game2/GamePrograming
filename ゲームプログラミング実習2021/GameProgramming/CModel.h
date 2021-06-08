#ifndef CMODEL_H
#define CMODEL_H
//vectorのインクルード
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include"CModelX.h"

/*
モデルクラス
モデルデータの入力や表示
*/
class CModel {
public:
	//頂点配列
	float *mpVertex; //頂点座標
	float *mpNormal; //法線
	float *mpTextureCoord; //テクスチャマッピング
	//デフォルトコンストラクタ
	CModel();
	//三角形の可変長配列
	std::vector<CTriangle> mTriangles;
	//マテリアルポインタの可変長配列
	std::vector<CMaterial*> mpMaterials;
	//テクスチャマッピングの保存(CVector型)
	std::vector<CVector> uv;

	~CModel();

	//モデルファイルの入力
	//Load(モデルファイル名, マテリアルファイル名)
	void Load(char *obj, char *mtl);
	//描画
	void Render();
	//描画
	//Render(行列)
	void Render(const CMatrix &m);
};
/*
CModelX
Xファイル形式の3Dモデルデータをプログラムで認識する
*/
class CModelX{
public:
	char*mpPointer;
	char mToken[1024];
	std::vector<CModelXFrame*>mFrame;

	CModelX()
		:mpPointer(0)
	{}

	~CModelX(){
		if (mFrame.size() > 0)
		{
			delete mFrame[0];
		}
	}

	//ファイル読み込み
	void Load(char*file);
	//単語の取り出し
	void GetToken();
	//ノードの読み込み
	void SkipNode();
	//浮動少数点データの取得
	float GetFloatToken();
	//整数データの取得
	int GetIntToken();
	/*
	Render
	全てのフレームの描画処理を呼び出す
	*/
	void CModelX::Render(){
		for (int i = 0; i < mFrame.size(); i++){
			mFrame[i]->Render();
		}
	}
};
#endif
