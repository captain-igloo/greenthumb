#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <wx/panel.h>

namespace greenthumb {

class ImagePanel : public wxPanel {
    public:
        ImagePanel(wxWindow* parent, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
            long style = wxTAB_TRAVERSAL, const wxString& name = wxPanelNameStr);

        void OnPaint(wxPaintEvent& event);

        void SetBitmap(const wxBitmap& bitmap);

    protected:
    private:
        wxBitmap bitmap;
};

}

#endif // IMAGEPANEL_H
