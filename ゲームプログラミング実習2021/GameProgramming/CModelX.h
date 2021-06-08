#ifndef CMODELX_H
#define CMODELX_H

#define MODEL_FILE "sample.blend.x"

//領域解放をマクロ化
#define SAFE_DELETE_ARRAY(a){if(a)delete[]a;a=0;}

#include<vector>
#include"CMatrix.h"
#include"CVector.h"
//配列のサイズ取得をマクロ化
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
class CModelX; //クラスの宣言
class CMaterial;//クラスの宣言
//CMeshクラスの定義
class CMesh{
public:
	int mVertexNum;
	CVector *mpVertex;
	int mFaceNum;
	int*mpVertexIndex;
	int mNormalNum;
	CVector*mpNormal;
	int mMaterialNum;//マテリアル数
	int mMaterialIndexNum;//マテリアル番号(面数)
	int*mpMaterialIndex;//マテリアル番号
	std::vector<CMaterial*>mMaterial;//マテリアルデータ
	//コンストラクタ
	CMesh()
		:mVertexNum(0)
		, mpVertex(0)
		, mFaceNum(0)
		, mpVertexIndex(nullptr)
		, mNormalNum(0)
		, mpNormal(nullptr)
		, mMaterialNum(0)
		, mMaterialIndexNum(0)
		, mpMaterialIndex(nullptr)
	{}
	//デストラクタ
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
	}
	void Render();
	//読み込み処理
	void Init(CModelX*model);
};

class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild;
	CMatrix mTransformMatrix;
	char*mpName;
	int mIndex;
	CMesh mMesh;
	//コンストラクタ
	CModelXFrame(CModelX* model);
	//デストラクタ
	~CModelXFrame(){
		//子フレームをすべて解放する
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++){
			delete *itr;
		}
		//名前のエリアを解放する
		SAFE_DELETE_ARRAY(mpName);
	}
	void CModelXFrame::Render(){
		if (mMesh.mFaceNum != 0)
			mMesh.Render();
	}
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