
// GraphAnalysisEngineDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GraphAnalysisEngine.h"
#include "GraphAnalysisEngineDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGraphAnalysisEngineDlg �Ի���




CGraphAnalysisEngineDlg::CGraphAnalysisEngineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphAnalysisEngineDlg::IDD, pParent)
	, outfilePath_MT(_T("nginx-1.4.4/nginx-1.4.4/html/��С����ɭ��.json"))
	, outfilePath_C(_T("���Ķ�.txt"))
	, infilePath(_T(""))
	, start(0)
	, end(0)
	, node(0)
	, edge(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for(int i = 0; i<256; i++)
	{
		Inverted_infilePath[i] = '\0';
		Inverted_outfilePath_MT[i] = '\0';
		Inverted_outfilePath_C[i] = '\0';
	}
}

void CGraphAnalysisEngineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT4, outfilePath_MT);
	DDX_Text(pDX, IDC_EDIT2, outfilePath_C);
	DDX_Text(pDX, IDC_MFCEDITBROWSE2, infilePath);
	DDX_Text(pDX, IDC_EDIT1, start);
	DDX_Text(pDX, IDC_EDIT3, end);
	DDX_Text(pDX, IDC_EDIT5, node);
	DDX_Text(pDX, IDC_EDIT6, edge);
}

BEGIN_MESSAGE_MAP(CGraphAnalysisEngineDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE2, &CGraphAnalysisEngineDlg::OnEnChangeMfceditbrowse2)
	ON_EN_CHANGE(IDC_EDIT4, &CGraphAnalysisEngineDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT2, &CGraphAnalysisEngineDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON2, &CGraphAnalysisEngineDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CGraphAnalysisEngineDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CGraphAnalysisEngineDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CGraphAnalysisEngineDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON3, &CGraphAnalysisEngineDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON7, &CGraphAnalysisEngineDlg::OnBnClickedButton7)
	//ON_BN_CLICKED(IDC_BUTTON8, &CGraphAnalysisEngineDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON8, &CGraphAnalysisEngineDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON1, &CGraphAnalysisEngineDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGraphAnalysisEngineDlg ��Ϣ�������

BOOL CGraphAnalysisEngineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CGraphAnalysisEngineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CGraphAnalysisEngineDlg::OnPaint()
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
HCURSOR CGraphAnalysisEngineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ȡ�����ļ�·���Ŀؼ�
void CGraphAnalysisEngineDlg::OnEnChangeMfceditbrowse2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	GetDlgItemText(IDC_MFCEDITBROWSE2, infilePath);
	int size = WideCharToMultiByte(CP_ACP, 0, infilePath.GetBuffer(), 
		-1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, infilePath.GetBuffer(), -1, Inverted_infilePath, 
		size, NULL, NULL);
	MessageBox(_T("ͼ���������У����򲻻������>.<�����Ե�..."));
	app.setInFile(Inverted_infilePath);
	//nΪ��ʼ�㣬��n < 0����������С����ɭ��
	app.printMinSpanningTree("nginx-1.4.4/nginx-1.4.4/html/MinSpanningTree.json");
	//���ӽ����ĶȺͽ������Ķȵ�����TXT��API
	app.printCentralityToTXT("���Ķ�.txt");
	//���ӽ����ĶȺͽ������Ķȵ�����JSON��API
	app.printBCentralityToJSON("nginx-1.4.4/nginx-1.4.4/html/BetweennessCentrality.json");
	app.printBCentralityToJSON("nginx-1.4.4/nginx-1.4.4/html/BetweennessCentrality.json");
	node = app.node;
	edge = app.edge;
	UpdateData(false);
	MessageBox(_T("�����Ȼû�б���^_^��ͼ������ɣ�"));
}

//��С����������·��������ؼ�
void CGraphAnalysisEngineDlg::OnEnChangeEdit4()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//���Ķȵ���·��������ؼ�
void CGraphAnalysisEngineDlg::OnEnChangeEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//��С����������ĵ���
void CGraphAnalysisEngineDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);
	int size = WideCharToMultiByte(CP_ACP, 0, outfilePath_MT.GetBuffer(), 
		-1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, outfilePath_MT.GetBuffer(), -1, Inverted_outfilePath_MT, 
		size, NULL, NULL);
	app.setOutFile(Inverted_outfilePath_MT);
 	//������С��������json�ļ�
 	app.exportMinSpanningTree();
}

//����127.0.0.1չʾ���
void CGraphAnalysisEngineDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/Minimum_Spanning_Tree.html"), NULL, NULL, SW_SHOWNORMAL);
}

//���Ķ�txt�ļ��ĵ���
void CGraphAnalysisEngineDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	int size = WideCharToMultiByte(CP_ACP, 0, outfilePath_C.GetBuffer(), 
		-1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, outfilePath_C.GetBuffer(), -1, Inverted_outfilePath_C, 
		size, NULL, NULL);
	app.setOutFile(Inverted_outfilePath_C);
	//�����������Ķ�����
	app.exportCentralityToTXT();
}


//�ӽ����Ķȣ�����json�ļ�������ɴ򿪲���
void CGraphAnalysisEngineDlg::OnBnClickedButton6()
{
	app.setOutFile("nginx-1.4.4/nginx-1.4.4/html/ClosenessCentrality.json");
	app.exportBCentralityToJSON();
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/ClosenessCentrality.html"), NULL, NULL, SW_SHOWNORMAL);
}

//�������Ķȣ�����json�ļ�������ɴ򿪲���
void CGraphAnalysisEngineDlg::OnBnClickedButton7()
{
	app.setOutFile("nginx-1.4.4/nginx-1.4.4/html/BetweennessCentrality.json");
	app.exportCCentralityToJSON();
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/BetweennessCentrality.html"), NULL, NULL, SW_SHOWNORMAL);
}

//ʹ��˵��
void CGraphAnalysisEngineDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL, _T("open"), _T("˵��.txt"), NULL, NULL, SW_SHOWNORMAL);
}

/*
//�����ļ�
void CGraphAnalysisEngineDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}*/

//�����С·�������ɺ����ݵ����
void CGraphAnalysisEngineDlg::OnBnClickedButton8()
{
	UpdateData(TRUE);
	//�������������·��API,������json�ļ��еı߼�Ϊs��e�����·��
	if (!app.printShortestPath("nginx-1.4.4/nginx-1.4.4/html/ShortestPath.json", start, end))
		MessageBox(_T("NO PATH!"));
	else
		MessageBox(_T("Shortest path has been found!"));
}


void CGraphAnalysisEngineDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/ShortestPath.html"), NULL, NULL, SW_SHOWNORMAL);
}
