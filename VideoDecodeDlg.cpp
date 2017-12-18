
// VideoDecodeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VideoDecode.h"
#include "VideoDecodeDlg.h"
#include "afxdialogex.h"
#include "opencv.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVideoDecodeDlg �Ի���



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


// CVideoDecodeDlg ��Ϣ�������

BOOL CVideoDecodeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_cbResol.AddString(_T("Ĭ��"));
	m_cbResol.AddString(_T("720P"));
	m_cbResol.AddString(_T("1080P"));
	m_cbResol.SetCurSel(0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CVideoDecodeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CVideoDecodeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVideoDecodeDlg::OnBnClickedBtnLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//ͼƬ·��
	BROWSEINFO bi = {0};
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("��ѡ��Ҫ�����ͼƬ·����");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sSavePath;
	m_edtSavePath.GetWindowText(sSavePath);
	if (sSavePath.IsEmpty())
	{
		MessageBox(_T("����ѡ�񱣴�ͼƬ·��"));
		return;
	}
	if (!PathIsDirectory(sSavePath))
	{
		if (FALSE == CreateDirectory(sSavePath, 0))
		{
			MessageBox(_T("·������ʧ��"));
			return;
		}		
	}

	CString sVideoPath;
	m_edtVideoPath.GetWindowText(sVideoPath);
	if (sVideoPath.IsEmpty())
	{
		MessageBox(_T("���ȼ�����Ƶ�ļ�"));
		return;
	}
	if (!PathFileExists(sVideoPath))
	{
		MessageBox(_T("���ص���Ƶ�ļ������ڣ������¼���"));
		return;
	}

	cv::VideoCapture capture;     // ��Ƶ��������
	if (!capture.open((std::string)(_bstr_t)(LPCTSTR)sVideoPath))
	{
		MessageBox(_T("��Ƶ��ʧ��"));
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
	sTip.Format(_T("������ɣ�һ��%d֡"), iFrameIndex);
	MessageBox(sTip);
}
