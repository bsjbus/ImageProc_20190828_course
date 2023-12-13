// COpenCVDlg.cpp: 구현 파일
//

#include "pch.h"
#include "ImageProc_20190828.h"
#include "afxdialogex.h"
#include "COpenCVDlg.h"


// COpenCVDlg 대화 상자

IMPLEMENT_DYNAMIC(COpenCVDlg, CDialogEx)

COpenCVDlg::COpenCVDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_OPENCV, pParent)
{

}

COpenCVDlg::~COpenCVDlg()
{
}

void COpenCVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COpenCVDlg, CDialogEx)
END_MESSAGE_MAP()


// COpenCVDlg 메시지 처리기
