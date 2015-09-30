
// GraphAnalysisEngineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GraphAnalysisEngine.h"
#include "GraphAnalysisEngineDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CGraphAnalysisEngineDlg 对话框




CGraphAnalysisEngineDlg::CGraphAnalysisEngineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGraphAnalysisEngineDlg::IDD, pParent)
	, outfilePath_MT(_T("nginx-1.4.4/nginx-1.4.4/html/最小生成森林.json"))
	, outfilePath_C(_T("中心度.txt"))
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


// CGraphAnalysisEngineDlg 消息处理程序

BOOL CGraphAnalysisEngineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGraphAnalysisEngineDlg::OnPaint()
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
HCURSOR CGraphAnalysisEngineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//读取输入文件路径的控件
void CGraphAnalysisEngineDlg::OnEnChangeMfceditbrowse2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	GetDlgItemText(IDC_MFCEDITBROWSE2, infilePath);
	int size = WideCharToMultiByte(CP_ACP, 0, infilePath.GetBuffer(), 
		-1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, infilePath.GetBuffer(), -1, Inverted_infilePath, 
		size, NULL, NULL);
	MessageBox(_T("图分析进行中，程序不会崩溃的>.<，请稍等..."));
	app.setInFile(Inverted_infilePath);
	//n为起始点，若n < 0，则生成最小生成森林
	app.printMinSpanningTree("nginx-1.4.4/nginx-1.4.4/html/MinSpanningTree.json");
	//结点接近中心度和介数中心度导出到TXT的API
	app.printCentralityToTXT("中心度.txt");
	//结点接近中心度和介数中心度导出到JSON的API
	app.printBCentralityToJSON("nginx-1.4.4/nginx-1.4.4/html/BetweennessCentrality.json");
	app.printBCentralityToJSON("nginx-1.4.4/nginx-1.4.4/html/BetweennessCentrality.json");
	node = app.node;
	edge = app.edge;
	UpdateData(false);
	MessageBox(_T("程序果然没有崩溃^_^，图分析完成！"));
}

//最小生成树导出路径的输入控件
void CGraphAnalysisEngineDlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//中心度导出路径的输入控件
void CGraphAnalysisEngineDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//最小生成树结果的导出
void CGraphAnalysisEngineDlg::OnBnClickedButton2()
{
	UpdateData(TRUE);
	int size = WideCharToMultiByte(CP_ACP, 0, outfilePath_MT.GetBuffer(), 
		-1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, outfilePath_MT.GetBuffer(), -1, Inverted_outfilePath_MT, 
		size, NULL, NULL);
	app.setOutFile(Inverted_outfilePath_MT);
 	//导出最小生成树的json文件
 	app.exportMinSpanningTree();
}

//调用127.0.0.1展示结果
void CGraphAnalysisEngineDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/Minimum_Spanning_Tree.html"), NULL, NULL, SW_SHOWNORMAL);
}

//中心度txt文件的导出
void CGraphAnalysisEngineDlg::OnBnClickedButton5()
{
	UpdateData(TRUE);
	int size = WideCharToMultiByte(CP_ACP, 0, outfilePath_C.GetBuffer(), 
		-1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, outfilePath_C.GetBuffer(), -1, Inverted_outfilePath_C, 
		size, NULL, NULL);
	app.setOutFile(Inverted_outfilePath_C);
	//导出结点的中心度数据
	app.exportCentralityToTXT();
}


//接近中心度：生成json文件，并完成打开操作
void CGraphAnalysisEngineDlg::OnBnClickedButton6()
{
	app.setOutFile("nginx-1.4.4/nginx-1.4.4/html/ClosenessCentrality.json");
	app.exportBCentralityToJSON();
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/ClosenessCentrality.html"), NULL, NULL, SW_SHOWNORMAL);
}

//介数中心度：生成json文件，并完成打开操作
void CGraphAnalysisEngineDlg::OnBnClickedButton7()
{
	app.setOutFile("nginx-1.4.4/nginx-1.4.4/html/BetweennessCentrality.json");
	app.exportCCentralityToJSON();
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/BetweennessCentrality.html"), NULL, NULL, SW_SHOWNORMAL);
}

//使用说明
void CGraphAnalysisEngineDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"), _T("说明.txt"), NULL, NULL, SW_SHOWNORMAL);
}

/*
//导入文件
void CGraphAnalysisEngineDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
}*/

//完成最小路径的生成和数据的输出
void CGraphAnalysisEngineDlg::OnBnClickedButton8()
{
	UpdateData(TRUE);
	//导出两点间的最短路径API,导出的json文件中的边即为s到e的最短路径
	if (!app.printShortestPath("nginx-1.4.4/nginx-1.4.4/html/ShortestPath.json", start, end))
		MessageBox(_T("NO PATH!"));
	else
		MessageBox(_T("Shortest path has been found!"));
}


void CGraphAnalysisEngineDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"), _T("http://127.0.0.1/ShortestPath.html"), NULL, NULL, SW_SHOWNORMAL);
}
