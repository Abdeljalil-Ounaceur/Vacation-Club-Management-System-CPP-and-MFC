// Villages.cpp : implementation file
//

#include "stdafx.h"
#include "gestionClub.h"
#include "Villages.h"
#include "afxdialogex.h"


// Villages dialog

IMPLEMENT_DYNAMIC(Villages, CDialogEx)

Villages::Villages(CWnd* pParent /*=NULL*/)
: CDialogEx(Villages::IDD, pParent),db(CDaoDatabase()),recset(&db),activitesRecset(&db),languesRecset(&db)
	, m_CodeVillageEdit(_T(""))
	, m_NomVillageEdit(_T(""))
	, m_ActiviteGratuite(FALSE)
{
	defaultQuery = L"SELECT No_Village, Code_Village, Village.Nom, Village.Pays, Pays.Nom FROM Village inner join Pays on Village.Pays = Pays.No_Pays";
	defaultActivitesQuery = L"SELECT Proposer.No_Activite as No_Activite, Designation, Gratuite, No_Village FROM Proposer INNER JOIN Activites ON Activites.No_Activite = Proposer.No_Activite";
	defaultLanguesQuery = L"SELECT Parler.No_Langue as No_Langue, Designation, No_Village FROM Parler INNER JOIN Langues ON Langues.No_Langue = Parler.No_Langue";
	db.Open(L"base.mdb");

	Create(Villages::IDD, pParent);
	ShowWindow(SW_SHOW);
}

Villages::~Villages()
{

	if(activitesRecset.IsOpen()){
		recset.Close();
	}

	if(languesRecset.IsOpen()){
		recset.Close();
	}

	if(recset.IsOpen()){
		recset.Close();
	}

	if(db.IsOpen()){
		db.Close();
	}
}

void Villages::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VILLAGES, m_ListVillages);
	DDX_Text(pDX, IDC_CODE_VILLAGE_EDIT, m_CodeVillageEdit);
	DDV_MaxChars(pDX, m_CodeVillageEdit, 3);
	DDX_Text(pDX, IDC_NOM_VILLAGE_EDIT, m_NomVillageEdit);
	DDV_MaxChars(pDX, m_NomVillageEdit, 30);
	DDX_Control(pDX, IDC_VILLAGE_PAYS_COMBO, m_PaysCombo);
	DDX_Control(pDX, IDC_CODE_VILLAGE_EDIT, m_CodeVillageControl);
	DDX_Control(pDX, IDC_NOM_VILLAGE_EDIT, m_NomVillageControl);

	DDX_Control(pDX, IDC_LIST_ACTIVITES, m_ListCurrentActivites);
	DDX_Control(pDX, IDC_COMBO_ACTIVITES, m_ActivitesCombo);
	DDX_Control(pDX, IDC_ACTIVITE_GRATUITE, m_ActiviteCheckBox);
	DDX_Check(pDX, IDC_ACTIVITE_GRATUITE, m_ActiviteGratuite);

	DDX_Control(pDX, IDC_LIST_LANGUES, m_ListCurrentLangues);
	DDX_Control(pDX, IDC_COMBO_LANGUES, m_LanguesCombo);
}


BEGIN_MESSAGE_MAP(Villages, CDialogEx)
	ON_BN_CLICKED(IDC_PREMIER, &Villages::OnBnClickedPremier)
	ON_BN_CLICKED(IDC_DERNIER, &Villages::OnBnClickedDernier)
	ON_BN_CLICKED(IDC_NEXT, &Villages::OnBnClickedNext)
	ON_BN_CLICKED(IDC_PREVIOUS, &Villages::OnBnClickedPrevious)
	ON_BN_CLICKED(IDC_TROUVER, &Villages::OnBnClickedTrouver)
	ON_BN_CLICKED(IDC_ACTUALISER, &Villages::OnBnClickedActualiser)
	ON_BN_CLICKED(IDC_SUPPRIMER, &Villages::OnBnClickedSupprimer)
	ON_BN_CLICKED(IDC_MODIFIER, &Villages::OnBnClickedModifier)
	ON_BN_CLICKED(IDC_AJOUTER, &Villages::OnBnClickedAjouter)

	ON_BN_CLICKED(IDC_PREMIER_ACTIVITES, &Villages::OnBnClickedPremierActivites)
	ON_BN_CLICKED(IDC_PREVIOUS_ACTIVITES, &Villages::OnBnClickedPreviousActivites)
	ON_BN_CLICKED(IDC_NEXT_ACTIVITES, &Villages::OnBnClickedNextActivites)
	ON_BN_CLICKED(IDC_DERNIER_ACTIVITES, &Villages::OnBnClickedDernierActivites)
	ON_BN_CLICKED(IDC_DISASSOCIER_ACTIVITES, &Villages::OnBnClickedDisassocierActivites)
	ON_BN_CLICKED(IDC_AJOUTER_ACTIVITES, &Villages::OnBnClickedAjouterActivites)
	ON_BN_CLICKED(IDC_MODIFIER_ACTIVITES, &Villages::OnBnClickedModifierActivites)

	ON_BN_CLICKED(IDC_PREMIER_LANGUES, &Villages::OnBnClickedPremierLangues)
	ON_BN_CLICKED(IDC_PREVIOUS_LANGUES, &Villages::OnBnClickedPreviousLangues)
	ON_BN_CLICKED(IDC_NEXT_LANGUES, &Villages::OnBnClickedNextLangues)
	ON_BN_CLICKED(IDC_DERNIER_LANGUES, &Villages::OnBnClickedDernierLangues)
	ON_BN_CLICKED(IDC_DISASSOCIER_LANGUES, &Villages::OnBnClickedDisassocierLangues)
	ON_BN_CLICKED(IDC_AJOUTER_LANGUES, &Villages::OnBnClickedAjouterLangues)
END_MESSAGE_MAP()


// Villages message handlers
BOOL Villages::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	//VILLAGES
	m_ListVillages.InsertColumn(0,L"No_Village",LVCFMT_CENTER,82,-1);
	m_ListVillages.InsertColumn(1,L"Code_Village",LVCFMT_CENTER,85,-1);
	m_ListVillages.InsertColumn(2,L"Nom",LVCFMT_CENTER,120,-1);
	m_ListVillages.InsertColumn(3,L"Pays",LVCFMT_CENTER,120,-1);
	ListView_SetExtendedListViewStyle(m_ListVillages, LVS_EX_GRIDLINES);
	m_ListVillages.SetBkColor(RGB(58, 210, 255));
	m_ListVillages.DeleteAllItems();


	m_CodeVillageControl.SetCueBanner(L"Code");
	m_NomVillageControl.SetCueBanner(L"Nom");
	m_PaysCombo.SetCueBanner(L"Pays");

	CDaoRecordset paysSet(&db);
	paysSet.Open(AFX_DAO_USE_DEFAULT_TYPE,L"select * from Pays Order By  Nom",NULL);
	COleVariant var;
	var.ChangeType(VT_BSTR, NULL);
	CString Nom_Pays;

	int i = 0;
	while(!paysSet.IsEOF()){
		paysSet.GetFieldValue(L"Nom",var);
		Nom_Pays = (LPCWSTR)var.pbstrVal;
		m_PaysCombo.InsertString(i,Nom_Pays);
		i++;
		paysSet.MoveNext();
	}
	paysSet.Close();


	//ACTIVITES
	m_ListCurrentActivites.InsertColumn(0,L"Activite",LVCFMT_CENTER,135,-1);
	m_ListCurrentActivites.InsertColumn(1,L"Gratuite",LVCFMT_CENTER,80,-1);
	ListView_SetExtendedListViewStyle(m_ListCurrentActivites, LVS_EX_GRIDLINES);
	m_ListCurrentActivites.SetBkColor(RGB(68, 220, 75));
	m_ListCurrentActivites.DeleteAllItems();

	m_ActivitesCombo.SetCueBanner(L"Activités");

	CDaoRecordset ActiviteSet(&db);
	ActiviteSet.Open(AFX_DAO_USE_DEFAULT_TYPE,L"select * from Activites",NULL);
	var.ChangeType(VT_BSTR, NULL);
	CString Designation;

	i = 0;
	while(!ActiviteSet.IsEOF()){
		ActiviteSet.GetFieldValue(L"Designation",var);
		Designation = (LPCWSTR)var.pbstrVal;
		m_ActivitesCombo.InsertString(i,Designation);
		i++;
		ActiviteSet.MoveNext();
	}
	ActiviteSet.Close();

	//LANGUES
	m_ListCurrentLangues.InsertColumn(0,L"Langue",LVCFMT_CENTER,170,-1);
	ListView_SetExtendedListViewStyle(m_ListCurrentLangues, LVS_EX_GRIDLINES);
	m_ListCurrentLangues.SetBkColor(RGB(255, 160, 8));
	m_ListCurrentLangues.DeleteAllItems();

	m_LanguesCombo.SetCueBanner(L"Langues");

	CDaoRecordset LangueSet(&db);
	LangueSet.Open(AFX_DAO_USE_DEFAULT_TYPE,L"select * from Langues",NULL);
	var.ChangeType(VT_BSTR, NULL);
	//CString Designation;

	i = 0;
	while(!LangueSet.IsEOF()){
		LangueSet.GetFieldValue(L"Designation",var);
		Designation = (LPCWSTR)var.pbstrVal;
		m_LanguesCombo.InsertString(i,Designation);
		i++;
		LangueSet.MoveNext();
	}
	LangueSet.Close();

	
	RefreshList();

	return TRUE;
}

//// VILLAGES

void Villages::ListData(CString query)
{
  COleVariant var;           // start Variables
  var.ChangeType(VT_BSTR, NULL);
  CString Code_Village, Nom_Village, Pays, No_Village; // end variables
  m_ListVillages.DeleteAllItems();                                        // clear list
  if(query == L""){ query = defaultQuery; }
  query = query + L" Order by No_Village";
  if(recset.IsOpen()){ recset.Close(); }	
  recset.Open(AFX_DAO_USE_DEFAULT_TYPE, query, NULL);
  
  int i = 0;
  while (!recset.IsEOF()) // do until eof
  {
    recset.GetFieldValue(L"No_Village", var);
    No_Village.Format(L"%d", (LPCWSTR)var.pbstrVal);
    m_ListVillages.InsertItem(i, No_Village, 0);
    recset.GetFieldValue(L"Code_Village", var);
    Code_Village = (LPCWSTR)var.pbstrVal;
    m_ListVillages.SetItemText(i, 1, Code_Village);
    recset.GetFieldValue(L"Village.Nom", var);
    Nom_Village = (LPCWSTR)var.pbstrVal;
    m_ListVillages.SetItemText(i, 2, Nom_Village);
    recset.GetFieldValue(L"Pays.Nom", var);
    Pays = (LPCWSTR)var.pbstrVal;
    m_ListVillages.SetItemText(i, 3, Pays);
    i++;
    recset.MoveNext();
  }
  m_CodeVillageEdit = L"";
  m_NomVillageEdit = L"";
  m_PaysCombo.SetCurSel(CB_ERR);
  UpdateData(false);
  RefreshActivites();
  RefreshLangues();
}

void Villages::RefreshList(){
	ListData(L"");
}

void Villages::OnBnClickedPremier()
{
	recset.MoveFirst();
	UpdateFields();
	
}


void Villages::OnBnClickedDernier()
{
	recset.MoveLast();
	UpdateFields();
}


void Villages::OnBnClickedNext()
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


void Villages::OnBnClickedPrevious()
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



void Villages::OnBnClickedTrouver()
{
	// TODO: Add your control notification handler code here
	CString Code_Village;
	GetDlgItemText(IDC_CODE_VILLAGE_EDIT, Code_Village);
	if(Code_Village != L""){
		ListData(defaultQuery +  L" WHERE Code_Village='" + Code_Village + L"'");
		OnBnClickedDernier();
	}
	
}

void Villages::OnBnClickedActualiser()
{
	// TODO: Add your control notification handler code here
	RefreshList();
}


void Villages::OnBnClickedSupprimer()
{
	// TODO: Add your control notification handler code here
	if(!recset.IsBOF() && !recset.IsEOF()){
		
		//////////
		COleVariant var;
		recset.GetFieldValue(L"No_Village", var);
		CString No_Village;
		No_Village.Format(L"%d", (LPCWSTR)var.pbstrVal);
		db.Execute(L"DELETE FROM Parler WHERE No_Village = "+No_Village);
		db.Execute(L"DELETE FROM Proposer WHERE No_Village = "+No_Village);
		//////////

		recset.Delete();
		RefreshList();
		AfxMessageBox(L"supprimé avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Villages::OnBnClickedModifier()
{
	// TODO: Add your control notification handler code here
	if(!recset.IsBOF() && !recset.IsEOF()){
		GetDlgItemText(IDC_CODE_VILLAGE_EDIT, m_CodeVillageEdit);
		GetDlgItemText(IDC_NOM_VILLAGE_EDIT, m_NomVillageEdit);
		recset.Edit();
		recset.SetFieldValue(L"Code_Village", (LPCWSTR)m_CodeVillageEdit);
		recset.SetFieldValue(L"Village.Nom", (LPCWSTR)m_NomVillageEdit);
		recset.SetFieldValue(L"Pays", GetComboPaysID());

		recset.Update();
		RefreshList();
	}
}

void Villages::OnBnClickedAjouter()
{
	// TODO: Add your control notification handler code here
	CString Code_Village, Nom_Village;
	GetDlgItemText(IDC_CODE_VILLAGE_EDIT, Code_Village);
	GetDlgItemText(IDC_NOM_VILLAGE_EDIT, Nom_Village);
	recset.AddNew();
	recset.SetFieldValue(L"Code_Village", (LPCWSTR)Code_Village);
	recset.SetFieldValue(L"Village.Nom", (LPCWSTR)Nom_Village);
	recset.SetFieldValue(L"Pays", GetComboPaysID());
	recset.Update();
	RefreshList();
	OnBnClickedDernier();
}

void Villages::UpdateFields(){

	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	recset.GetFieldValue(L"Code_Village", var);
	m_CodeVillageEdit = (LPCWSTR)var.pbstrVal;
	recset.GetFieldValue(L"Village.Nom", var);
	m_NomVillageEdit = (LPCWSTR)var.pbstrVal;
	recset.GetFieldValue(L"Pays.Nom", var);
	m_PaysCombo.SelectString(0,(LPCWSTR)var.pbstrVal);
	UpdateData(false);

	RefreshActivites();
	RefreshLangues();
}

COleVariant Villages::GetComboPaysID(){
	CString pays;
		m_PaysCombo.GetLBText(m_PaysCombo.GetCurSel(),pays);
		CDaoRecordset paysSet(&db);
		COleVariant No_Pays((LONG)0);
		paysSet.Open(AFX_DAO_USE_DEFAULT_TYPE, L"select * from pays where Nom = '"+pays+L"'", NULL);
		if (!paysSet.IsEOF()) {
			paysSet.MoveFirst();
			paysSet.GetFieldValue(L"No_Pays", No_Pays);
		}
		paysSet.Close();
		return No_Pays;
}

//// END VILLAGES

//// ACTIVITES
void Villages::RefreshActivites(){

	if(activitesRecset.IsOpen()){ activitesRecset.Close(); }

	CString query;
	int No_Village = !recset.IsEOF() ? recset.GetFieldValue(L"No_Village").intVal : 0;
    query.Format(_T("%s WHERE No_Village = %d"), defaultActivitesQuery, No_Village);
	activitesRecset.Open(AFX_DAO_USE_DEFAULT_TYPE, query , NULL);
	
	m_ListCurrentActivites.DeleteAllItems();
	COleVariant var;
	CString Designation, Gratuite;
	BOOL bGratuite;	
	int i = 0;
	while (!activitesRecset.IsEOF()) // do until eof
	{
		m_ListCurrentActivites.InsertItem(i, 0, 0);
		activitesRecset.GetFieldValue(L"Designation", var);
		Designation = (LPCWSTR)var.pbstrVal;
		m_ListCurrentActivites.SetItemText(i, 0, Designation);
		activitesRecset.GetFieldValue(L"Gratuite", var);
		bGratuite = var.boolVal;
		Gratuite = bGratuite? L"Oui":L"Non";
		m_ListCurrentActivites.SetItemText(i, 1, Gratuite);
		i++;
		activitesRecset.MoveNext();
	}
	m_ActiviteGratuite = FALSE;
	m_ActivitesCombo.SetCurSel(CB_ERR);

}

void Villages::OnBnClickedPremierActivites()
{
	// TODO: Add your control notification handler code here
	activitesRecset.MoveFirst();
	activitesUpdateFields();
}


void Villages::OnBnClickedPreviousActivites()
{
	// TODO: Add your control notification handler code here
	if(!activitesRecset.IsBOF()){
		try{
			activitesRecset.MovePrev();
			activitesUpdateFields();
		}catch(...){
			OnBnClickedNextActivites();
		}
	}
}


void Villages::OnBnClickedNextActivites()
{
	// TODO: Add your control notification handler code here
	if(!activitesRecset.IsEOF()){
		try{
			activitesRecset.MoveNext();
			activitesUpdateFields();
		}catch(...){
			OnBnClickedPreviousActivites();
		}
	}else{
		OnBnClickedPremierActivites();
	}
}


void Villages::OnBnClickedDernierActivites()
{
	// TODO: Add your control notification handler code here
	activitesRecset.MoveLast();
	activitesUpdateFields();
}


void Villages::OnBnClickedDisassocierActivites()
{
	// TODO: Add your control notification handler code here
	if(!activitesRecset.IsBOF() && !activitesRecset.IsEOF()){
		activitesRecset.Delete();
		RefreshActivites();
		AfxMessageBox(L"acitivité disassociée avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Villages::OnBnClickedModifierActivites()
{
	// TODO: Add your control notification handler code here
	if(!activitesRecset.IsBOF() && !activitesRecset.IsEOF()){
		BOOL bGratuite = m_ActiviteCheckBox.GetCheck() == BST_CHECKED;
		COleVariant No_Village = recset.GetFieldValue(L"No_Village");
		activitesRecset.Edit();
		activitesRecset.SetFieldValue(L"No_Village", No_Village);
		activitesRecset.SetFieldValue(L"No_Activite", GetComboActiviteID());
		activitesRecset.SetFieldValue(L"Gratuite", COleVariant((LONG) bGratuite));
		activitesRecset.Update();
		RefreshActivites();
	}
}


void Villages::OnBnClickedAjouterActivites()
{
	//TODO: Add your control notification handler code here
	BOOL bGratuite = m_ActiviteCheckBox.GetCheck() == BST_CHECKED;
	COleVariant No_Village = recset.GetFieldValue(L"No_Village");
	activitesRecset.AddNew();
	activitesRecset.SetFieldValue(L"No_Village", No_Village);
	activitesRecset.SetFieldValue(L"No_Activite", GetComboActiviteID());
	activitesRecset.SetFieldValue(L"Gratuite", COleVariant((LONG) bGratuite));
	activitesRecset.Update();
	RefreshActivites();
	OnBnClickedDernierActivites();

}

void Villages::activitesUpdateFields(){
	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	activitesRecset.GetFieldValue(L"Designation", var);
	m_ActivitesCombo.SelectString(0,(LPCWSTR)var.pbstrVal);
	activitesRecset.GetFieldValue(L"Gratuite", var);
	m_ActiviteGratuite = var.boolVal? TRUE: FALSE;
	UpdateData(false);
}

COleVariant Villages::GetComboActiviteID(){
	CString Activite;
	m_ActivitesCombo.GetLBText(m_ActivitesCombo.GetCurSel(),Activite);
	CDaoRecordset ActiviteSet(&db);
	COleVariant No_Activite((LONG)0);
	ActiviteSet.Open(AFX_DAO_USE_DEFAULT_TYPE, L"select * from Activites where Designation = '"+Activite+L"'", NULL);
	if (!ActiviteSet.IsEOF()) {
		ActiviteSet.MoveFirst();
		ActiviteSet.GetFieldValue(L"No_Activite", No_Activite);
	}
	ActiviteSet.Close();
	return No_Activite;
}


//// END ACTIVITES




//// LANGUES
void Villages::RefreshLangues(){

	if(languesRecset.IsOpen()){ languesRecset.Close(); }

	CString query;
	int No_Village = !recset.IsEOF() ? recset.GetFieldValue(L"No_Village").intVal : 0;
    query.Format(_T("%s WHERE No_Village = %d"), defaultLanguesQuery, No_Village);
	languesRecset.Open(AFX_DAO_USE_DEFAULT_TYPE, query , NULL);
	
	m_ListCurrentLangues.DeleteAllItems();
	COleVariant var;
	CString Designation;
	int i = 0;
	while (!languesRecset.IsEOF()) // do until eof
	{
		m_ListCurrentLangues.InsertItem(i, 0, 0);
		languesRecset.GetFieldValue(L"Designation", var);
		Designation = (LPCWSTR)var.pbstrVal;
		m_ListCurrentLangues.SetItemText(i, 0, Designation);
		i++;
		languesRecset.MoveNext();
	}
	m_LanguesCombo.SetCurSel(CB_ERR);

}

void Villages::OnBnClickedPremierLangues()
{
	// TODO: Add your control notification handler code here
	languesRecset.MoveFirst();
	languesUpdateFields();
}


void Villages::OnBnClickedPreviousLangues()
{
	// TODO: Add your control notification handler code here
	if(!languesRecset.IsBOF()){
		try{
			languesRecset.MovePrev();
			languesUpdateFields();
		}catch(...){
			OnBnClickedNextLangues();
		}
	}
}


void Villages::OnBnClickedNextLangues()
{
	// TODO: Add your control notification handler code here
	if(!languesRecset.IsEOF()){
		try{
			languesRecset.MoveNext();
			languesUpdateFields();
		}catch(...){
			OnBnClickedPreviousLangues();
		}
	}else{
		OnBnClickedPremierLangues();
	}
}


void Villages::OnBnClickedDernierLangues()
{
	// TODO: Add your control notification handler code here
	languesRecset.MoveLast();
	languesUpdateFields();
}


void Villages::OnBnClickedDisassocierLangues()
{
	// TODO: Add your control notification handler code here
	if(!languesRecset.IsBOF() && !languesRecset.IsEOF()){
		languesRecset.Delete();
		RefreshLangues();
		AfxMessageBox(L"langue disassociée avec succès !", MB_ICONINFORMATION | MB_OK);
	}
}


void Villages::OnBnClickedAjouterLangues()
{
	// TODO: Add your control notification handler code here
	COleVariant No_Village = recset.GetFieldValue(L"No_Village");
	languesRecset.AddNew();
	languesRecset.SetFieldValue(L"No_Village", No_Village);
	languesRecset.SetFieldValue(L"No_Langue", GetComboLangueID());
	languesRecset.Update();

	RefreshLangues();
	OnBnClickedDernierLangues();
}


void Villages::languesUpdateFields(){
	COleVariant var; // start Variables
	var.ChangeType(VT_BSTR, NULL);
	languesRecset.GetFieldValue(L"Designation", var);
	m_LanguesCombo.SelectString(0,(LPCWSTR)var.pbstrVal);
	UpdateData(false);
}

COleVariant Villages::GetComboLangueID(){
	CString Langue;
	m_LanguesCombo.GetLBText(m_LanguesCombo.GetCurSel(),Langue);
	CDaoRecordset LangueSet(&db);
	COleVariant No_Langue((LONG)0);
	LangueSet.Open(AFX_DAO_USE_DEFAULT_TYPE, L"select * from Langues where Designation = '"+Langue+L"'", NULL);
	if (!LangueSet.IsEOF()) {
		LangueSet.MoveFirst();
		LangueSet.GetFieldValue(L"No_Langue", No_Langue);
	}
	LangueSet.Close();
	return No_Langue;
}

//// END LANGUES

