
// MFCConnectServiceDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFCConnectService.h"
#include "MFCConnectServiceDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#undef UNICODE 
#pragma warning(disable : 4996)

#define LOGFILE "C:\\Users\\Vanh\\Desktop\\TestService\\memstatus.log"
#define NAMEPIPES "\\\\.\\pipe\\testpipesVA"
#define BUFSIZE 512
// CMFCConnectServiceDlg dialog
BOOL bCreatedPipes = FALSE;
int ChangeFile(char* PathFile);

CMFCConnectServiceDlg::CMFCConnectServiceDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCONNECTSERVICE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCConnectServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCConnectServiceDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNADD, &CMFCConnectServiceDlg::OnBnClickedBtnadd)
	ON_BN_CLICKED(IDC_BTNDELETE, &CMFCConnectServiceDlg::OnBnClickedBtndelete)
END_MESSAGE_MAP()


// CMFCConnectServiceDlg message handlers

BOOL CMFCConnectServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCConnectServiceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCConnectServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCConnectServiceDlg::OnBnClickedBtnadd()
{
    //Create File o day
    char buff[512+7] = { 0 };
    char Temp[512] = { 0 };
    lstrcatA(buff, "AddFil:");
    GetWindowTextA((GetDlgItem(IDC_MFCEDITBROWSE1)->m_hWnd),Temp,512);
    lstrcatA(buff, Temp);
    //const char* PathAddFile = "AddFil:C:\\Users\\Vanh\\Desktop\\TestService\\taofile.txt";
    ChangeFile(buff);
}


void CMFCConnectServiceDlg::OnBnClickedBtndelete()
{
    // Delete File o day
    char buff[512 + 7] = { 0 };
    char Temp[512] = { 0 };
    lstrcatA(buff, "Delete:");
    GetWindowTextA((GetDlgItem(IDC_MFCEDITBROWSE1)->m_hWnd), Temp, 512);
    lstrcatA(buff, Temp);
    ChangeFile((char*)buff);
}

int ChangeFile(char* PathFile)
{
    HANDLE hPipe;
    BOOL   fSuccess = FALSE;
    char Err[512] = { 0 };
    DWORD  cbRead, cbToWrite, cbWritten, dwMode;
    // Khởi tạo kết nối tới Pipes
    while (1)
    {

        hPipe = CreateFileA(
            NAMEPIPES,   // pipe name 
            GENERIC_READ |  // read and write access 
            GENERIC_WRITE,
            0,              // no sharing 
            NULL,           // default security attributes
            OPEN_EXISTING,  // opens existing pipe 
            0,              // default attributes 
            NULL);          // no template file 
        // Nếu tạo đc thì break
        if (hPipe != INVALID_HANDLE_VALUE)
        {
            break;
        } 
        // Exit if an error other than ERROR_PIPE_BUSY occurs. 
        // Xuwr lys looi
        if (GetLastError() != ERROR_PIPE_BUSY)
        {
            sprintf(Err,"Could not open pipe. GLE=%d\n", GetLastError());
            MessageBoxA(0, Err, "Tittle", MB_OK);
            return -1;
        }
        // Đợi Pipes khi nó bận chừng 20s
        if (!WaitNamedPipeA(NAMEPIPES, 20000))
        {
            //printf("Could not open pipe: 20 second wait timed out.");
            MessageBoxA(0, "Could not open pipe: 20 second wait timed out.", "Tittle", MB_OK);
            return -1;
        }
    }
    // Pipe đã được kết nối, chuyển chế độ READ tin nhắn
    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(
        hPipe,    // pipe handle 
        &dwMode,  // new pipe mode 
        NULL,     // don't set maximum bytes 
        NULL);    // don't set maximum time 
    if (!fSuccess)
    {
        sprintf(Err, "SetNamedPipeHandleState failed. GLE=%d\n", GetLastError());
        MessageBoxA(0, Err, "Tittle", MB_OK);
        return -1;
    }
    // Send a message to the pipe server. 

    cbToWrite = (lstrlenA(PathFile) + 1) * sizeof(TCHAR);
    while (1)
    {
        //printf("Sending %d byte message: \"%s\"\n", cbToWrite, PathFile);
        fSuccess = WriteFile(
            hPipe,                  // pipe handle 
            PathFile,             // message 
            cbToWrite,              // message length 
            &cbWritten,             // bytes written 
            NULL);                  // not overlapped 

        if (!fSuccess)

        {
            sprintf(Err,"WriteFile to pipe failed. GLE=%d\n", GetLastError());
            MessageBoxA(0, Err, "Tittle", MB_OK);
            return -1;
        }
        break;
    }
    //printf("\nMessage sent to server, receiving reply as follows:\n");
    char chBuf[BUFSIZE] = { 0 };
    do
    {
        // Read from the pipe. 

        fSuccess = ReadFile(
            hPipe,    // pipe handle 
            chBuf,    // buffer to receive reply 
            BUFSIZE * sizeof(char),  // size of buffer 
            &cbRead,  // number of bytes read 
            NULL);    // not overlapped 

        if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
            break;

        //printf("\"%s\"\n", chBuf);
        MessageBoxA(0, chBuf, "Tittle", MB_OK);

    } while (!fSuccess);  // repeat loop if ERROR_MORE_DATA
    CloseHandle(hPipe);
}