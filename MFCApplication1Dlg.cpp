
// MFCApplication1Dlg.cpp : 実装ファイル
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <highlevelmonitorconfigurationapi.h>
#include <lowlevelmonitorconfigurationapi.h>
#include <physicalmonitorenumerationapi.h>
#include <vector>
#include <string>

// dxva2.libをリンク
#pragma comment(lib, "dxva2.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg ダイアログ



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_SLIDER1, &CMFCApplication1Dlg::OnTRBNThumbPosChangingSlider1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTONAPPLAY, &CMFCApplication1Dlg::OnBnClickedButtonapplay)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CMFCApplication1Dlg::OnDeltaposSpin1)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMFCApplication1Dlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// CMFCApplication1Dlg メッセージ ハンドラー

///モニターハンドルベクター
std::vector<HANDLE> hMonitors;
///物理モニターハンドルベクター
std::vector<HANDLE> hPhysicalMonitors;


BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	GethMonitorsAndhPhysicalMonitors();
	CString str;
	BOOL none = true;

	for (int i = 0; i < hPhysicalMonitors.size(); i++)
	{
		DWORD dwMinimumBrightness = {}; DWORD dwCurrentBrightness = {  }; DWORD dwMaximumBrightness = {  };
		BOOL s = GetDDCCIMonitorBrightness(i, &dwMinimumBrightness, &dwCurrentBrightness, &dwMaximumBrightness);
		str.Format(_T("モニター%d"), i);

		if (s==FALSE) {
			break;
		}
		else {
			none = false;
		}

		((CListCtrl*)GetDlgItem(IDC_LIST3))->InsertItem(0, str);

		((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetRange(dwMinimumBrightness, dwMaximumBrightness);
		((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(dwCurrentBrightness);
		str.Format(_T("%d"), dwCurrentBrightness);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowTextW(str);

	}


	//DISPLAY_DEVICE dd;
	//dd.cb = sizeof(dd);
	//int deviceIndex = 0;
	//while (EnumDisplayDevices(0, deviceIndex, &dd, 0))
	//{
	//	std::wstring deviceName = dd.DeviceName;
	//	int monitorIndex = 0;
	//	while (EnumDisplayDevices(deviceName.c_str(), monitorIndex, &dd, 0))
	//	{
	//		str.Format(_T("%s %s"), dd.DeviceName, dd.DeviceString);
	//		//AfxMessageBox(str);
	//		++monitorIndex;
	//	}
	//	++deviceIndex;
	//}


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




///モニターハンドルおよび物理モニターハンドルを実際に取得するCALLBACK関数
BOOL CALLBACK MyMonitorEnumProcDDCCI(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	LPVOID lpMsgBuf;

	///hMonitorsへプッシュ
	hMonitors.push_back(hMonitor);

	///hPhysicalMonitorsへプッシュ
	DWORD pdwNumberOfPhysicalMonitors = { 0 };
	LPPHYSICAL_MONITOR pPhysicalMonitorArray = { 0 };
	//物理モニター数を取得する（下準備）
	GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &pdwNumberOfPhysicalMonitors);
	//pPhysicalMonitorArrayから、物理モニターハンドルを取得
	pPhysicalMonitorArray = (LPPHYSICAL_MONITOR)malloc(pdwNumberOfPhysicalMonitors * sizeof(PHYSICAL_MONITOR));
	BOOL s = GetPhysicalMonitorsFromHMONITOR(hMonitor, pdwNumberOfPhysicalMonitors, pPhysicalMonitorArray);
	HANDLE hPhysicalMonitor = pPhysicalMonitorArray->hPhysicalMonitor;
	//SetMonitorBrightness(hPhysicalMonitor,20);


	printf("SUCCESS:%d\n", s);
	printf("szPhysicalMonitorDescription:%ls\n", pPhysicalMonitorArray->szPhysicalMonitorDescription);
	//追加
	hPhysicalMonitors.push_back(hPhysicalMonitor);
	printf("%d\n", hPhysicalMonitors.size());
	printf("%d\n", hMonitors.size());

	return TRUE;

}
///モニターハンドルおよび物理モニターハンドルを取得
void GethMonitorsAndhPhysicalMonitors() {
	EnumDisplayMonitors(NULL, NULL, MyMonitorEnumProcDDCCI, 0);
}

BOOL GetDDCCIMonitorBrightness(int Index, LPDWORD pdwMinimumBrightness, LPDWORD pdwCurrentBrightness, LPDWORD pdwMaximumBrightness) {

	HANDLE hPhysicalMonitor = hPhysicalMonitors[Index];

	BOOL s = GetMonitorBrightness(hPhysicalMonitor, pdwMinimumBrightness, pdwCurrentBrightness, pdwMaximumBrightness);
	printf("%d\n", s);
	//printf("%d\n", pdwMinimumBrightness);
	//printf("%d\n", pdwCurrentBrightness);
	//printf("%d\n", pdwMaximumBrightness);

	DestroyPhysicalMonitor(hPhysicalMonitor);


	return s;
}


void CMFCApplication1Dlg::OnTRBNThumbPosChangingSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// この機能には Windows Vista 以降のバージョンが必要です。
	// シンボル _WIN32_WINNT は >= 0x0600 にする必要があります。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}


void CMFCApplication1Dlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

	int val = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos();
	CString str;
	str.Format(_T("%d"), val);
	((CStatic*)GetDlgItem(IDC_EDIT1))->SetWindowTextW(str);
	//AfxMessageBox(_T(""));

	//プライマリモニターのハンドルを取得
	HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	///物理モニターのハンドルを取得する
	DWORD pdwNumberOfPhysicalMonitors = { 0 };
	//物理モニター数を取得する（下準備）
	GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &pdwNumberOfPhysicalMonitors);
	LPPHYSICAL_MONITOR pPhysicalMonitorArray = { 0 };
	//モニターハンドルから物理モニターを取得する
	//pPhysicalMonitorArrayから、物理モニターハンドルを取得可能
	pPhysicalMonitorArray = (LPPHYSICAL_MONITOR)malloc(pdwNumberOfPhysicalMonitors * sizeof(PHYSICAL_MONITOR));
	BOOL s = GetPhysicalMonitorsFromHMONITOR(hMonitor, pdwNumberOfPhysicalMonitors, pPhysicalMonitorArray);
	//物理モニターハンドルを取得
	HANDLE hPhysicalMonitor = pPhysicalMonitorArray->hPhysicalMonitor;
	///明るさを取得
	DWORD pdwMinimumBrightness = { 0 };
	DWORD pdwCurrentBrightness = { 0 };
	DWORD pdwMaximumBrightness = { 0 };
	s = GetMonitorBrightness(
		hPhysicalMonitor, &
		pdwMinimumBrightness, &
		pdwCurrentBrightness, &
		pdwMaximumBrightness
	);
	if (!s) {
		AfxMessageBox(_T("DDC/CIに対応していません"));
		return;
	}

	s = SetMonitorBrightness(hPhysicalMonitor, ((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->GetPos());

	DestroyPhysicalMonitor(pPhysicalMonitorArray);
	DestroyPhysicalMonitor(hMonitor);

}


void CMFCApplication1Dlg::OnBnClickedButtonapplay()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	BOOL s = ApplyDisplaySetting();

	if (s == TRUE) {
		AfxMessageBox(_T("ディスプレイに設定を適用しました。"));
	}
	else {
		AfxMessageBox(_T("設定適用に失敗しました。"));
	}
}

BOOL ApplyDisplaySetting() {
	//プライマリモニターのハンドルを取得
	HMONITOR hMonitor = MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY);
	///物理モニターのハンドルを取得する
	DWORD pdwNumberOfPhysicalMonitors = { 0 };
	//物理モニター数を取得する（下準備）
	GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &pdwNumberOfPhysicalMonitors);
	LPPHYSICAL_MONITOR pPhysicalMonitorArray = { 0 };
	//モニターハンドルから物理モニターを取得する
	//pPhysicalMonitorArrayから、物理モニターハンドルを取得可能
	pPhysicalMonitorArray = (LPPHYSICAL_MONITOR)malloc(pdwNumberOfPhysicalMonitors * sizeof(PHYSICAL_MONITOR));
	BOOL s = GetPhysicalMonitorsFromHMONITOR(hMonitor, pdwNumberOfPhysicalMonitors, pPhysicalMonitorArray);
	//物理モニターハンドルを取得
	HANDLE hPhysicalMonitor = pPhysicalMonitorArray->hPhysicalMonitor;

	s = SaveCurrentMonitorSettings(hPhysicalMonitor);

	DestroyPhysicalMonitor(hPhysicalMonitor);

	return s;
}

void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	BOOL s = ApplyDisplaySetting();

	if (s == TRUE) {
		//AfxMessageBox(_T("ディスプレイに設定を適用しました。"));
	}
	else {
		AfxMessageBox(_T("設定適用に失敗しました。"));
	}

	CDialogEx::OnOK();
}


void CMFCApplication1Dlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}


void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO: これが RICHEDIT コントロールの場合、このコントロールが
	// この通知を送信するには、CDialogEx::OnInitDialog() 関数をオーバーライドし、
	// CRichEditCtrl().SetEventMask() を関数し呼び出します。
	// OR 状態の ENM_CHANGE フラグをマスクに入れて呼び出す必要があります。

	// TODO: ここにコントロール通知ハンドラー コードを追加してください。
}


void CMFCApplication1Dlg::OnBnClickedCancel()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnCancel();
}


void CMFCApplication1Dlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	*pResult = 0;
}
