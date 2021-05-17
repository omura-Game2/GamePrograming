#ifndef CMODELX_H
#define CMODELX_H

#define MODEL_FILE "sample.blend.x"

//�̈������}�N����
#define SAFE_DELETE_ARRAY(a){if(a)delete[]a;a=0;}

#include<vector>
#include"CMatrix.h"
#include"CVector.h"
//�z��̃T�C�Y�擾���}�N����
#define ARRAY_SIZE(a)(sizeof(a)/sizeof(a[0]))
class CModelX; 
//CMesh�N���X�̒�`
class CMesh{
public:
	int mVertexNum;
	CVector *mpVertex;
	int mFaceNum;
	int*mpVertexIndex;
	int mNormalNum;
	CVector*mpNormal;
	//�R���X�g���N�^
	CMesh()
		:mVertexNum(0)
		, mpVertex(0)
		, mFaceNum(0)
		, mpVertexIndex(nullptr)
		, mNormalNum(0)
		, mpNormal(nullptr)
	{}
	//�f�X�g���N�^
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
	}
	//�ǂݍ��ݏ���
	void Init(CModelX*model);
};
class CModelXFrame{
public:
	std::vector<CModelXFrame*>mChild;
	CMatrix mTransformMatrix;
	char*mpName;
	int mIndex;
	CMesh mMesh;
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame(){
		//�q�t���[�������ׂĉ������
		std::vector<CModelXFrame*>::iterator itr;
		for (itr = mChild.begin(); itr != mChild.end(); itr++){
			delete *itr;
		}
		//���O�̃G���A���������
		SAFE_DELETE_ARRAY(mpName);
	}
};

#endif