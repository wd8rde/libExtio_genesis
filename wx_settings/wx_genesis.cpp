///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx_genesis.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_MainPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bMainPanelSizer;
	bMainPanelSizer = new wxBoxSizer( wxVERTICAL );
	
	tabBox = new wxNotebook( m_MainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNB_TOP, wxT("Control") );
	m_ControlsTabPanel = new wxPanel( tabBox, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	m_ControlsBtnPanel = new wxPanel( m_ControlsTabPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDOUBLE_BORDER|wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_RFPreampBtn = new wxToggleButton( m_ControlsBtnPanel, wxID_ANY, wxT("RF Preamp"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_RFPreampBtn, 0, wxALL, 5 );
	
	m_AFPreampBtn = new wxToggleButton( m_ControlsBtnPanel, wxID_ANY, wxT("AF Preamp"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_AFPreampBtn, 0, wxALL, 5 );
	
	m_RFAttenBtn = new wxToggleButton( m_ControlsBtnPanel, wxID_ANY, wxT("RF Atten"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_RFAttenBtn, 0, wxALL, 5 );
	
	m_MuteBtn = new wxToggleButton( m_ControlsBtnPanel, wxID_ANY, wxT("Mute"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_MuteBtn, 0, wxALL, 5 );
	
	m_MonitorBtn = new wxToggleButton( m_ControlsBtnPanel, wxID_ANY, wxT("Monitor"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_MonitorBtn, 0, wxALL, 5 );
	
	m_TxBtn = new wxToggleButton( m_ControlsBtnPanel, wxID_ANY, wxT("TX"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TxBtn->SetFont( wxFont( 9, 74, 90, 90, false, wxT("Sans") ) );
	
	bSizer12->Add( m_TxBtn, 0, wxALL, 5 );
	
	m_ControlsBtnPanel->SetSizer( bSizer12 );
	m_ControlsBtnPanel->Layout();
	bSizer12->Fit( m_ControlsBtnPanel );
	bSizer10->Add( m_ControlsBtnPanel, 2, wxALIGN_BOTTOM|wxALL|wxEXPAND, 5 );
	
	wxString m_BandRadioBoxChoices[] = { wxT("160m"), wxT("80m"), wxT("60-40m"), wxT("30-20m"), wxT("17-15m"), wxT("12-10m"), wxT("6m"), wxT("gen") };
	int m_BandRadioBoxNChoices = sizeof( m_BandRadioBoxChoices ) / sizeof( wxString );
	m_BandRadioBox = new wxRadioBox( m_ControlsTabPanel, wxID_ANY, wxT("Band Filter"), wxDefaultPosition, wxDefaultSize, m_BandRadioBoxNChoices, m_BandRadioBoxChoices, 1, wxRA_SPECIFY_COLS );
	m_BandRadioBox->SetSelection( 7 );
	bSizer10->Add( m_BandRadioBox, 1, wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	m_ControlsTabPanel->SetSizer( bSizer10 );
	m_ControlsTabPanel->Layout();
	bSizer10->Fit( m_ControlsTabPanel );
	tabBox->AddPage( m_ControlsTabPanel, wxT("Controls"), true );
	m_OptionsTabPanel = new wxPanel( tabBox, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	m_OptionsChkPanel = new wxPanel( m_OptionsTabPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_PA10ChkBox = new wxCheckBox( m_OptionsChkPanel, wxID_ANY, wxT("PA10"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_PA10ChkBox, 0, wxALL, 5 );
	
	m_LineMicChkBox = new wxCheckBox( m_OptionsChkPanel, wxID_ANY, wxT("Line/Mic"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_LineMicChkBox, 0, wxALL, 5 );
	
	m_2ndRxChkBox = new wxCheckBox( m_OptionsChkPanel, wxID_ANY, wxT("2nd Rx"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_2ndRxChkBox, 0, wxALL, 5 );
	
	m_TvrChkBox = new wxCheckBox( m_OptionsChkPanel, wxID_ANY, wxT("Transverter"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_TvrChkBox, 0, wxALL, 5 );
	
	m_OptionsChkPanel->SetSizer( bSizer5 );
	m_OptionsChkPanel->Layout();
	bSizer5->Fit( m_OptionsChkPanel );
	bSizer7->Add( m_OptionsChkPanel, 1, wxEXPAND | wxALL, 5 );
	
	m_KeyerSubPanel = new wxPanel( m_OptionsTabPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_KeyerSubPanel, wxID_ANY, wxT("Keyer") ), wxVERTICAL );
	
	m_IambicChkBox = new wxCheckBox( m_KeyerSubPanel, wxID_ANY, wxT("Iambic"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_IambicChkBox, 0, wxALL, 5 );
	
	m_RevBChkBox = new wxCheckBox( m_KeyerSubPanel, wxID_ANY, wxT("Rev B"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_RevBChkBox, 0, wxALL, 5 );
	
	m_ReverseChkBox = new wxCheckBox( m_KeyerSubPanel, wxID_ANY, wxT("Reverse Paddles"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_ReverseChkBox, 0, wxALL, 5 );
	
	m_AutoCorChkBox = new wxCheckBox( m_KeyerSubPanel, wxID_ANY, wxT("Auto Correction"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer4->Add( m_AutoCorChkBox, 0, wxALL, 5 );
	
	m_KRatioSubPanel = new wxPanel( m_KeyerSubPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( m_KRatioSubPanel, wxID_ANY, wxT("Ratio") ), wxHORIZONTAL );
	
	m_KRatioLabel = new wxStaticText( m_KRatioSubPanel, wxID_ANY, wxT("1.0:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_KRatioLabel->Wrap( -1 );
	sbSizer6->Add( m_KRatioLabel, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_KRatioTextCtrl = new wxTextCtrl( m_KRatioSubPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_KRatioTextCtrl, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_KRatioSpinBtn = new wxSpinButton( m_KRatioSubPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_KRatioSpinBtn, 0, wxALIGN_CENTER|wxALL, 0 );
	
	m_KRatioSubPanel->SetSizer( sbSizer6 );
	m_KRatioSubPanel->Layout();
	sbSizer6->Fit( m_KRatioSubPanel );
	sbSizer4->Add( m_KRatioSubPanel, 0, wxALIGN_BOTTOM|wxALL|wxEXPAND, 0 );
	
	m_KeyerSubPanel->SetSizer( sbSizer4 );
	m_KeyerSubPanel->Layout();
	sbSizer4->Fit( m_KeyerSubPanel );
	bSizer7->Add( m_KeyerSubPanel, 1, wxEXPAND | wxALL, 0 );
	
	m_OptionsTabPanel->SetSizer( bSizer7 );
	m_OptionsTabPanel->Layout();
	bSizer7->Fit( m_OptionsTabPanel );
	tabBox->AddPage( m_OptionsTabPanel, wxT("Options"), false );
	
	bMainPanelSizer->Add( tabBox, 1, wxALL|wxEXPAND, 10 );
	
	m_sdbDialog = new wxStdDialogButtonSizer();
	m_sdbDialogOK = new wxButton( m_MainPanel, wxID_OK );
	m_sdbDialog->AddButton( m_sdbDialogOK );
	m_sdbDialogCancel = new wxButton( m_MainPanel, wxID_CANCEL );
	m_sdbDialog->AddButton( m_sdbDialogCancel );
	m_sdbDialog->Realize();
	bMainPanelSizer->Add( m_sdbDialog, 0, wxALIGN_RIGHT, 5 );
	
	m_MainPanel->SetSizer( bMainPanelSizer );
	m_MainPanel->Layout();
	bMainPanelSizer->Fit( m_MainPanel );
	bSizer2->Add( m_MainPanel, 1, wxEXPAND | wxALL, 10 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
}

MainFrame::~MainFrame()
{
}
