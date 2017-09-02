/////////////////////////////////////////////////////////////////////////////
// suport.h : Services for applications
//

////////////////////////////////////////////////////////////////
// General defines

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef ABS
#define ABS(a) (((a)<0) ? -(a) : (a))
#endif

#ifndef TRUE
#define TRUE (1 == 1)
#endif

#ifndef FALSE
#define FALSE (1 != 1)
#endif

#define		CASTINT(val) ((int)(val))
#define		MATHPI  3.1415926535

#ifndef MBOX
#define MBOX		xMsgBox
#define MBOXP		AfxPrintf
#endif


#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif // ndef WS_EX_LAYERED

// Preparation for the function we want to import from USER32.DLL
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)(HWND hWnd, 
                                  COLORREF crKey, BYTE bAlpha, DWORD dwFlags);

extern	lpfnSetLayeredWindowAttributes m_pSetLayeredWindowAttributes;

			
/////////////////////////////////////////////////////////////////////////////
// Misc app services 

CMenu 	*GetTemplateMenu();
void	OpenSource(CString &dir);
void 	SaveAllDocs();
void 	message(const char * str);
void 	rowcol(const char * str);
void 	filesize(const char * str);
void 	hold(const char * str);
void 	mac(const char * str);
void 	mode(const char * str);
void	PathToFname(CString &docname);
void	PathToDir(CString &docname);
void	ShortenPath(CString &str, int len); 
int		HashString(const char *name);
void    YieldToWin();
int     YieldToWinEx();
int	 	FindNumOfChar(CString &str, CString &mus, int start);

CString GetCurrentDir();

void	SplitPath(CString &full,  CString &drive,
					CString &dir, CString &fname, CString &ext);

BOOL	GetSpecialFolder(UINT SpecialFolder, CString &SpecialFolderString);
int		GetDaysUsed();
int		LicMonth(const char *str);

//int		SendMail(const char * str[], int num);

int		SendMail(const char *to, const char * str[], int num);

int     AfxPrintf(const char *Format, ...);
int		IsDir(const char *str);
int		IsFile(const char *str);

void	RepairPath(CString &dir);

CString		rmrf(const char * str);

int		CheckLic(const char *str, int *res);
int		SumStr(const char *str);

int		GetRegRootInt(const char *str, const char *key);
void	SetRegRootInt(const char *str, const char *key, int val);

int		GetLocMachineInt(const char *str, const char *key);
void	SetLocMachineInt(const char *str, const char *key, int val);

CString EscapeComma(CString &str);
CString EscapeSql(CString &str);

void	Help();

#define     ROTATE_LONG_LEFT(x, n)  (((x) << (n))  | ((x) >> (32 - (n))))
#define     ROTATE_LONG_RIGHT(x, n) (((x) >> (n))  | ((x) << (32 - (n))))

void	uuid2str(UUID *id, CString &str);
void	str2uuid(UUID *id, CString &str);

void		Repair_ImgPath(CString &str);
void		Normalize_ImgPath(CString &img);

CString		Create_db_Name(void *view);

// EOF
