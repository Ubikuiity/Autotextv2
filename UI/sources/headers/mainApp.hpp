#pragma once

#include "ID.hpp"
#include <wx/wx.h>
#include "mainFrame.hpp"
#include "mainTrayIcon.hpp"

class MyApp : public wxApp
{
    public:
    virtual bool OnInit();
};
