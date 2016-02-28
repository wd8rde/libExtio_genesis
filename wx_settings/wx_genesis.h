///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 29 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wx_genesis__
#define __wx_genesis__

#include <wx/string.h>
#include <wx/tglbtn.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/spinbutt.h>
#include <wx/statbox.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		wxPanel* m_MainPanel;
		wxNotebook* tabBox;
		wxPanel* m_ControlsTabPanel;
		wxPanel* m_ControlsBtnPanel;
		wxToggleButton* m_RFPreampBtn;
		wxToggleButton* m_AFPreampBtn;
		wxToggleButton* m_RFAttenBtn;
		wxToggleButton* m_MuteBtn;
		wxToggleButton* m_MonitorBtn;
		wxToggleButton* m_TxBtn;
		wxRadioBox* m_BandRadioBox;
		wxPanel* m_OptionsTabPanel;
		wxPanel* m_OptionsChkPanel;
		wxCheckBox* m_PA10ChkBox;
		wxCheckBox* m_LineMicChkBox;
		wxCheckBox* m_2ndRxChkBox;
		wxCheckBox* m_TvrChkBox;
		wxPanel* m_KeyerSubPanel;
		wxCheckBox* m_IambicChkBox;
		wxCheckBox* m_RevBChkBox;
		wxCheckBox* m_ReverseChkBox;
		wxCheckBox* m_AutoCorChkBox;
		wxPanel* m_KRatioSubPanel;
		wxStaticText* m_KRatioLabel;
		wxTextCtrl* m_KRatioTextCtrl;
		wxSpinButton* m_KRatioSpinBtn;
		wxStdDialogButtonSizer* m_sdbDialog;
		wxButton* m_sdbDialogOK;
		wxButton* m_sdbDialogCancel;
	
	public:
		
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Genesis"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrame();
	
};

#endif //__wx_genesis__
