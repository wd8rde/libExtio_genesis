// wxWidgets "Genesis_gui world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <pthread.h>
#include "wx_genesis.h"

#define LOG_ERR(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#define LOG_INFO(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#ifdef DO_ANNOYING
#define LOG_ANNOYING(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
#else
#define LOG_ANNOYING(...) {}
#endif

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};
class MyFrame: public MainFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnGenesis_gui(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
enum
{
    ID_Genesis_gui = 1
};
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Genesis_gui,   MyFrame::OnGenesis_gui)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
wxEND_EVENT_TABLE()
//wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( "Genesis_gui World", wxPoint(50, 50), wxSize(450, 340) );
    frame->Show( true );
    return true;
}
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : MainFrame(NULL, wxID_ANY, title, pos, size)
{
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Genesis_gui world sample",
                  "About Genesis_gui World", wxOK | wxICON_INFORMATION );
}
void MyFrame::OnGenesis_gui(wxCommandEvent& event)
{
    wxLogMessage("Genesis_gui world from wxWidgets!");
}


static bool gui_initialized = false;
static bool atexit_registered = false;
static void* gui_thread_func(void* p_data)
{
    wxApp* p_app = (wxApp*)p_data;
    LOG_ERR("%s:%d\n",__FUNCTION__,__LINE__);
    p_app->OnRun();
    LOG_ERR("%s:%d\n",__FUNCTION__,__LINE__);
}
IMPLEMENT_APP_NO_MAIN(MyApp)

void gui_onexit(void)
{
    LOG_ERR("%s:%d\n",__FUNCTION__,__LINE__);
    if(gui_initialized)
    {
        LOG_ERR("%s:%d\n",__FUNCTION__,__LINE__);
        wxTheApp->OnExit();
        wxEntryCleanup();
    }
    LOG_ERR("%s:%d\n",__FUNCTION__,__LINE__);
}

void show_gui()
{
    LOG_ERR("%s:%d\n",__FUNCTION__,__LINE__);
    if(!atexit_registered)
    {
        atexit(gui_onexit);
        atexit_registered = true;
    }

    if(!gui_initialized)
    {
        wxApp::SetInstance( new MyApp() );
        int argc = 1;
        wxChar *argv[2];
        argv[0] = L"test";
        argv[1] = NULL;
        wxEntryStart( argc, (wxChar**)argv );
        gui_initialized = true;
    }
    wxTheApp->OnInit();

    pthread_t gui_thread;
    int rslt = pthread_create(&gui_thread, 
                              NULL /* pthread_attr_t *attr*/, 
                              gui_thread_func, 
                              wxTheApp);
    if (0 != rslt)
    {
        LOG_ERR("%s:%d pthread_create failed %d, %s\n",__FUNCTION__,__LINE__, rslt, strerror(rslt));
    }
    LOG_ERR("%s:%d\n",__FUNCTION__,__LINE__);
}


