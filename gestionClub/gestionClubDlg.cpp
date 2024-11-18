
// gestionClubDlg.cpp : implementation file
//

#include "stdafx.h"
#include "gestionClub.h"
#include "gestionClubDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CgestionClubDlg dialog




CgestionClubDlg::CgestionClubDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CgestionClubDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgestionClubDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CgestionClubDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_VILLAGES, &CgestionClubDlg::OnBnClickedBtnVillages)
	ON_BN_CLICKED(IDC_BTN_ACTIVITES, &CgestionClubDlg::OnBnClickedBtnActivites)
	ON_BN_CLICKED(IDC_BTN_PAYS, &CgestionClubDlg::OnBnClickedBtnPays)
	ON_BN_CLICKED(IDC_BTN_LANGUES, &CgestionClubDlg::OnBnClickedBtnLangues)
	ON_COMMAND(ID_FICHIER_CREER_BD, &CgestionClubDlg::OnFichierCreerBd)
	ON_COMMAND(ID_AIDE_APROPOS, &CgestionClubDlg::OnAideApropos)
	ON_BN_CLICKED(IDC_BTN_CREEBD, &CgestionClubDlg::OnBnClickedBtnCreebd)
	ON_BN_CLICKED(IDC_BTN_APROPOS, &CgestionClubDlg::OnBnClickedBtnApropos)
	ON_COMMAND(ID_FICHIER_QUITTER, &CgestionClubDlg::OnFichierQuitter)
	ON_COMMAND(ID_PAGES_VILLAGES, &CgestionClubDlg::OnPagesVillages)
	ON_COMMAND(ID_PAGES_ACTIVITES, &CgestionClubDlg::OnPagesActivites)
	ON_COMMAND(ID_PAGES_LANGUES, &CgestionClubDlg::OnPagesLangues)
	ON_COMMAND(ID_PAGES_PAYS, &CgestionClubDlg::OnPagesPays)
	ON_BN_CLICKED(IDC_BTN_QUITTER, &CgestionClubDlg::OnBnClickedBtnQuitter)
END_MESSAGE_MAP()


// CgestionClubDlg message handlers

BOOL CgestionClubDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	rc = new CRect();
	GetDlgItem(IDC_CADRE)->GetWindowRect(rc);
	ScreenToClient(rc);

	currentChild = nullptr;

	OnBnClickedBtnVillages();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CgestionClubDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CgestionClubDlg::OnPaint()
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
HCURSOR CgestionClubDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CgestionClubDlg::OnBnClickedBtnVillages(){ShowChild(new Villages(this));}
void CgestionClubDlg::OnBnClickedBtnActivites(){ShowChild(new Activites(this));}
void CgestionClubDlg::OnBnClickedBtnPays(){ShowChild(new Pays(this));}
void CgestionClubDlg::OnBnClickedBtnLangues(){ShowChild(new Langues(this));}

void CgestionClubDlg::ShowChild(CWnd* child){
	delete currentChild;
	currentChild = child;
	child->MoveWindow(rc);
}


void CgestionClubDlg::OnFichierCreerBd()
{
    // TODO: Add your command handler code here
    CDaoDatabase db;
    CString lpszFile = L"base.mdb";
	delete currentChild;
	currentChild = nullptr;

    if (PathFileExists(lpszFile)) {
        int result = AfxMessageBox(L"Le fichier de la base de donées existe déja. Voulez vous le supprimer?", MB_YESNO | MB_ICONQUESTION);
        if (result != IDYES) {
            return;
        }
		if (!DeleteFile(lpszFile)) {
            AfxMessageBox(L"Erreur de suppression du fichier de la base de données");
            return;
        }
    }



    try {
        db.Create(lpszFile);
    } catch (CDaoException* e) {
        AfxMessageBox(L"Erreur de la  création de la base de données.");
        e->Delete();
        return;
    }

    if (!db.IsOpen()) {
        AfxMessageBox(L"Erreur d'ouverture de la base de données.");
        return;
    }

    CString create_tables[] = {
		L"CREATE TABLE Activites(No_Activite AUTOINCREMENT PRIMARY KEY,Designation VARCHAR(30) UNIQUE);",
		L"CREATE TABLE Continent(No_Continent AUTOINCREMENT PRIMARY KEY,Code_Continent CHAR(3) UNIQUE,Nom VARCHAR(30) UNIQUE);",
		L"CREATE TABLE Langues(No_Langue AUTOINCREMENT PRIMARY KEY,Designation VARCHAR(30) UNIQUE);",
		L"CREATE TABLE Pays(No_Pays AUTOINCREMENT PRIMARY KEY,Code_Pays CHAR(3) UNIQUE,Nom VARCHAR(30) UNIQUE,Continent INT,FOREIGN KEY (Continent) REFERENCES Continent(No_Continent));",
		L"CREATE TABLE Village(No_Village AUTOINCREMENT PRIMARY KEY,Code_Village CHAR(3) UNIQUE,Nom VARCHAR(30) UNIQUE,Pays INT,FOREIGN KEY (Pays) REFERENCES Pays(No_Pays));",
		L"CREATE TABLE Proposer(No_Village INT,No_Activite INT,Gratuite YESNO,PRIMARY KEY (No_Village, No_Activite),FOREIGN KEY (No_Village) REFERENCES Village(No_Village),FOREIGN KEY (No_Activite) REFERENCES Activites(No_Activite));",
		L"CREATE TABLE Parler(No_Village INT,No_Langue INT,PRIMARY KEY (No_Village, No_Langue),FOREIGN KEY (No_Village) REFERENCES Village(No_Village),FOREIGN KEY (No_Langue) REFERENCES Langues(No_Langue));"
	};

	CString fill_tables[] = {
		L"INSERT INTO Continent (Code_Continent, Nom) VALUES ('AFR', 'Afrique')",
		L"INSERT INTO Continent (Code_Continent, Nom) VALUES ('AME', 'Amerique')",
		L"INSERT INTO Continent (Code_Continent, Nom) VALUES ('ASI', 'Asie')",
		L"INSERT INTO Continent (Code_Continent, Nom) VALUES ('AUS', 'Australie');",
		L"INSERT INTO Continent (Code_Continent, Nom) VALUES ('EUR', 'Europe');",
		
		// Afrique
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('DZA', 'Algérie', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('EGY', 'Égypte', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('ZAF', 'Afrique du Sud', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('MAR', 'Maroc', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('TUN', 'Tunisie', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('NGA', 'Nigeria', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('KEN', 'Kenya', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('ETH', 'Éthiopie', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('GHA', 'Ghana', 1)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('SEN', 'Sénégal', 1)",

		// Amerique
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('ARG', 'Argentine', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('BRA', 'Brésil', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('CAN', 'Canada', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('CHL', 'Chili', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('COL', 'Colombie', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('CUB', 'Cuba', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('MEX', 'Mexique', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('PER', 'Pérou', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('USA', 'États-Unis', 2)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('VEN', 'Venezuela', 2)",

		// Asia 
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('CHN', 'Chine', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('IND', 'Inde', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('JPN', 'Japon', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('KOR', 'Corée du Sud', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('VNM', 'Vietnam', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('IDN', 'Indonésie', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('TUR', 'Turquie', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('SAU', 'Arabie Saoudite', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('IRN', 'Iran', 3)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('THA', 'Thaïlande', 3)",

		// Australie
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('AUS', 'Australie',4)",

		// Europe
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('AUT', 'Autriche', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('BEL', 'Belgique', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('FRA', 'France', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('DEU', 'Allemagne', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('ITA', 'Italie', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('NLD', 'Pays-Bas', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('POL', 'Pologne', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('ESP', 'Espagne', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('SWE', 'Suède', 5)",
		L"INSERT INTO Pays (Code_Pays, Nom, Continent) VALUES ('GBR', 'Royaume-Uni', 5)",

		// Langues
		L"INSERT INTO langues (designation) VALUES ('Allemand')",
		L"INSERT INTO langues (designation) VALUES ('Anglais')",
		L"INSERT INTO langues (designation) VALUES ('Arabe')",
		L"INSERT INTO langues (designation) VALUES ('Chinois')",
		L"INSERT INTO langues (designation) VALUES ('Coréen')",
		L"INSERT INTO langues (designation) VALUES ('Danois')",
		L"INSERT INTO langues (designation) VALUES ('Espagnol')",
		L"INSERT INTO langues (designation) VALUES ('Finnois')",
		L"INSERT INTO langues (designation) VALUES ('Français')",
		L"INSERT INTO langues (designation) VALUES ('Grec')",
		L"INSERT INTO langues (designation) VALUES ('Hindi')",
		L"INSERT INTO langues (designation) VALUES ('Italien')",
		L"INSERT INTO langues (designation) VALUES ('Japonais')",
		L"INSERT INTO langues (designation) VALUES ('Néerlandais')",
		L"INSERT INTO langues (designation) VALUES ('Norvégien')",
		L"INSERT INTO langues (designation) VALUES ('Polonais')",
		L"INSERT INTO langues (designation) VALUES ('Portugais')",
		L"INSERT INTO langues (designation) VALUES ('Russe')",
		L"INSERT INTO langues (designation) VALUES ('Suédois')",
		L"INSERT INTO langues (designation) VALUES ('Tchèque')",

		// Activites
		L"INSERT INTO Activites (Designation) VALUES ('Natation')",
		L"INSERT INTO Activites (Designation) VALUES ('Randonnée')",
		L"INSERT INTO Activites (Designation) VALUES ('Cyclisme')",
		L"INSERT INTO Activites (Designation) VALUES ('Pêche')",
		L"INSERT INTO Activites (Designation) VALUES ('Visites touristiques')",
		L"INSERT INTO Activites (Designation) VALUES ('Pique-nique')",
		L"INSERT INTO Activites (Designation) VALUES ('Navigation')",
		L"INSERT INTO Activites (Designation) VALUES ('Équitation')",
		L"INSERT INTO Activites (Designation) VALUES ('Observation des oiseaux')",
		L"INSERT INTO Activites (Designation) VALUES ('Photographie')"

	};

    
	for(int i=0;i<7;i++)
		db.Execute(create_tables[i]);
	for(int i=0;i<76;i++)
		db.Execute(fill_tables[i]);

    db.Close();

    AfxMessageBox(L"Base de données créée avec succès.");


}



void CgestionClubDlg::OnAideApropos()
{
	// TODO: Add your command handler code here
	CAboutDlg().DoModal();
}


void CgestionClubDlg::OnBnClickedBtnCreebd()
{
	// TODO: Add your control notification handler code here
	OnFichierCreerBd();
}



void CgestionClubDlg::OnBnClickedBtnApropos()
{
	// TODO: Add your control notification handler code here
	OnAideApropos();
}


void CgestionClubDlg::OnFichierQuitter()
{
	// TODO: Add your command handler code here
	OnOK();
}


void CgestionClubDlg::OnPagesVillages()
{
	// TODO: Add your command handler code here
	OnBnClickedBtnVillages();
}


void CgestionClubDlg::OnPagesActivites()
{
	// TODO: Add your command handler code here
	OnBnClickedBtnActivites();
}


void CgestionClubDlg::OnPagesLangues()
{
	// TODO: Add your command handler code here
	OnBnClickedBtnLangues();
}


void CgestionClubDlg::OnPagesPays()
{
	// TODO: Add your command handler code here
	OnBnClickedBtnPays();
}


void CgestionClubDlg::OnBnClickedBtnQuitter()
{
	// TODO: Add your control notification handler code here
	OnFichierQuitter();
}
