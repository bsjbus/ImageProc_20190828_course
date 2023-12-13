
// ImageProc_20190828View.cpp: CImageProc20190828View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImageProc_20190828.h"
#endif

#include "ImageProc_20190828Doc.h"
#include "ImageProc_20190828View.h"
#include <vfw.h>
#include "COpenCVDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageProc20190828View

IMPLEMENT_DYNCREATE(CImageProc20190828View, CScrollView)

BEGIN_MESSAGE_MAP(CImageProc20190828View, CScrollView)
	// 표준 인쇄 명령입니다.
	//ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	//ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_PIXEL_ADD, &CImageProc20190828View::OnPixelAdd)
	ON_COMMAND(ID_PIXEL_SUB, &CImageProc20190828View::OnPixelSub)
	ON_COMMAND(ID_PIXEL_MUL, &CImageProc20190828View::OnPixelMul)
	ON_COMMAND(ID_PIXEL_DIV, &CImageProc20190828View::OnPixelDiv)
	ON_COMMAND(ID_PIXEL_HISTO_EQ, &CImageProc20190828View::OnPixelHistoEq)
	ON_COMMAND(ID_PIXEL_CONTRAST_STRETCHING, &CImageProc20190828View::OnPixelContrastStretching)
	ON_COMMAND(ID_PIXEL_BINARIZATION, &CImageProc20190828View::OnPixelBinarization)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_ADD, &CImageProc20190828View::OnPixelTwoImageAdd)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_SUB, &CImageProc20190828View::OnPixelTwoImageSub)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_MUL, &CImageProc20190828View::OnPixelTwoImageMul)
	ON_COMMAND(ID_PIXEL_TWO_IMAGE_DIV, &CImageProc20190828View::OnPixelTwoImageDiv)
	ON_COMMAND(ID_REGION_SMOOTHING, &CImageProc20190828View::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_SHRAPENING, &CImageProc20190828View::OnRegionShrapening)
	ON_COMMAND(ID_REGION_EMBOSING, &CImageProc20190828View::OnRegionEmbosing)
	ON_COMMAND(ID_REGION_PREWITT, &CImageProc20190828View::OnRegionPrewitt)
	ON_COMMAND(ID_RREGION_ROBERTS, &CImageProc20190828View::OnRregionRoberts)
	ON_COMMAND(ID_RREGION_SOBEL, &CImageProc20190828View::OnRregionSobel)
	ON_COMMAND(ID_REGION_AVERAGE_FILTERING, &CImageProc20190828View::OnRegionAverageFiltering)
	ON_COMMAND(ID_REGION_MEDIAN_FILTERING, &CImageProc20190828View::OnRegionMedianFiltering)
	ON_COMMAND(ID_MOPOLOGY_COLOR_GRAY, &CImageProc20190828View::OnMopologyColorGray)
	ON_COMMAND(ID_MOPOLOGY_BINARIZATION, &CImageProc20190828View::OnMopologyBinarization)
	ON_COMMAND(ID_MOPOLOGY_EROSION, &CImageProc20190828View::OnMopologyErosion)
	ON_COMMAND(ID_MOPOLOGY_OPENING, &CImageProc20190828View::OnMopologyOpening)
	ON_COMMAND(ID_MOPOLOGY_DILATION, &CImageProc20190828View::OnMopologyDilation)
	ON_COMMAND(ID_MOPOLOGY_CLOSING, &CImageProc20190828View::OnMopologyClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXELCOPY, &CImageProc20190828View::OnGeometryZoominPixelcopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_BILINEARINTERPOLATION, &CImageProc20190828View::OnGeometryZoominBilinearinterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CImageProc20190828View::OnGeometryZoomoutSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEAN_SUB, &CImageProc20190828View::OnGeometryZoomoutMeanSub)
	ON_COMMAND(ID_GEOMETRY_AVG_FILTERING, &CImageProc20190828View::OnGeometryAvgFiltering)
	ON_COMMAND(ID_GEOMETRY_ROTATIONT, &CImageProc20190828View::OnGeometryRotationt)
	ON_COMMAND(ID_GEOMETRY_HOLIZANTAL_FLIP, &CImageProc20190828View::OnGeometryHolizantalFlip)
	ON_COMMAND(ID_GEOMETRY_VERTICAL_FLIP, &CImageProc20190828View::OnGeometryVerticalFlip)
	ON_COMMAND(ID_GEOMETRY_WARPING, &CImageProc20190828View::OnGeometryWarping)
	ON_COMMAND(ID_GEOMETRY_MOPING, &CImageProc20190828View::OnGeometryMoping)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_AVI_VIEW, &CImageProc20190828View::OnAviView)
	ON_COMMAND(ID_OPENCV, &CImageProc20190828View::OnOpencv)
END_MESSAGE_MAP()

// CImageProc20190828View 생성/소멸

CImageProc20190828View::CImageProc20190828View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

	bAviMode = false;

}

CImageProc20190828View::~CImageProc20190828View()
{
}

BOOL CImageProc20190828View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImageProc20190828View 그리기

void CImageProc20190828View::OnDraw(CDC* pDC)
{
	CImageProc20190828Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (bAviMode)// 동영상 재생
	{
		//재생
		LoadAviFile(pDC);
		bAviMode = false;
		return;
	}

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// pDC->TextOutA(100, 100, _T("테스트출력"));

	/*int x, y;

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
			pDC->SetPixel(x, y, RGB(pDoc -> inputImg[x][y], pDoc->inputImg[x][y] , pDoc->inputImg[x][y]));


	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
			pDC->SetPixel(256 + 20 + x, y, RGB(pDoc->resultImg[x][y], pDoc->resultImg[x][y], pDoc->resultImg[x][y]));

	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++)
			pDC->SetPixel((256 + 20)*2+ x, y, RGB(pDoc->inputImg2[x][y], pDoc->inputImg2[x][y], pDoc->inputImg2[x][y]));*/ //임의 크기 영상처리 이후에는 안씀

	
	int y,x;
	if (pDoc->inputImg != NULL)
	{
		if (pDoc->depth == 1)
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x, y, 
						RGB(pDoc->inputImg[y][x], 
						pDoc->inputImg[y][x], 
						pDoc->inputImg[y][x]));
		}
		else
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x, y, 
						RGB(pDoc->inputImg[y][3*x+0], 
						pDoc->inputImg[y][3 * x + 1], 
						pDoc->inputImg[y][3 * x + 2]));
		}
	}


	
	if (pDoc->inputImg2 != NULL)
	{
		if (pDoc->depth == 1)
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(2 * (pDoc->imageWidth + 20) + x, y, 
						RGB(pDoc->inputImg2[y][x], 
							pDoc->inputImg2[y][x], 
							pDoc->inputImg2[y][x]));
		}

		else {
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(2 * (pDoc->imageWidth + 20) + x, y,
						RGB(pDoc->inputImg2[y][3 * x + 0], 
							pDoc->inputImg2[y][3 * x + 1], 
							pDoc->inputImg2[y][3 * x + 2]));
		} 
	}


	if (pDoc->resultImg != NULL)
	{
		if (pDoc->depth == 1)
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 20, y,
						RGB(pDoc->resultImg[y][x],
							pDoc->resultImg[y][x],
							pDoc->resultImg[y][x]));
		}

		else
		{
			for (y = 0; y < pDoc->imageHeight; y++)
				for (x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 20, y,
						RGB(pDoc->resultImg[y][3 * x + 0],
							pDoc->resultImg[y][3 * x + 1],
							pDoc->resultImg[y][3 * x + 2]));
		}
	}

	if (pDoc->gresultImg != NULL)
	{
		if (pDoc->depth == 1)
		{
			for (y = 0; y < pDoc->gimageHeight; y++)
				for (x = 0; x < pDoc->gimageWidth; x++)
					pDC->SetPixel(x,pDoc->imageHeight + 20 + y,
						RGB(pDoc->gresultImg[y][x],
							pDoc->gresultImg[y][x],
							pDoc->gresultImg[y][x]));
		}

		else
		{
			for (y = 0; y < pDoc->gimageHeight; y++)
				for (x = 0; x < pDoc->gimageWidth; x++)
					pDC->SetPixel(x, pDoc->imageHeight + 20 + y,
						RGB(pDoc->gresultImg[y][3 * x + 0],
							pDoc->gresultImg[y][3 * x + 1],
							pDoc->gresultImg[y][3 * x + 2]));
		}
	}
}

void CImageProc20190828View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = 2048;// 내부 화면 사이즈 조절
	sizeTotal.cy = 1024;// 내부 화면 사이즈 조절
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImageProc20190828View 인쇄

BOOL CImageProc20190828View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageProc20190828View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageProc20190828View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CImageProc20190828View 진단

#ifdef _DEBUG
void CImageProc20190828View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProc20190828View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProc20190828Doc* CImageProc20190828View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProc20190828Doc)));
	return (CImageProc20190828Doc*)m_pDocument;
}
#endif //_DEBUG


// CImageProc20190828View 메시지 처리기




void CImageProc20190828View::OnPixelAdd()
{

	CImageProc20190828Doc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL)return;

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] + 50;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;

			//pDoc->resultImg[y][x] = pDoc->inputImg[y][x] + 50;

		}
	Invalidate(); // 데이터 수정한걸 다시 그리면서 화면에 나타남
	
}


void CImageProc20190828View::OnPixelSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();
	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] - 50;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}
	Invalidate();

}


void CImageProc20190828View::OnPixelMul()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();
	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] * 1.5;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}
	Invalidate();
}


void CImageProc20190828View::OnPixelDiv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();
	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x] / 1.5;
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}
	Invalidate();
}

//2주차------------------------------------------------------




void CImageProc20190828View::OnPixelHistoEq()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y, i, k;
	int acc_Hist;                     // 누적된 히스토그램 값
	int N = pDoc->imageHeight * pDoc->imageWidth;                  // N 전체 화소 수 
	int hist[256], sum[256];            // hist 히스토그램 밝기 sum 누적분포

	for (i = 0; i < 256; i++) hist[i] = 0;  // 히스토그램 초기화

	// 히스토그램 구하기
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			k = pDoc->inputImg[y][x];
			hist[k]++;                     // 히스토그램 처리
		}

	// 누적분포
	acc_Hist = 0;
	for (i = 0; i < 256; i++) {            // 히스토그램 각 밝기 값을 sum에 저장
		acc_Hist += hist[i];
		sum[i] = acc_Hist;
	}

	// 평활화 화소값 저장
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			k = pDoc->inputImg[y][x];
			pDoc->resultImg[y][x] = (float)sum[k] / N * 255; // float 캐스트 연산자로 소수점을 구해야함 안그럼 0과 1 흰 검으로 나타남
		}
	Invalidate();


}


void CImageProc20190828View::OnPixelContrastStretching()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y;
	int minVal = 255;
	int maxVal = 0;

	// 입력 이미지에서 최소값과 최대값을 찾습니다.
	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			int pixelValue = pDoc->inputImg[y][x];
			if (pixelValue < minVal) {
				minVal = pixelValue;
			}
			if (pixelValue > maxVal) {
				maxVal = pixelValue;
			}
		}
	}

	// 명암 대비 스트레칭을 적용하여 결과 이미지를 생성합니다.
	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			int pixelValue = pDoc->inputImg[y][x];
			int stretchedValue = 255 * (pixelValue - minVal) / (maxVal - minVal);

			// 범위를 벗어난 값은 조정합니다.
			if (stretchedValue > 255) {
				stretchedValue = 255;
			}
			else if (stretchedValue < 0) {
				stretchedValue = 0;
			}
			pDoc->resultImg[y][x] = stretchedValue;
		}
	}
	Invalidate();

}




void CImageProc20190828View::OnPixelBinarization() // 이진화
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();


	int x, y, value;
	int threshold = 128;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++) {
			value = pDoc->inputImg[y][x];                     // 인풋 이미지의 픽셀을 받아옴
			if (value > threshold)   pDoc->resultImg[y][x] = 255;   // 받은 각 픽셀의 값이 128보다 크면 흰
			else               pDoc->resultImg[y][x] = 0;      // 픽셀의 값이 128보다 같거나 작으면 검
		}
	Invalidate();

}

//3주차----------------------------------------------------------------------------------------------------

void CImageProc20190828View::LoadTwoImage()
{
	// TODO: 여기에 구현 코드 추가.
	CImageProc20190828Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);
	CFile file;

	if (dlg.DoModal() == IDOK) {	// DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소

		file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
		CArchive ar(&file, CArchive::load);
		ar.Read(pDoc->inputImg2, 256 * 256);
		//file.Read(pDoc->inputImg2, 256 * 256);			// 파일 읽어서 inputImage2에 삽입
		file.Close();									// 파일 닫기

		int x, y;
		for (y = 0; y < 256; y++) {
			for (x = 0; x < 256; x++) {
				pDoc->resultImg[y][x] = pDoc->inputImg[y][x];	// 결과 이미지를 초기화
			}
		}
		Invalidate();
		// dlg.GetPathName();	// 경로명 파일이름 확장자 등 다 넘김
		// dlg.GetFileName();	// 파일 이름만 넘김
		// dlg.GetFileExt();	// 파일 경로만 넘김
		// dlg.GetFileTitle();	// 파일 확장자만 넘김
	}
}


void CImageProc20190828View::OnPixelTwoImageAdd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++) {
			value = 0.7 * pDoc->inputImg[y][x] + 0.3 * pDoc->inputImg2[y][x];	// 0.* 수정해서 해당 이미지 선명도를 높임
			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정
			pDoc->resultImg[y][x] = value;
		}

	Invalidate();
}


void CImageProc20190828View::OnPixelTwoImageSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			value = abs(pDoc->inputImg[y][x] - pDoc->inputImg2[y][x]);	// abs로 절댓값을 구함

			//if (value > 64) value = 255;		// 이진화시킴 (아래 코드 필요없어짐)
			//else value = 0;

			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정
			pDoc->resultImg[y][x] = value;
		}
	Invalidate();

}


void CImageProc20190828View::OnPixelTwoImageMul()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) 
		{
			value = pDoc->inputImg[y][x] * pDoc->inputImg2[y][x];	// abs로 절댓값을 구함

			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정

			pDoc->resultImg[y][x] = value;
		}
	Invalidate();
}


void CImageProc20190828View::OnPixelTwoImageDiv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	CFileDialog dlg(TRUE);

	if (dlg.DoModal() != IDOK) // DoModal 호출하면 뜸 // IDOK = 확인 // IDCANCEL == 취소
		return;

	CFile file;
	file.Open(dlg.GetPathName(), CFile::modeRead); // 파일 열어서 읽기모드
	CArchive ar(&file, CArchive::load);
	//LoadTwoImage();		// 새로운 이미지 불러오기
	pDoc->LoadSecondImageFile(ar);
	file.Close();

	int x, y, value;
	for (y = 0; y < 256; y++)
		for (x = 0; x < 256; x++) {
			value = (float)pDoc->inputImg[y][x] / pDoc->inputImg2[y][x];	// abs로 절댓값을 구함

			if (value > 255) value = 255;		// 픽셀 값이 255가 넘어가면 255로 고정
			else if (value < 0) value = 0;		//  " 0보다 작아지면 0으로 고정

			pDoc->resultImg[y][x] = value;
		}
	Invalidate();
}


//영역처리기반----------------------


void CImageProc20190828View::Convolve(unsigned char** Inimg, unsigned char** Outimg, int cols, int rows, float mask[][3], int bias, int depth)
{
	// TODO: 여기에 구현 코드 추가.


	int x, y, value, i, j;
	float sum, redsum, greensum, bluesum;

	for (y = 1; y < rows - 1; y++)
	{		// 테두리 부분 계산 의미 없으므로
		for (x = 1; x < cols - 1; x++)
		{
			if (depth == 1)
			{				// 흑백일 때
				sum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						sum += Inimg[y - 1 + j][x - 1 + i] * mask[j][i];
					}
					sum += bias;
					if (sum > 255) sum = 255;
					else if (sum < 0)sum = 0;
					Outimg[y][x] = sum;
				
			}
			else
			{						// 컬러일 때
				redsum = greensum = bluesum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						redsum += Inimg[y - 1 + j][3 * (x - 1 + i) + 0] * mask[j][i];
						greensum += Inimg[y - 1 + j][3 * (x - 1 + i) + 1] * mask[j][i];
						bluesum += Inimg[y - 1 + j][3 * (x - 1 + i) + 2] * mask[j][i];
					}
				redsum += bias;
				greensum += bias;
				bluesum += bias;

				if (redsum > 255) redsum = 255;
				else if (redsum < 0)redsum = 0;
				if (greensum > 255) greensum = 255;
				else if (greensum < 0)greensum = 0;
				if (bluesum > 255) bluesum = 255;
				else if (bluesum < 0)bluesum = 0;

				Outimg[y][3 * x + 0] = redsum;
				Outimg[y][3 * x + 1] = greensum;
				Outimg[y][3 * x + 2] = bluesum;

			}
		}
	}
};



void CImageProc20190828View::OnRegionSmoothing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	float kernel[3][3] = { {1 / 9.0,1 / 9.0,1 / 9.0},
						   {1 / 9.0,1 / 9.0,1 / 9.0},
						   {1 / 9.0,1 / 9.0,1 / 9.0} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);
	Invalidate();
}



void CImageProc20190828View::OnRegionShrapening()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	float kernel[3][3] = { {0,-1,0},
						   {-1,5,-1},
				           {0,-1,0} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();

}



void CImageProc20190828View::OnRegionEmbosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.


	CImageProc20190828Doc* pDoc = GetDocument();
	float kernel[3][3] = { {-1, 0, 0},
						   {0,  0, 0},
						   {0,  0, 1} };
	Convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);

	Invalidate();
	
}


//5주차---------------------------------------------



void CImageProc20190828View::OnRegionPrewitt()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	float Hmask[3][3] = { {-1, -1, -1},
						   {0,  0, 0},
						   {1,  1, 1} }; //위 아래 달라지면 다르게 나옴


	float Vmask[3][3] = { {1,  0, -1},
						  {1,  0, -1},
						  {1,  0, -1} };// 앞뒤 바뀌어도 상관은 없다


	unsigned char** Er, ** Ec;

	int x, y, value;

	//메모리 할당
	Er= (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec= (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	Convolve(pDoc->inputImg, Er, pDoc->imageWidth, pDoc->imageHeight,Hmask, 0, pDoc->depth);//영역기반처리
	Convolve(pDoc->inputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, Vmask, 0, pDoc->depth);

	for(y= 0; y < pDoc -> imageHeight; y++ )
		for (x = 0; x < pDoc->imageWidth *pDoc -> depth; x++)
		{
			value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]); //연산해서 합침
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}

	//메모리 할당 해제
	for (int i = 0; i < pDoc->imageHeight; i++)
	{
		free(Er[i]);	
		free(Ec[i]);
	}
	free(Er);	
	free(Ec);

	Invalidate();
}


void CImageProc20190828View::OnRregionRoberts()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	float Hmask[3][3] = { {-1, 0, 0},
						   {0,  1, 0},
						   {0,  0, 0} }; //위 아래 달라지면 다르게 나옴


	float Vmask[3][3] = { {0, 0, -1},
						   {0,  1, 0},
						   {0,  0, 0} };;// 앞뒤 바뀌어도 상관은 없다


	unsigned char** Er, ** Ec;

	int x, y, value;

	//메모리 할당
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	Convolve(pDoc->inputImg, Er, pDoc->imageWidth, pDoc->imageHeight, Hmask, 0, pDoc->depth);//영역기반처리
	Convolve(pDoc->inputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, Vmask, 0, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++)
		{
			value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]); //연산해서 합침
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}

	//메모리 할당 해제
	for (int i = 0; i < pDoc->imageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void CImageProc20190828View::OnRregionSobel()
{
	CImageProc20190828Doc* pDoc = GetDocument();

	float Hmask[3][3] = { {-1, -2, -1},
						   {0,  0, 0},
						   {1,  2, 1} }; //위 아래 달라지면 다르게 나옴


	float Vmask[3][3] = { {1,  0, -1},
						  {2,  0, 2},
						  {1,  0, -1} };// 앞뒤 바뀌어도 상관은 없다


	unsigned char** Er, ** Ec;

	int x, y, value;

	//메모리 할당
	Er = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->imageHeight * sizeof(unsigned char*));

	for (int i = 0; i < pDoc->imageHeight; i++) {
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		Er[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->imageWidth * pDoc->depth);
	}

	Convolve(pDoc->inputImg, Er, pDoc->imageWidth, pDoc->imageHeight, Hmask, 0, pDoc->depth);//영역기반처리
	Convolve(pDoc->inputImg, Ec, pDoc->imageWidth, pDoc->imageHeight, Vmask, 0, pDoc->depth);

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth * pDoc->depth; x++)
		{
			value = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]); //연산해서 합침
			if (value > 255) value = 255;
			else if (value < 0) value = 0;
			pDoc->resultImg[y][x] = value;
		}

	//메모리 할당 해제
	for (int i = 0; i < pDoc->imageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}

// 잡음제거----------------------------------------

void CImageProc20190828View::OnRegionAverageFiltering() // 컬러처리 안함 영상에있으니 나중에 하기
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();
	int x, y, i, j;
	int xcor, ycor;
	int sum, count, rsum, gsum, bsum;		// 합과 몇개 필터

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				sum = 0;
				count = 0;
				for (j = -2; j <= 2; j++)
					for (i = -2; i <= 2; i++)
					{
						ycor = y + j;
						xcor = x + i;
						if (0 <= ycor && ycor <= pDoc->imageHeight - 1 && 0 <= xcor && xcor <= pDoc->imageWidth - 1)
						{
							sum += pDoc->inputImg[ycor][xcor];
							count++;
						}

					}
				pDoc->resultImg[y][x] = sum / count;
			}
			else
			{
				rsum = bsum = gsum = 0;
				count = 0;
				for (j = -2; j <= 2; j++)
					for (i = -2; i <= 2; i++)
					{
						ycor = y + j;
						xcor = x + i;
						if (0 <= ycor && ycor <= pDoc->imageHeight - 1 && 0 <= xcor && xcor <= pDoc->imageWidth - 1)
						{
							rsum += pDoc->inputImg[ycor][3 * xcor + 0];
							gsum += pDoc->inputImg[ycor][3 * xcor + 1];
							bsum += pDoc->inputImg[ycor][3 * xcor + 2];
							count++;
						}

					}
				pDoc->resultImg[y][3 * x + 0] = rsum / count;
				pDoc->resultImg[y][3 * x + 1] = gsum / count;
				pDoc->resultImg[y][3 * x + 2] = bsum / count;
			}
		}
	Invalidate();
}


void CImageProc20190828View::OnRegionMedianFiltering() //잡음 더 생김 다시 해보기
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y, i, j;
	int n[9], temp;


	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++)
		{
			if (pDoc->depth == 1) {
				n[0] = pDoc->inputImg[y - 1][x -1];
				n[1] = pDoc->inputImg[y - 1][x -0];
				n[2] = pDoc->inputImg[y - 1][x +1];
				n[3] = pDoc->inputImg[y - 0][x -1];
				n[4] = pDoc->inputImg[y - 0][x -0];
				n[5] = pDoc->inputImg[y - 0][x +1];
				n[6] = pDoc->inputImg[y + 1][x -1];
				n[7] = pDoc->inputImg[y + 1][x-0];
				n[8] = pDoc->inputImg[y + 1][x+1];

				//sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1])
						{
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][x] = n[4];
			}
			else {
				n[0] = pDoc->inputImg[y - 1][3 * (x - 1) + 0];
				n[1] = pDoc->inputImg[y - 1][3 * (x - 0) + 0];
				n[2] = pDoc->inputImg[y - 1][3 * (x + 1) + 0];
				n[3] = pDoc->inputImg[y - 0][3 * (x - 1) + 0];
				n[4] = pDoc->inputImg[y - 0][3 * (x - 0) + 0];
				n[5] = pDoc->inputImg[y - 0][3 * (x + 1) + 0];
				n[6] = pDoc->inputImg[y + 1][3 * (x - 1) + 0];
				n[7] = pDoc->inputImg[y + 1][3 * (x - 0) + 0];
				n[8] = pDoc->inputImg[y + 1][3 * (x + 1) + 0];

				//sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1])
						{
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][3 * x + 0] = n[4];

				n[0] = pDoc->inputImg[y - 1][3 * (x - 1) + 1];
				n[1] = pDoc->inputImg[y - 1][3 * (x - 0) + 1];
				n[2] = pDoc->inputImg[y - 1][3 * (x + 1) + 1];
				n[3] = pDoc->inputImg[y - 0][3 * (x - 1) + 1];
				n[4] = pDoc->inputImg[y - 0][3 * (x - 0) + 1];
				n[5] = pDoc->inputImg[y - 0][3 * (x + 1) + 1];
				n[6] = pDoc->inputImg[y + 1][3 * (x - 1) + 1];
				n[7] = pDoc->inputImg[y + 1][3 * (x - 0) + 1];
				n[8] = pDoc->inputImg[y + 1][3 * (x + 1) + 1];

				//sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1])
						{
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][3 * x + 1] = n[4];

				n[0] = pDoc->inputImg[y - 1][3 * (x - 1) + 2];
				n[1] = pDoc->inputImg[y - 1][3 * (x - 0) + 2];
				n[2] = pDoc->inputImg[y - 1][3 * (x + 1) + 2];
				n[3] = pDoc->inputImg[y - 0][3 * (x - 1) + 2];
				n[4] = pDoc->inputImg[y - 0][3 * (x - 0) + 2];
				n[5] = pDoc->inputImg[y - 0][3 * (x + 1) + 2];
				n[6] = pDoc->inputImg[y + 1][3 * (x - 1) + 2];
				n[7] = pDoc->inputImg[y + 1][3 * (x - 0) + 2];
				n[8] = pDoc->inputImg[y + 1][3 * (x + 1) + 2];

				//sorting
				for (i = 8; i > 0; i--)
					for (j = 0; j < i; j++)
						if (n[j] > n[j + 1])
						{
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
				pDoc->resultImg[y][3 * x + 2] = n[4];
			}

		}
	Invalidate();
}


void CImageProc20190828View::OnMopologyColorGray()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	if (pDoc -> depth == 1) 
		return;

	int x, y;
	int gray;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			gray = (pDoc->inputImg[y][3 * x + 0] + pDoc->inputImg[y][3 * x + 1] + pDoc->inputImg[y][3 * x + 2]) / 3;
			pDoc->inputImg[y][3 * x + 0] = gray;
			pDoc->inputImg[y][3 * x + 1] = gray;
			pDoc->inputImg[y][3 * x + 2] = gray;
		}
	Invalidate();
}


void CImageProc20190828View::OnMopologyBinarization()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y;
	int thresh =128;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
			{
				if (pDoc->inputImg[y][x] > thresh)
					pDoc->inputImg[y][x] = 255;
				else
					pDoc->inputImg[y][x] = 0;
			}
			else
				if (pDoc->inputImg[y][3 * x + 0] + pDoc->inputImg[y][3 * x + 1] + pDoc->inputImg[y][3 * x + 2] / 3 > thresh)
				{
					pDoc->inputImg[y][3 * x + 0] = 255;
					pDoc->inputImg[y][3 * x + 1] = 255;
					pDoc->inputImg[y][3 * x + 2] = 255;
				}
				else
				{
					pDoc->inputImg[y][3 * x + 0] = 0;
					pDoc->inputImg[y][3 * x + 1] = 0;
					pDoc->inputImg[y][3 * x + 2] = 0;
				}
		}
	Invalidate();
}


void CImageProc20190828View::OnMopologyErosion()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y, i, j;
	int min,rmin,gmin,bmin = 255;

	for (y = 1; y < pDoc->imageHeight-1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++)
		{
			if (pDoc->depth == 1)
			{
				min = 255;
				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++)
					{
						if (pDoc->inputImg[y + j][x + i] < min)
							min = pDoc->inputImg[y + j][x + i];
					}
				pDoc->resultImg[y][x] = min;
			}
			else
			{
				rmin = 255;
				gmin = 255;
				bmin = 255;
				
				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++)
					{
						if (pDoc->inputImg[y + j][3 * (x + i) + 0] < rmin)
							rmin = pDoc->inputImg[y + j][3 * (x + i) + 0];


						if (pDoc->inputImg[y + j][3 * (x + i) + 0] < gmin)
							gmin = pDoc->inputImg[y + j][3 * (x + i) + 0];

						if (pDoc->inputImg[y + j][3 * (x + i) + 0] < bmin)
							bmin = pDoc->inputImg[y + j][3 * (x + i) + 0];
					}
				pDoc->resultImg[y][3 * x + 0] = rmin;
				pDoc->resultImg[y][3 * x + 1] = gmin;
				pDoc->resultImg[y][3 * x + 2] = bmin;
			}
			
		}
	Invalidate();
}


void CImageProc20190828View::OnMopologyDilation()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y, i, j;
	int max, rmax, gmax, bmax = 0;

	for (y = 1; y < pDoc->imageHeight - 1; y++)
		for (x = 1; x < pDoc->imageWidth - 1; x++)
		{
			if (pDoc->depth == 1)
			{
				max = 0;
				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++)
					{
						if (pDoc->inputImg[y + j][x + i] > max)
							max = pDoc->inputImg[y + j][x + i];
					}
				pDoc->resultImg[y][x] = max;
			}
			else
			{
				rmax = 0;
				gmax = 0;
				bmax = 0;

				for (j = -1; j <= 1; j++)
					for (i = -1; i <= 1; i++)
					{
						if (pDoc->inputImg[y + j][3 * (x + i) + 0] > rmax)
							rmax = pDoc->inputImg[y + j][3 * (x + i) + 0];


						if (pDoc->inputImg[y + j][3 * (x + i) + 0] > gmax)
							gmax = pDoc->inputImg[y + j][3 * (x + i) + 0];

						if (pDoc->inputImg[y + j][3 * (x + i) + 0] > bmax)
							bmax = pDoc->inputImg[y + j][3 * (x + i) + 0];
					}
				pDoc->resultImg[y][3 * x + 0] = rmax;
				pDoc->resultImg[y][3 * x + 1] = gmax;
				pDoc->resultImg[y][3 * x + 2] = bmax;
			}

		}
	Invalidate();
}


void CImageProc20190828View::OnMopologyOpening() //침식연산
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();

	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
}


void CImageProc20190828View::CopyResultToInput() 
{
	// TODO: 여기에 구현 코드 추가.

	CImageProc20190828Doc* pDoc = GetDocument();
	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth*pDoc->depth; x++)
		{
			pDoc->inputImg[y][x] = pDoc->resultImg[y][x];
		}
}


void CImageProc20190828View::OnMopologyClosing()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();
	OnMopologyDilation();
	CopyResultToInput();

	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
	CopyResultToInput();
	OnMopologyErosion();
}

//기하학적 처리 ---------------------------------------------------------------

void CImageProc20190828View::OnGeometryZoominPixelcopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y;
	int xscale = 3;
	int yscale = 2; // 정수배만 되고 숫자 달라져도 상관없음

	if (pDoc->gresultImg != NULL) // 할당삭제, 새로 생성 
	{
		for (int i = 0; i < pDoc ->gimageHeight; i++)
			free(pDoc->gresultImg[i]);
		free(pDoc-> gresultImg);
	}

	pDoc->gimageWidth = pDoc->imageWidth * xscale;
	pDoc->gimageHeight = pDoc->imageHeight * yscale;

	//메모리할당
	pDoc -> gresultImg = (unsigned char**)malloc(pDoc -> gimageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc -> gimageHeight; i++)
	{
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		pDoc->gresultImg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc-> depth);
	}

	
	for (y = 0; y < pDoc->gimageHeight; y++)
		for (x = 0; x < pDoc -> gimageWidth; x++) 
		{
			if(pDoc-> depth==1)
			pDoc->gresultImg[y][x] = pDoc->inputImg[y / yscale][x/xscale];

			else 
			{
				pDoc->gresultImg[y][3 * x + 0] = pDoc->inputImg[y / yscale][3 * (x / xscale) + 0];
				pDoc->gresultImg[y][3 * x + 1] = pDoc->inputImg[y / yscale][3 * (x / xscale) + 1];
				pDoc->gresultImg[y][3 * x + 2] = pDoc->inputImg[y / yscale][3 * (x / xscale) + 2];
			}
		}

	//전방향사상
/*
	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			pDoc->gresultImg[y * yscale][x * xscale] = pDoc->inputImg[x][y];
		}
*/

	Invalidate();
}


void CImageProc20190828View::OnGeometryZoominBilinearinterpolation() 
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y;
	float xscale = 2.5;
	float yscale = 1.5; // 정수배만 되고 숫자 달라져도 상관없음
	float src_x, src_y;
	float alpha, beta;
	int Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
	int E, F;

	if (pDoc->gresultImg != NULL) // 할당삭제, 새로 생성 
	{
		for (int i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gresultImg[i]);
		free(pDoc->gresultImg);
	}

	pDoc->gimageWidth = pDoc->imageWidth * xscale + 0.5;
	pDoc->gimageHeight = pDoc->imageHeight * yscale + 0.5;

	//메모리할당
	pDoc->gresultImg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gimageHeight; i++)
	{
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		pDoc->gresultImg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}

	//역방향 사상
	for (y = 0; y < pDoc->gimageHeight; y++)
		for (x = 0; x < pDoc->gimageWidth; x++)
		{
			src_x = x / xscale;
			src_y = y / yscale;
			alpha = src_x - (int)src_x;
			beta = src_y - (int)src_y;

			Ax = (int)src_x;
			Ay = (int)src_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if (Bx > pDoc->imageWidth - 1) Bx = pDoc->imageWidth - 1;
			if (Cy > pDoc->imageHeight - 1) Cy = pDoc->imageHeight - 1;
			if (Dx > pDoc->imageWidth - 1) Dx = pDoc->imageWidth - 1;
			if (Dy > pDoc->imageHeight - 1) Dy = pDoc->imageHeight - 1;

			if (pDoc->depth == 1)
			{
				E = (1 - alpha) * pDoc->inputImg[Ay][Ax] + alpha * pDoc->inputImg[By][Bx];
				F = (1 - alpha) * pDoc->inputImg[Cy][Cx] + alpha * pDoc->inputImg[Dy][Dx];

				pDoc->gresultImg[y][x] = (1 - beta) * E + beta * F;
			}

			else
			{
				E = (1 - alpha) * pDoc->inputImg[Ay][3 * Ax + 0] + alpha * pDoc->inputImg[By][3 * Bx + 0];
				F = (1 - alpha) * pDoc->inputImg[Cy][3 * Cx + 0] + alpha * pDoc->inputImg[Dy][3 * Dx + 0];
				pDoc->gresultImg[y][3 * x + 0] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputImg[Ay][3 * Ax + 1] + alpha * pDoc->inputImg[By][3 * Bx + 1];
				F = (1 - alpha) * pDoc->inputImg[Cy][3 * Cx + 1] + alpha * pDoc->inputImg[Dy][3 * Dx + 1];
				pDoc->gresultImg[y][3 * x + 1] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputImg[Ay][3 * Ax + 2] + alpha * pDoc->inputImg[By][3 * Bx + 2];
				F = (1 - alpha) * pDoc->inputImg[Cy][3 * Cx + 2] + alpha * pDoc->inputImg[Dy][3 * Dx + 2];
				pDoc->gresultImg[y][3 * x + 2] = (1 - beta) * E + beta * F;

			}

		}

	Invalidate();
}

//10주차--------------------------------------------------------------

void CImageProc20190828View::OnGeometryZoomoutSubsampling()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProc20190828Doc* pDoc = GetDocument();


	int xscale = 2;// 1/3
	int yscale = 2; // 1/2     // 정수배만 되고 숫자 달라져도 상관없음
	int x, y;

	if (pDoc->gresultImg != NULL) // 할당삭제, 새로 생성 
	{
		for (int i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gresultImg[i]);
		free(pDoc->gresultImg);
	}

	pDoc->gimageWidth = pDoc->imageWidth / xscale;
	pDoc->gimageHeight = pDoc->imageHeight / yscale ;

	//메모리 할당
	pDoc->gresultImg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gimageHeight; i++)
	{
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		pDoc->gresultImg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}

	//역방향사상
	for (y = 0; y < pDoc->gimageHeight; y++)
		for (x = 0; x < pDoc->gimageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->gresultImg[y][x] = pDoc->inputImg[y * yscale][x * xscale];

			else
			{
				pDoc->gresultImg[y][3 * x + 0] = pDoc->inputImg[y * yscale][3 * (x * xscale) +0 ];
				pDoc->gresultImg[y][3 * x + 1] = pDoc->inputImg[y * yscale][3 * (x * xscale) +1 ];
				pDoc->gresultImg[y][3 * x + 2] = pDoc->inputImg[y * yscale][3 * (x * xscale) +2 ];

			}
		}
	Invalidate();
}




void CImageProc20190828View::OnGeometryZoomoutMeanSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	OnRegionSmoothing();
	CopyResultToInput();
	OnGeometryZoomoutSubsampling();
}


void CImageProc20190828View::OnGeometryAvgFiltering() 
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.


	CImageProc20190828Doc* pDoc = GetDocument();


	int xscale = 2;// 1/3
	int yscale = 2; // 1/2     // 정수배만 되고 숫자 달라져도 상관없음
	int x, y,i,j;
	int sum,rsum,gsum,bsum;
	int src_x, src_y;


	if (pDoc->gresultImg != NULL) // 할당삭제, 새로 생성 
	{
		for (int i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gresultImg[i]);
		free(pDoc->gresultImg);
	}

	pDoc->gimageWidth = pDoc->imageWidth / xscale;
	pDoc->gimageHeight = pDoc->imageHeight / yscale;

	//메모리 할당
	pDoc->gresultImg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gimageHeight; i++)
	{
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		pDoc->gresultImg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}

	//전방향사상
	for (y = 0; y < pDoc->imageHeight; y+= yscale)
		for (x = 0; x < pDoc->imageWidth; x+= xscale)
		{
			if (pDoc->depth == 1)
			{
				sum = 0;
				for (j = 0; j < yscale; j++)
					for (i = 0; i < xscale; i++)
					{
						src_x = x + i;
						src_y = y + j;

						sum += pDoc->inputImg[src_y][src_x];
					}
				pDoc->gresultImg[y / yscale][x / xscale] = sum / (xscale * yscale);
			}

			else
			{
				rsum = 0; 
				gsum = 0; 
				bsum = 0;
				for (j = 0; j < yscale; j++)
					for (i = 0; i < xscale; i++)
					{
						src_x = x + i;
						src_y = y + j;

						rsum += pDoc->inputImg[src_y][3 * src_x + 0];
						gsum += pDoc->inputImg[src_y][3 * src_x + 1];
						bsum += pDoc->inputImg[src_y][3 * src_x + 2];
					}
				pDoc->gresultImg[y / yscale][3 * (x / xscale) + 0] = rsum / (xscale * yscale);
				pDoc->gresultImg[y / yscale][3 * (x / xscale) + 1] = gsum / (xscale * yscale);
				pDoc->gresultImg[y / yscale][3 * (x / xscale) + 2] = bsum / (xscale * yscale);
			}
		}
	Invalidate();
}


#define PI 3.1416926521
#include"CAngleDialog.h"
void CImageProc20190828View::OnGeometryRotationt()// 컬러 처리 안함 + 튕김
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.


	CImageProc20190828Doc* pDoc = GetDocument();
	CAngleDialog dlg;

	int angle = 30; //dgree 단위로
	float radian; 
	int Hy;
	int Cx, Cy;
	int x,y, xdiff, ydiff;
	int x_source, y_source;


	dlg.m_iAngle = angle; // Dialog에 뜨는 기본값이 30이 됨
	if (dlg.DoModal() == IDCANCEL) return;
	angle = dlg.m_iAngle;



	radian = PI / 180 * angle;
	// y의 마지막
	Hy = pDoc->imageHeight - 1;
	// 중심점
	Cx = pDoc->imageWidth / 2;
	Cy = pDoc->imageHeight / 2;


	if (pDoc->gresultImg != NULL) // 할당삭제, 새로 생성 
	{
		for (int i = 0; i < pDoc->gimageHeight; i++)
			free(pDoc->gresultImg[i]);
		free(pDoc->gresultImg);
	}

	pDoc->gimageWidth = pDoc->imageHeight * fabs(cos(PI / 2 - radian)) + pDoc->imageWidth * fabs(cos(radian));   
	// 회전했을 때 이미지 크기
	pDoc->gimageHeight = pDoc->imageHeight * fabs(cos(radian)) + pDoc->imageWidth * fabs(cos(PI / 2 - radian));   
	// radian = 세타 // fabs 양수로 만듬


	//메모리 할당
	pDoc->gresultImg = (unsigned char**)malloc(pDoc->gimageHeight * sizeof(unsigned char*));
	for (int i = 0; i < pDoc->gimageHeight; i++)
	{
		//inputImg[] : unsigned char 포인터 // malloc(imageWidth * depth) 포인터의 값
		pDoc->gresultImg[i] = (unsigned char*)malloc(pDoc->gimageWidth * pDoc->depth);
	}
	
	xdiff = (pDoc->gimageWidth - pDoc->imageWidth) / 2;
	ydiff = (pDoc->gimageHeight - pDoc->imageHeight) / 2;


	for (y = -ydiff; y < pDoc->gimageHeight - ydiff; y++)
		for (x = -xdiff; x < pDoc->gimageWidth - xdiff; x++) 
		{
			x_source = (Hy - y - Cx) * sin(radian) + (x - Cx) * cos(radian) + Cx;
			y_source = Hy - ((Hy - y - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);

			if (pDoc->depth == 1) 
			{
				if (x_source<0 || x_source>pDoc->imageWidth - 1 || y_source<0 || y_source>pDoc->imageHeight - 1)
					pDoc->gresultImg[y + ydiff][x + xdiff] = 255;
				else

					pDoc->gresultImg[y + ydiff][x + xdiff] = pDoc->inputImg[y_source][x_source];
			}
			else {
				if (x_source<0 || x_source>pDoc->imageWidth - 1 || y_source<0 || y_source>pDoc->imageHeight - 1) {
					pDoc->gresultImg[y + ydiff][3 * (x + xdiff) + 0] = 255;
					pDoc->gresultImg[y + ydiff][3 * (x + xdiff) + 1] = 255;
					pDoc->gresultImg[y + ydiff][3 * (x + xdiff) + 2] = 255;
				}
				else {
					pDoc->gresultImg[y + ydiff][3 * (x + xdiff) + 0] = pDoc->inputImg[y_source][3 * x_source + 0];
					pDoc->gresultImg[y + ydiff][3 * (x + xdiff) + 1] = pDoc->inputImg[y_source][3 * x_source + 1];
					pDoc->gresultImg[y + ydiff][3 * (x + xdiff) + 2] = pDoc->inputImg[y_source][3 * x_source + 2];
				}
			}
		}
	Invalidate();
}


void CImageProc20190828View::OnGeometryHolizantalFlip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->resultImg[y][x] = pDoc->inputImg[y][pDoc->imageWidth - 1 - x];

			else
				pDoc->resultImg[y][3 * x + 0] = pDoc->inputImg[y][3 * (pDoc->imageWidth - 1 - x) + 0]; //r
				pDoc->resultImg[y][3 * x + 1] = pDoc->inputImg[y][3 * (pDoc->imageWidth - 1 - x) + 1]; //g
				pDoc->resultImg[y][3 * x + 2] = pDoc->inputImg[y][3 * (pDoc->imageWidth - 1 - x) + 2]; //b

		}
	Invalidate();
}



void CImageProc20190828View::OnGeometryVerticalFlip()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CImageProc20190828Doc* pDoc = GetDocument();

	int x, y;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->resultImg[y][x] = pDoc->inputImg[pDoc ->imageHeight-1- y][x];

			else
				pDoc->resultImg[y][3 * x + 0] = pDoc->inputImg[pDoc->imageHeight - 1 - y][3 * x + 0];//r
				pDoc->resultImg[y][3 * x + 1] = pDoc->inputImg[pDoc->imageHeight - 1 - y][3 * x + 1]; //g
				pDoc->resultImg[y][3 * x + 2] = pDoc->inputImg[pDoc->imageHeight - 1 - y][3 * x + 2];//b

		}
	Invalidate();
}


//영상 워핑-------------------------------


typedef struct // 새로 만든 자료형 typedef를 붙여주면 됨
{
	int Px;
	int Py;
	int Qx;
	int Qy;
}control_line;

control_line mctrl_source = { 100,100,150,150 };
control_line mctrl_dest = { 100,100,200,200 };

void CImageProc20190828View::OnGeometryWarping()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageProc20190828Doc* pDoc = GetDocument();

	control_line source_lines[5] = { {100,100,150,150},
		{0,0,pDoc->imageWidth - 1,0},{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1},
		{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1},{0,pDoc->imageHeight - 1,0,0} };

	control_line dest_lines[5] = { {100,100,200,200},
		{0,0,pDoc->imageWidth - 1,0},{pDoc->imageWidth - 1,0,pDoc->imageWidth - 1,pDoc->imageHeight - 1},
		{pDoc->imageWidth - 1,pDoc->imageHeight - 1,0,pDoc->imageHeight - 1},{0,pDoc->imageHeight - 1,0,0} };

	source_lines[0] = mctrl_source;
	dest_lines[0] = mctrl_dest;

	int x, y;
	double u; //수직 교차점의 위치
	double h; // 제어선으로부터 픽셀의 수직 변위
	double d; //제어선하고 픽셀 사이의 거리
	double tx, ty; // 결과영상 픽셀에 대응되는 입력 영상 픽셀 사이의 변위의 합
	double xp, yp; //각 제어선에 대해 계산된 입력 영ㅇ상의 대응되는 픽셀 위치
	double weight; //각 제어선의 가중치
	double totalweight; //가중치의 합
	double a = 0.001; //0으로 나눠지는 것을 막기 위함
	double b = 2.0;
	double p = 0.75;

	int x1, y1, x2, y2; // 1번은 시작점 2번은 종료점
	int src_x1, src_y1, src_x2, src_y2;
	double src_line_length, dest_line_length; // 이동전 이동후 저장

	int num_lines = 5; //제어선의 개수
	int line;
	int source_x, source_y;
	int last_row, last_col;

	last_col = pDoc->imageWidth - 1;
	last_row = pDoc->imageHeight - 1;

	for (y = 0; y < pDoc->imageHeight; y++)
		for (x = 0; x < pDoc->imageWidth; x++)
		{
			tx = 0.0;
			ty = 0.0;
			totalweight = 0.0;


			for (line = 0; line < num_lines; line++)
			{
				x1 = dest_lines[line].Px;
				y1 = dest_lines[line].Py;
				x2 = dest_lines[line].Qx;
				y2 = dest_lines[line].Qy;

				dest_line_length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				u = (double) ((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
					(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

				h = ((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

				if (u < 0)       d = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
				else if (u > 1)  d = sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));
				else             d = fabs(h);

				src_x1 = source_lines[line].Px;
				src_y1 = source_lines[line].Py;
				src_x2 = source_lines[line].Qx;
				src_y2 = source_lines[line].Qy;

				src_line_length = sqrt((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1));


				xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
				yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

				weight = pow(pow(dest_line_length, p) / (a + d), b);

				tx += (xp - x) * weight;
				ty += (yp - y) * weight;
				totalweight += weight;
			}

			source_x = x + (tx / totalweight);
			source_y = y + (ty / totalweight);

			if (source_x< 0)         source_x = 0;
			if (source_x> last_col)  source_x = last_col;
			if (source_y < 0)        source_y = 0;
			if (source_y > last_row) source_y = last_row;
			
			if (pDoc->depth == 1)
				pDoc->resultImg[y][x] = pDoc->inputImg[source_y][source_x];
			else
			{
				pDoc->resultImg[y][3 * x + 0] = pDoc->inputImg[source_y][3 * source_x + 0];
				pDoc->resultImg[y][3 * x + 1] = pDoc->inputImg[source_y][3 * source_x + 1];
				pDoc->resultImg[y][3 * x + 2] = pDoc->inputImg[source_y][3 * source_x + 2];
			}
		}
	Invalidate();
}





void CImageProc20190828View::OnGeometryMoping()
{

	CImageProc20190828Doc* pDoc = GetDocument();

}

CPoint mpos_st, mpos_end;
void CImageProc20190828View::OnLButtonDown(UINT nFlags, CPoint point) // 마우스 클릭
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	mpos_st = point;
	CScrollView::OnLButtonDown(nFlags, point);
}


void CImageProc20190828View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	mpos_end = point;

	CDC* pDC = GetDC();
	CPen rpen;

	rpen.CreatePen(PS_SOLID, 0, RGB(255, 0, 0));
	pDC->SelectObject(&rpen);


	pDC->MoveTo(mpos_st);
	pDC->LineTo(mpos_end);
	ReleaseDC(pDC);

	int Ax, Ay, Bx, By;
	Ax = mpos_st.x;
	Ay = mpos_st.y;
	Bx = mpos_end.x;
	By = mpos_end.y;

	if (Ax < Bx) mctrl_source.Px = Ax - (Bx - Ax) / 2;
	else		 mctrl_source.Px = Ax + (Bx - Ax) / 2;

	if (Ay < By) mctrl_source.Py = Ay - (By - Ay) / 2;
	else		 mctrl_source.Py = Ay + (By - Ay) / 2;

	mctrl_dest.Px = mctrl_source.Px;
	mctrl_dest.Py = mctrl_source.Py;
	
	mctrl_source.Qx = mpos_st.x;
	mctrl_source.Qy = mpos_st.y;
	mctrl_dest.Qx = mpos_end.x;
	mctrl_dest.Qy = mpos_end.y;



	CScrollView::OnLButtonUp(nFlags, point);
}


void CImageProc20190828View::OnAviView()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CFileDialog dlg(true,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		"AVI파일(*avi)|*.avi|모든파일|*.*|");
	
	if (dlg.DoModal() == IDOK)
	{
		AviFileName = dlg.GetPathName();
		bAviMode = true;
		Invalidate();
	}
}


void CImageProc20190828View::LoadAviFile(CDC* pDC)
{
	// TODO: 여기에 구현 코드 추가.
	PAVIFILE pavi;
	AVIFILEINFO fi;
	int stm;
	PAVISTREAM pstm = NULL;
	AVISTREAMINFO si;
	PGETFRAME pfrm = NULL;
	int frame;
	LPBITMAPINFOHEADER pbmpih;
	unsigned char* image;
	int x, y;

	AVIFileInit();
	AVIFileOpen(&pavi, AviFileName, OF_READ | OF_SHARE_DENY_NONE, NULL);
	AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));

	for (stm = 0; stm < fi.dwStreams; stm++)
	{
		AVIFileGetStream(pavi, &pstm, 0, stm);
		AVIStreamInfo(pstm, &si, sizeof(si));
		if (si.fccType == streamtypeVIDEO)
		{
			pfrm = AVIStreamGetFrameOpen(pstm, NULL);
			for (frame = 0; frame < si.dwLength; frame++)
			{
				pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
				if (!pbmpih) continue;

				image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);
				/*
				for (y = 0; y < fi.dwHeight; y++)
					for (x = 0; x < fi.dwWidth; x++)
					{
						pDC->SetPixel(x, fi.dwHeight - 1 - y,
							RGB(image[(y * fi.dwWidth + x) * 3 + 2],
								image[(y * fi.dwWidth + x) * 3 + 1],
								image[(y * fi.dwWidth + x) * 3 + 0]));
						
					}
				*/

				::SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, fi.dwWidth, fi.dwHeight,
					0, 0, 0, fi.dwWidth, image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
				Sleep(33);
			}
		}
	}
	AVIStreamGetFrameClose(pfrm);
	AVIStreamRelease(pstm);
	AVIFileRelease(pavi);
	AVIFileExit();
}

//--------------------------------------------------------------------------
void CImageProc20190828View::OnOpencv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	COpenCVDlg dlg;

	dlg.DoModal();
}
