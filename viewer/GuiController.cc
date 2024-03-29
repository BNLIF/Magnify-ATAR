#include "GuiController.h"
#include "MainWindow.h"
#include "ViewWindow.h"
#include "ControlWindow.h"
#include "Data.h"

#include "TApplication.h"
#include "TSystem.h"
#include "TExec.h"
#include "TROOT.h"
#include "TMath.h"
#include "TGFileDialog.h"

#include "TGMenu.h"
#include "TGNumberEntry.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TH2I.h"
#include "TH1I.h"
#include "TBox.h"
#include "TLine.h"
#include "TColor.h"
#include "TLatex.h"

#include <iostream>
#include <vector>
using namespace std;

GuiController::GuiController(const TGWindow *p, int w, int h, const char* fn)
{
    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;

    TString filename;
    if (!fn) {
        cout << "no filename given, terminating" << endl;
        gApplication->Terminate(0);
    }
    else {
        filename = fn;
    }
    data = new Data(filename.Data());
    data->c1 = vw->can;

    InitConnections();

    data->Draw1();

}

GuiController::~GuiController()
{
    // gApplication->Terminate(0);
}

void GuiController::InitConnections()
{
    mw->fMenuFile->Connect("Activated(int)", "GuiController", this, "HandleMenu(int)");

    // cw->clusterEntry->SetLimitValues(0, data->nCluster-1);
    cw->clusterEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ClusterChanged(int)");

    // cw->clusterIdEntry->SetNumber(data->rec_cluster_id->at(currentCluster));
    // cw->clusterIdEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ClusterIdChanged(int)");

    // cw->pointIndexEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ZoomChanged()");

    // cw->zoomEntry->Connect("ValueSet(Long_t)", "GuiController", this, "ZoomChanged()");
    // cw->badChanelButton->Connect("Clicked()", "GuiController", this, "ToggleBadChannel()");
    // cw->drawTrackButton->Connect("Clicked()", "GuiController", this, "ToggleDrawTrack()");
    // cw->allClusterButton->Connect("Clicked()", "GuiController", this, "ToggleAllCluster()");

    cw->unZoomButton->Connect("Clicked()", "GuiController", this, "Play()");
    // cw->rangeZoomButton->Connect("Clicked()", "GuiController", this, "RangeZoom()");

    vw->can->Connect(
        "ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
        "GuiController",
        this,
        "ProcessCanvasEvent(Int_t,Int_t,Int_t,TObject*)"
    );
}

void GuiController::ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject* selected)
{
    if (ev == 1) { // clicked
        // if (!(selected->IsA() == TGraph::Class()
        //     || selected->IsA() == TH2F::Class()
        //     // || selected->IsA() == TLine::Class()
        // )) return;
        TVirtualPad* pad = vw->can->GetClickSelectedPad();
        int padNo = pad->GetNumber();
        double xx = pad->AbsPixeltoX(x);
        double yy = pad->AbsPixeltoY(y);
        // int ci = data->FindClusterIndex(xx, yy);
        // int pi = data->FindPointIndex(xx, yy);
        cout << "pad " << padNo << ": (" << xx << ", " << yy << ")" 
            // << "; cluster index: " << ci
            // << "; point index: " << pi
            << endl;
    }
}

void GuiController::ClusterChanged(int i)
{
    int n = cw->clusterEntry->GetNumber();
    data->Draw1(n);
    cout << "n changed to " << n << endl;
}

void GuiController::Play()
{
    cout << "chao clicked this button" << endl;
}


void GuiController::HandleMenu(int id)
{
    switch (id) {
        case M_FILE_EXIT:
            gApplication->Terminate(0);
            break;
    }
}

