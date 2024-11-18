// Pays.cpp : implementation file
//

#include "stdafx.h"
#include "gestionClub.h"
#include "Pays.h"
#include "afxdialogex.h"


// Pays dialog

IMPLEMENT_DYNAMIC(Pays, CDialogEx)

Pays::Pays(CWnd* pParent /*=NULL*/)
	: CDialogEx(Pays::IDD, pParent),db(CDaoDatabase()),recset(&db)
	, m_CodePaysEdit(_T(""))
	, m_NomPaysEdit(_T(""))
{
	defaultQuery = L"SELECT No_Pays, Code_Pays, Pays.Nom, Pays.Continent, Continent.Nom FROM Pays inner join Continent on Pays.Continent = Continent.No_Continent";
	db.Open(L"base.mdb");
	recset.Open(AFX_DAO_USE_DEFAULT_TYPE, defaultQuery, NULL);

	Create(Pays::IDD, pParent);
	ShowWindow(SW_SHOW);
}

Pays::~Pays()
{
	if(recset.IsOpen()){
		recset.Close();
	}

	if(db.IsOpen()){
		db.Close();
	}
}

void Pays::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAYS, m_ListPays);
	DDX_Text(pDX, IDC_CODE_PAYS_EDIT, m_CodePaysEdit);
	DDV_MaxChars(pDX, m_CodePaysEdit, 3);
	DDX_Text(pDX, IDC_NOM_PAYS_EDIT, m_NomPaysEdit);
	DDV_MaxChars(pDX, m_NomPaysEdit, 30);
	DDX_Control(pDX, IDC_PAYS_CONTINENT_COMBO, m_ContinentCombo);
	DDX_Control(pDX, IDC_CODE_PAYS_EDIT, m_CodePaysControl);
	DDX_Control(pDX, IDC_NOM_PAYS_EDIT, m_NomPaysControl);
}


BEGIN_MESSAGE_MAP(Pays, CDialogEx)
	ON_BN_CLICKED(IDC_PREMIER, &Pays::OnBnClickedPremier)
	ON_BN_CLICKED(IDC_DERNIER, &Pays::OnBnClickedDernier)
	ON_BN_CLICKED(IDC_NEXT, &Pays::OnBnClickedNext)
	ON_BN_CLICKED(IDC_PREVIOUS, &Pays::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_TROUVER, &Pays::OnBnClickedTrouver)
	ON_BN_CLICKED(IDC_ACTUALISER, &Pays::OnBnClickedActualiser)
	ON_BN_CLICKED(IDC_SUPPRIMER, &Pays::OnBnClickedSupprimer)
	ON_BN_CLICKED(IDC_MODIFIER, &Pays::OnBnClickedModifier)
	ON_BN_CLICKED(IDC_AJOUTER, &Pays::OnBnClickedAjouter)
END_MESSAGE_MAP()


// Pays message handlers
BOOL Pays::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ListPays.InsertColumn(0,L"No_Pays",LVCFMT_CENTER,80,-1);
	m_ListPays.InsertColumn(1,L"Code_Pays",LVCFMT_CENTER,120,-1);
	m_ListPays.InsertColumn(2,L"Nom",LVCFMT_CENTER,179-1);
	m_ListPays.InsertColumn(3,L"Continent",LVCFMT_CENTER,180,-1);
	ListView_SetExtendedListViewStyle(m_ListPays, LVS_EX_GRIDLINES);
	m_ListPays.DeleteAllItems();
	m_ListPays.SetBkColor(RGB(255, 138, 250)); // pour colorer la list

	m_ContinentCombo.SetCueBanner(L"Continent");
	m_CodePaysControl.SetCueBanner(L"Code");
	m_NomPaysControl.SetCueBanner(L"Nom");

	CDaoRecordset ContinentSet(&db);
	ContinentSet.Open(AFX_DAO_USE_DEFAULT_TYPE,L"select * from Continent",NULL);
	COleVariant var;
	var.ChangeType(VT_BSTR, NULL);
	CString Nom_Continent;

	int i = 0;
	while(!ContinentSet.IsEOF()){
		ContinentSet.GetFieldValue(L"Nom",var);
		Nom_Continent = (LPCWSTR)var.pbstrVal;
		m_ContinentCombo.InsertString(i,Nom_Continent);
		i++;
		ContinentSet.MoveNext();
	}
	ContinentSet.Close();

	RefreshList();
	return TRUE;
}

void Pays::ListData(CString query)
{
  COleVariant var;           // start Variables
  var.ChangeType(VT_BSTR, NULL);
  CString Code_Pays, Nom_Pays, Continent, No_Pays; // end variables
  m_ListPays.DeleteAllItems();                                        // clear list
  if(query == L""){ query = defaultQuery; }
  query = query + L" Order by Pays.Nom";
  recset.Close();
  recset.Open(AFX_DAO_USE_DEFAULT_TYPE, query, NULL);
  
  int i = 0;
  while (!recset.IsEOF()) // do until eof
  {
    recset.GetFieldValue(L"No_Pays", var);
    No_Pays.Format(L"%d", (LPCWSTR)var.pbstrVal);
    m_ListPays.InsertItem(i, No_Pays, 0);
    recset.GetFieldValue(L"Code_Pays", var);
    Code_Pays = (LPCWSTR)var.pbstrVal;
    m_ListPays.SetItemText(i, 1, Code_Pays);
    recset.GetFieldValue(L"Pays.Nom", var);
    Nom_Pays = (LPCWSTR)var.pbstrVal;
    m_ListPays.SetItemText(i, 2, Nom_Pays);
    recset.GetFieldValue(L"Continent.Nom", var);
    Continent = (LPCWSTR)var.pbstrVal;
    //Continent.Format(L"%d", (LPCWSTR)var.pbstrVal);
    m_ListPays.SetItemText(i, 3, Continent);
    i++;
    recset.MoveNext();
  }
}

void Pays::RefreshList(){
	ListData(L"");
}

void Pays::OnBnClickedPremier()
{
	recset.MoveFirst();
	UpdateFields();
}


void Pays::OnBnClickedDernier()
{
	recset.MoveLast();
	UpdateFields();
}


void Pays::OnBnClickedNext()
{
	if(!recset.IsEOF()){
		try{
			recset.MoveNext();
			UpdateFields();
		}catch(...){
			OnBnClickedPrevious();
		}
	}else{
		OnBnClickedPremier();
	}
}


void Pays::OnBnClickedPrevious()
{
	if(!recset.IsBOF()){
		try{
			recset.MovePrev();
			UpdateFields();
		}catch(...){
			OnBnClickedNext();
		}
	}
}

void Pays::UpdateFields(){
	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	recset.GetFieldValue(L"Code_Pays", var);
	m_CodePaysEdit = (LPCWSTR)var.pbstrVal;
	recset.GetFieldValue(L"Pays.Nom", var);
	m_NomPaysEdit = (LPCWSTR)var.pbstrVal;
	recset.GetFieldValue(L"Continent.Nom", var);
	m_ContinentCombo.SelectString(0,(LPCWSTR)var.pbstrVal);
	UpdateData(false);
}



void Pays::OnBnClickedTrouver()
{
	// TODO: Add your control notification handler code here
	CString Code_Pays;
	GetDlgItemText(IDC_CODE_PAYS_EDIT, Code_Pays);
	if(Code_Pays != L""){
		ListData(defaultQuery +  L" WHERE Code_Pays='" + Code_Pays + L"'");
	}
	
}


void Pays::OnBnClickedActualiser()
{
	// TODO: Add your control notification handler code here
	RefreshList();
}


void Pays::OnBnClickedSupprimer()
{
	// TODO: Add your control notification handler code here
	if(!recset.IsBOF() && !recset.IsEOF()){

		//////////
		COleVariant var;
		recset.GetFieldValue(L"No_Pays", var);
		CString No_Pays;
		No_Pays.Format(L"%d", (LPCWSTR)var.pbstrVal);
		db.Execute(L"DELETE FROM Village WHERE Pays = "+No_Pays);
		//////////

		recset.Delete();
		RefreshList();
		AfxMessageBox(L"supprimé avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Pays::OnBnClickedModifier()
{
	// TODO: Add your control notification handler code here
	if(!recset.IsBOF() && !recset.IsEOF()){
		GetDlgItemText(IDC_CODE_PAYS_EDIT, m_CodePaysEdit);
		GetDlgItemText(IDC_NOM_PAYS_EDIT, m_NomPaysEdit);
		recset.Edit();
		recset.SetFieldValue(L"Code_Pays", (LPCWSTR)m_CodePaysEdit);
		recset.SetFieldValue(L"Pays.Nom", (LPCWSTR)m_NomPaysEdit);
		recset.SetFieldValue(L"Continent", GetComboContinentID());

		recset.Update();
		RefreshList();
	}
}

COleVariant Pays::GetComboContinentID(){
	CString Continent;
		m_ContinentCombo.GetLBText(m_ContinentCombo.GetCurSel(),Continent);
		CDaoRecordset ContinentSet(&db);
		COleVariant No_Continent((LONG)0);
		ContinentSet.Open(AFX_DAO_USE_DEFAULT_TYPE, L"select * from Continent where Nom = '"+Continent+L"'", NULL);
		if (!ContinentSet.IsEOF()) {
			ContinentSet.MoveFirst();
			ContinentSet.GetFieldValue(L"No_Continent", No_Continent);
		}
		ContinentSet.Close();
		return No_Continent;
}

void Pays::OnBnClickedAjouter()
{
	// TODO: Add your control notification handler code here
	CString Code_Pays, Nom_Pays;
	GetDlgItemText(IDC_CODE_PAYS_EDIT, Code_Pays);
	GetDlgItemText(IDC_NOM_PAYS_EDIT, Nom_Pays);
	recset.AddNew();
	recset.SetFieldValue(L"Code_Pays", (LPCWSTR)Code_Pays);
	recset.SetFieldValue(L"Pays.Nom", (LPCWSTR)Nom_Pays);
	recset.SetFieldValue(L"Continent", GetComboContinentID());
	recset.Update();
	RefreshList();
	OnBnClickedDernier();
}