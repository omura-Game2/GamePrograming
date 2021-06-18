#include"glut.h"
#include<stdio.h>
#include"CModelX.h"
#include<string.h>
#include"CModel.h"
#include"CMatrix.h"
#include"CMaterial.h"

void CModelX::Load(char*file){
	//
	//ファイルサイズを取得する
	//
	FILE*fp;
	fp = fopen(file, "rb");
	if (fp == NULL){
		printf("fopen error:%s\n", file);
		return;
	}
	//ファイルの最後へ移動
	fseek(fp, 0L, SEEK_END);
	//ファイルサイズの取得
	int size = ftell(fp);
	//ファイルサイズ+1バイト分の領域を確保
	char*buf = mpPointer = new char[size + 1];
	//
	//ファイルから3Dモデルのデータを読み込む
	//
	//ファイルの先頭へ移動
	fseek(fp, 0L, SEEK_SET);
	//確保した領域にファイルサイズ分データを読み込む
	fread(buf, size, 1, fp);
	//最後に\0を設定する(文字列の終端)
	buf[size] = '\0';
	fclose(fp);

	//文字列の最後まで繰り返し
	while (*mpPointer != '\0'){
		GetToken();
		//単語がFrameの場合
		if (strcmp(mToken, "Frame") == 0){
			//フレームを作成する
			new CModelXFrame(this);
		}

	}

	SAFE_DELETE_ARRAY(buf);
}
void CModelX::GetToken(){
	char* p = mpPointer;
	char* q = mToken;
	//空白()タブ(\t)改行(\r)(\n),;"以外の文字になるまで読み飛ばす
	/*
	strchr(文字列、文字)
	文字列に文字が含まれていれば、見つかった文字へのポインタを返す
	見つからなかったらNULLを返す
	*/
	while (*p != '\0' && strchr(" \t\r\n,;\"", *p))p++;
	if (*p == '{' || *p == '}'){
		//{または}ならmTokenに代入し次の文字へ
		*q++ = *p++;
	}

	else{
		//空白()タブ(\t)改行(\r)(\n),;"}以外の文字になるまでmToken代入する
		while (*p != '\0' && !strchr(" \t\r\n,;\"}", *p))
			*q++ = *p++;
	}
	*q = '\0';
	mpPointer = p;

	//もしmTokenが//の場合は、コメントなので改行まで読み飛ばす
	/*
	strcmp(文字列1,文字列2)
	文字列1と文字列2が等しい場合、0を返します
	文字列1と文字列2が等しくない場合、0以外を返します
	*/
	if (!strcmp("//", mToken)){
		//改行まで読み飛ばす
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//読み込み位置の更新
		mpPointer = p;
		//単語を取得する(再帰呼び出し)
		GetToken();
	}
}
/*
SkipNode
ノードを読み飛ばす
*/
void CModelX::SkipNode(){
	//文字が終わったら終了
	while (*mpPointer != '\0'){
		GetToken();
		//{が見つかったらループ終了
		if (strchr(mToken, '{'))break;
	}
	int count = 1;
	//文字が終わるか、カウントが０になったら終了
	while (*mpPointer != '\0'&& count > 0){
		GetToken();
		//{を見つけるとカウントアップ
		if (strchr(mToken, '{'))count++;
		//}を見つけるとカウントダウン
		else if (strchr(mToken, '}'))count--;
	}
}
/*
CModelXFrame
model:CModelXインスタンスへのポインタ
フレームを作成する
読み込み中にFrameが見つかれば、フレームを作成し、
子フレームに追加する
*/
CModelXFrame::CModelXFrame(CModelX*model){
	//現在のフレーム配列の要素数を取得し設定する
	mIndex = model->mFrame.size();
	//CModelXのフレーム配列に追加する
	model->mFrame.push_back(this);
	//変換行列を単位行列にする
	mTransformMatrix.Identity();
	//次の単語(フレーム名の予定)を取得する
	model->GetToken();
	//フレーム名分エリアを確保する
	mpName = new char[strlen(model->mToken) + 1];
	//フレーム名をコピーする
	strcpy(mpName, model->mToken);
	//次の単語({の予定})を取得する
	model->GetToken(); //{
	//文字がなくなったら終わり
	while (*model->mpPointer != '\0'){
		//次の単語取得
		model->GetToken();//Frame
		//}かっこの場合は終了
		if (strchr(model->mToken, '}'))break;
		//新なフレームの場合は、子フレームに追加
		if (strcmp(model->mToken, "Frame") == 0){
			//フレームを作成し、子フレームの配列に追加
			mChild.push_back(
				new CModelXFrame(model));
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0){
			model->GetToken();//{
			for (int i = 0; i < ARRAY_SIZE(mTransformMatrix.mF); i++){
				mTransformMatrix.mF[i] = model->GetFloatToken();
			}
			model->GetToken();//}
		}
		else if (strcmp(model->mToken, "Mesh") == 0){
			mMesh.Init(model);
		}
		else{
			//上記以外の要素は読み飛ばす
			model->SkipNode();
		}
	}
	//デバッグバージョンのみ有効
#ifdef _DEBUG
	printf("%s\n", mpName);
	mTransformMatrix. Print();
#endif
}
/*
GetFloatToken
単語を浮動小数点型のデータで返す
*/
float CModelX::GetFloatToken(){
	GetToken();
	//atof
	//文字列をfloat型へ変換
	return atof(mToken);
}
/*
GetIntToken
単語を整数型のデータで返す
*/
int CModelX::GetIntToken(){
	GetToken();
	return atof(mToken);
}
/*
CSkinWeights
スキンウェイトの読み込み
*/
CSkinWeights::CSkinWeights(CModelX*model)
: mpFrameName(0)
, mFrameIndex(0)
, mIndexNum(0)
, mpIndex(nullptr)
, mpWeight(nullptr)
{
	model->GetToken();  //{
	model->GetToken();  //FrameName
	//フレーム名エリア確保、設定
	mpFrameName = new char[strlen(model->mToken) + 1];
	strcpy(mpFrameName, model->mToken);
	//頂点番号数取得
	mIndexNum = model->GetIntToken();
	//頂点番号数が0を超える
	if (mIndexNum > 0){
		//頂点番号と頂点ウェイトのエリア確保
		mpIndex = new int[mIndexNum];
		mpWeight = new float[mIndexNum];
		//頂点番号取得
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = model->GetIntToken();
		//頂点ウェイト取得
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = model->GetFloatToken();
	}
	//オフセット行列取得
	for (int i = 0; i < 16; i++){
		mOffset.mF[i] = model->GetFloatToken();
	}
	model->GetToken();   //}

#ifndef _DEBUG
	printf("SkinWeights:%d\n", mpFrameName);
	for (int i = 0; i < mIndexNum; i++){
		printf("%d", mpIndex[i]);
		printf("%10f\n", mpWeight[i]);
	}
	mOffset.Print();
#endif
}
/*
Init
Meshのデータを取り込む
*/
void CMesh::Init(CModelX*model){
	model->GetToken();
	if (!strchr(model->mToken, '{')){
		//名前の場合、次が{
		model->GetToken(); //{
	}
	//頂点数の取得
	mVertexNum = model->GetIntToken();
	//頂点数分エリアを確保
	mpVertex = new CVector[mVertexNum];
	//頂点数分データを取り込む
	for (int i = 0; i < mVertexNum; i++){
		mpVertex[i].mX = model->GetFloatToken();
		mpVertex[i].mY = model->GetFloatToken();
		mpVertex[i].mZ = model->GetFloatToken();
	}
	printf("VertexNum,%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++)
	{
		printf("%10f", mpVertex[i].mX);
		printf("%10f", mpVertex[i].mY);
		printf("%10f\n", mpVertex[i].mZ);
	}
	mFaceNum = model->GetIntToken();  //面数読み込み
	//頂点数は1面に3頂点
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3){
		model->GetToken(); //頂点数読み飛ばす
		mpVertexIndex[i] = model->GetIntToken();
		mpVertexIndex[i + 1] = model->GetIntToken();
		mpVertexIndex[i + 2] = model->GetIntToken();
	}
	printf("FaceNum,%d\n", mFaceNum);
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		printf("%10d", mpVertexIndex[i]);
		printf("%10d", mpVertexIndex[i + 1]);
		printf("%10d\n", mpVertexIndex[i + 2]);
	}
//文字がなくなったら終わり
while (model->mpPointer != '\0'){
	model->GetToken();   //MeshNormals
	//}かっこの場合は終了
	if (strchr(model->mToken, '}'))
		break;
	if (strcmp(model->mToken, "MeshNormals") == 0){
		model->GetToken();
		//法線データ数を取得
		mNormalNum = model->GetIntToken();
		//法線のデータを配列に取り込む
		CVector*pNormal = new CVector[mNormalNum];
		for (int i = 0; i < mNormalNum; i++){
			pNormal[i].mX = model->GetFloatToken();
			pNormal[i].mY = model->GetFloatToken();
			pNormal[i].mZ = model->GetFloatToken();
		}
		//法線数=面数x3
		mNormalNum = model->GetIntToken() * 3;//FanceNum
		int ni;
		//頂点毎に法線データを設定する
		mpNormal = new CVector[mNormalNum];
		for (int i = 0; i < mNormalNum; i += 3){
			model->GetToken();//3
			ni = model->GetIntToken();
			mpNormal[i] = pNormal[ni];

			ni = model->GetIntToken();
			mpNormal[i + 1] = pNormal[ni];

			ni = model->GetIntToken();
			mpNormal[i + 2] = pNormal[ni];
		}
		delete[] pNormal;
		model->GetToken();   //}
	}
		//MashMaterialListのとき
		else if (strcmp(model->mToken, "MeshMaterialList") == 0){
			model->GetToken();//{
			//Materialの数
			mMaterialNum = model->GetIntToken();
			//FaceNum
			mMaterialIndexNum = model->GetIntToken();
			//マテリアルインデックスの作成
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i< mMaterialIndexNum; i++){
				mpMaterialIndex[i] = model->GetIntToken();
			}
			//マテリアルデータの作成
			for (int i = 0; i < mMaterialNum; i++){
				model->GetToken();  //Material
				if (strcmp(model->mToken, "Material") == 0){
					mMaterial.push_back(new CMaterial(model));
				}
			}
			model->GetToken();  //}End of MeshMaterialList
		}
	//SkinWeightsの時
	else if (strcmp(model->mToken, "SkinWeights") == 0){
		//CSkinWeightsクラスのインスタンスを作成し、配列に追加
		mSkinWeights.push_back(new CSkinWeights(model));
	}
	else{
		//以外のノードは読み飛ばし
		model->SkipNode();
	}
	printf("NormalNum:%d\n", mNormalNum);
	for (int i = 0; i < mNormalNum; i++)
	{
		printf("%10f", mpNormal[i].mX);
		printf("%10f", mpNormal[i].mY);
		printf("%10f\n", mpNormal[i].mZ);
	}
  }
}
void CMesh::Render(){
	/*頂点データ、法線データの配列を有効にする*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/*頂点データ、法線データの場所を指定する*/
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);

	/*頂点インデックスの場合を指定して図形を描画する*/
	for (int i = 0; i < mFaceNum; i++){
		//マテリアルを適用する
	mMaterial[mpMaterialIndex[i]]->Enabled();
	glDrawElements(GL_TRIANGLES, 3 , 
		GL_UNSIGNED_INT, (mpVertexIndex +i*3));
	}
	/*頂点データ、法線データの配列を無効にする*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}
