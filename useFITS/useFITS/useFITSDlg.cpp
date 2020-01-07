// useFITSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "useFITS.h"
#include "useFITSDlg.h"
#include "fitsio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CuseFITSDlg �Ի���




CuseFITSDlg::CuseFITSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CuseFITSDlg::IDD, pParent)
	, m_text_fits_fname(_T(""))
	, m_text_out_msg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuseFITSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_text_fits_fname);
	DDX_Text(pDX, IDC_EDIT_OUT_MSG, m_text_out_msg);
}

BEGIN_MESSAGE_MAP(CuseFITSDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CuseFITSDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CuseFITSDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CuseFITSDlg ��Ϣ�������

BOOL CuseFITSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

	//	 ��ʾ�ڴ��������fits�ļ����ļ���
	m_text_fits_fname = TEXT("����fits�ļ���");
	UpdateData(FALSE);	//	������ͬ�����ؼ�

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CuseFITSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CuseFITSDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CuseFITSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CuseFITSDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	//	���ؼ�����ͬ��������
	UpdateData(TRUE);

	CString str = _T("��������ļ�����")+m_text_fits_fname;
	MessageBox(str);
}

void CuseFITSDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//	�ٴ�ȷ��fits�ļ���
	CString str = _T("Your fits filename is: ") + m_text_fits_fname;
	m_text_out_msg = str;
	UpdateData(FALSE);

	//int n = 0;
	//while( n < 5 ){
	//	CString tmp;
	//	tmp.Format(_T("\r\n> %d"),n);
	//	str += tmp;
	//	m_text_out_msg = str;
	//UpdateData(FALSE);
	//n += 1;
	//}

	fitsfile *infptr, *outfptr;   /* FITS file pointers defined in fitsio.h */
    int status = 0;       /* status must always be initialized = 0  */

	char fits_in[1024];
	char fits_out[1024];
	sprintf_s(fits_in,"f_1.fits");
	sprintf_s(fits_out,"f_1_bak.fits");

	CString msg(fits_in);
	MessageBox(_T("Ready to open: ") + msg);


	if ( !fits_open_file(&infptr, fits_in, READONLY, &status) )
    {

		CString msg1(fits_in);
		MessageBox(_T("Successfully opened: ") + msg1);

	/*	�������ļ��Ƿ����,�������,���Ƴ�*/
		remove(fits_out);

      /* Create the output file */
      if ( !fits_create_file(&outfptr, fits_out, &status) )
      {
		CString msg2(fits_out);
		MessageBox(_T("Copying ") + msg + _T(" into ")+ msg2);
 
        /* copy the previous, current, and following HDUs */
        fits_copy_file(infptr, outfptr, 1, 1, 1, &status);

        fits_close_file(outfptr,  &status);
      }
	  else{
		CString msg2(fits_out);
		MessageBox(_T("Failed to create: ") + msg2);
	  }

      fits_close_file(infptr, &status);

	} else {
		MessageBox(_T("Failed to open: ") + msg);
	}

    /* if error occured, print out error message */
    //if (status) fits_report_error(stderr, status);
}
