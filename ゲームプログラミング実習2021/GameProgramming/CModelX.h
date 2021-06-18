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
class CModelX; //�N���X�̐錾
class CMaterial;//�N���X�̐錾
/*
CSkinWeights
�X�L���E�F�C�g�N���X
*/
class CSkinWeights{
public:
	char*mpFrameName;  //�t���[����
	int mFrameIndex;  //�t���[���ԍ�
	int mIndexNum;  //���_�ԍ�
	int *mpIndex;  //���_�ԍ��z��
	float *mpWeight; //���_�E�F�C�g�z��
	CMatrix mOffset; //�I�t�Z�b�g�}�g���b�N�X

	CSkinWeights(CModelX*model);

	~CSkinWeights(){
		SAFE_DELETE_ARRAY(mpFrameName);
		SAFE_DELETE_ARRAY(mpIndex);
		SAFE_DELETE_ARRAY(mpWeight);
	}

};
//CMesh�N���X�̒�`
class CMesh{
public:
	int mVertexNum;
	CVector *mpVertex;
	int mFaceNum;
	int*mpVertexIndex;
	int mNormalNum;
	CVector*mpNormal;
	int mMaterialNum;//�}�e���A����
	int mMaterialIndexNum;//�}�e���A���ԍ�(�ʐ�)
	int*mpMaterialIndex;//�}�e���A���ԍ�
	std::vector<CMaterial*>mMaterial;//�}�e���A���f�[�^
	std::vector<CSkinWeights*>mSkinWeights;//�X�L���E�F�C�g
	//�R���X�g���N�^
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
	//�f�X�g���N�^
	~CMesh(){
		SAFE_DELETE_ARRAY(mpVertex);
		SAFE_DELETE_ARRAY(mpVertexIndex);
		SAFE_DELETE_ARRAY(mpNormal);
		SAFE_DELETE_ARRAY(mpMaterialIndex);
		//�X�L���E�F�C�g�̍폜
		for (int i = 0; i < mSkinWeights.size(); i++){
			delete mSkinWeights[i];
		}
	}
	void Render();
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
	void CModelXFrame::Render(){
		if (mMesh.mFaceNum != 0)
			mMesh.Render();
	}
};
/*
CModelX
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
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

	//�t�@�C���ǂݍ���
	void Load(char*file);
	//�P��̎��o��
	void GetToken();
	//�m�[�h�̓ǂݍ���
	void SkipNode();
	//���������_�f�[�^�̎擾
	float GetFloatToken();
	//�����f�[�^�̎擾
	int GetIntToken();
	/*
	Render
	�S�Ẵt���[���̕`�揈�����Ăяo��
	*/
	void CModelX::Render(){
		for (int i = 0; i < mFrame.size(); i++){
			mFrame[i]->Render();
		}
	}
};

#endif