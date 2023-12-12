
// ImageProc_20190828Doc.h: CImageProc20190828Doc 클래스의 인터페이스
//


#pragma once


class CImageProc20190828Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CImageProc20190828Doc() noexcept;
	DECLARE_DYNCREATE(CImageProc20190828Doc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CImageProc20190828Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	


	// Doc 파일 우클릭 -> 추가 -> 변수추가
public:
	// 입력이미지


	//unsigned char inputImg[256][256];	//[y][x] 입력
	//unsigned char inputImg2[256][256];	// 이미지 변수 255*255 크기
	//unsigned char resultImg[256][256]; //출력

	unsigned char** inputImg;
	unsigned char** inputImg2;
	unsigned char** resultImg;

	int imageWidth;
	int imageHeight;
	int depth;			// 흑백 1, 칼라 3 저장 됨

	int gimageWidth;
	int gimageHeight; // 가로세로 확대 축소

	unsigned char** gresultImg; //[y][x] g이미지에서 할당해서 넣어줘야됨

	void LoadImageFile(CArchive& ar);
	void LoadSecondImageFile(CArchive& ar);

	unsigned char** morphedImg[10];
};
