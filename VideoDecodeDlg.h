
// VideoDecodeDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CVideoDecodeDlg 对话框
class CVideoDecodeDlg : public CDialogEx
{
// 构造
public:
	CVideoDecodeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_VIDEODECODE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	DECLARE_MESSAGE_MAP()


private:
	
	CComboBox m_cbResol;
	CEdit m_edtVideoPath;
	CEdit m_edtSavePath;	
	CProgressCtrl m_ProgressCtrl;
};
