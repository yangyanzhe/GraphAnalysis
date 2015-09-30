
// GraphAnalysisEngineDlg.h : 头文件
//
#include "Graph.h"
#pragma once


// CGraphAnalysisEngineDlg 对话框
class CGraphAnalysisEngineDlg : public CDialogEx
{
// 构造
public:
	CGraphAnalysisEngineDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GRAPHANALYSISENGINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeMfceditbrowse2();

public:
	char Inverted_infilePath[256];	
	char Inverted_outfilePath_MT[256];
	char Inverted_outfilePath_C[256];

	//从editBrowse中获得路径
	Graph app;
	// 输入路径
	CString infilePath;
	// 最小生成树的导出路径
	CString outfilePath_MT;
	// 中心度的导出路径
	CString outfilePath_C;
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	//afx_msg void OnBnClicked();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton7();
	//afx_msg void OnBnClickedButton8();
	int start;
	int end;
	int node;
	int edge;
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton1();
};
