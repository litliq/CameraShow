
// VideoDecodeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VideoDecode.h"
#include "VideoDecodeDlg.h"
#include "afxdialogex.h"
#include "opencv.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoDecodeDlg 对话框



CVideoDecodeDlg::CVideoDecodeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDecodeDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoDecodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COB_DIM, m_cbResol);
	DDX_Control(pDX, IDC_EDIT1, m_edtVideoPath);
	DDX_Control(pDX, IDC_EDIT2, m_edtSavePath);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
}

BEGIN_MESSAGE_MAP(CVideoDecodeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LOAD, &CVideoDecodeDlg::OnBnClickedBtnLoad)
	ON_BN_CLICKED(IDC_BUTTON2, &CVideoDecodeDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CVideoDecodeDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CVideoDecodeDlg 消息处理程序

BOOL CVideoDecodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_cbResol.AddString(_T("默认"));
	m_cbResol.AddString(_T("720P"));
	m_cbResol.AddString(_T("1080P"));
	m_cbResol.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVideoDecodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVideoDecodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoDecodeDlg::OnBnClickedBtnLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, _T("mp4|avi"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("Image Files(*.mp4;*.avi)|*.mp4;*.avi|All Files(*.*)|*.*||"), this);
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	CString sSavePath = dlg.GetPathName();
	m_edtVideoPath.SetWindowText(sSavePath);

	int iIndex = sSavePath.ReverseFind('.');
	if (iIndex > 0)
	{	
		m_edtSavePath.SetWindowText(sSavePath.Left(iIndex));
	}
}


void CVideoDecodeDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//图片路径
	BROWSEINFO bi = {0};
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("请选择要保存的图片路径：");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | 0x40;
	bi.lpfn = NULL;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (NULL == pidl)
	{
		return;
	}

	TCHAR szPath[MAX_PATH] = {0};
	SHGetPathFromIDList(pidl, szPath);

	m_edtSavePath.SetWindowText(szPath);
}


void CVideoDecodeDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sSavePath;
	m_edtSavePath.GetWindowText(sSavePath);
	if (sSavePath.IsEmpty())
	{
		MessageBox(_T("请先选择保存图片路径"));
		return;
	}
	if (!PathIsDirectory(sSavePath))
	{
		if (FALSE == CreateDirectory(sSavePath, 0))
		{
			MessageBox(_T("路径创建失败"));
			return;
		}		
	}

	CString sVideoPath;
	m_edtVideoPath.GetWindowText(sVideoPath);
	if (sVideoPath.IsEmpty())
	{
		MessageBox(_T("请先加载视频文件"));
		return;
	}
	if (!PathFileExists(sVideoPath))
	{
		MessageBox(_T("加载的视频文件不存在，请重新加载"));
		return;
	}

	cv::VideoCapture capture;     // 视频操作对象
	if (!capture.open((std::string)(_bstr_t)(LPCTSTR)sVideoPath))
	{
		MessageBox(_T("视频打开失败"));
		return;
	}

	int iCount = (int)capture.get(CV_CAP_PROP_FRAME_COUNT);
	m_ProgressCtrl.SetRange(0, iCount);

	int iFrameIndex = 0;
	while (true)
	{
		cv::Mat matFrame;
		capture >> matFrame;
		if (matFrame.empty())
		{
			break;
		}

		iFrameIndex++;
		m_ProgressCtrl.SetPos(iFrameIndex);
		CString sSavePic;
		sSavePic.Format(_T("%s\\%d.jpg"), sSavePath, iFrameIndex);

		cv::imwrite((std::string)(_bstr_t)(LPCTSTR)sSavePic, matFrame);
	}

	capture.release();
	
	CString sTip;
	sTip.Format(_T("解码完成，一共%d帧"), iFrameIndex);
	MessageBox(sTip);
}
