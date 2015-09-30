
// GraphAnalysisEngineDlg.h : ͷ�ļ�
//
#include "Graph.h"
#pragma once


// CGraphAnalysisEngineDlg �Ի���
class CGraphAnalysisEngineDlg : public CDialogEx
{
// ����
public:
	CGraphAnalysisEngineDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GRAPHANALYSISENGINE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	//��editBrowse�л��·��
	Graph app;
	// ����·��
	CString infilePath;
	// ��С�������ĵ���·��
	CString outfilePath_MT;
	// ���Ķȵĵ���·��
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
