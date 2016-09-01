#include <wx/dcclient.h>

#include "ImagePanel.h"

namespace greenthumb {

ImagePanel::ImagePanel(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
    long style, const wxString& name) : wxPanel(parent, id, pos, size, style, name) {

    Bind(wxEVT_PAINT, &ImagePanel::OnPaint, this);

}

void ImagePanel::OnPaint(wxPaintEvent& event) {

    if (bitmap.IsOk()) {
        wxPaintDC dc(this);
        PrepareDC(dc);
        dc.DrawBitmap(bitmap, 0, 0);
    }

}

void ImagePanel::SetBitmap(const wxBitmap& bitmap) {
    this->bitmap = bitmap;
}

}
