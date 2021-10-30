// =============================================================================
// This file is part of boinctui.
// http://boinctui.googlecode.com
// Copyright (C) 2012,2013 Sergey Suslov
//
// boinctui is free software; you can redistribute it and/or modify it  under
// the terms of the GNU General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// boinctui is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details
// <http://www.gnu.org/licenses/>.
// =============================================================================

#ifndef ADDPROJECTFORM_H
#define ADDPROJECTFORM_H

#include <string.h>
#include <stdlib.h>
#include "nform.h"
#include "nstatictext.h"
#include "srvdata.h"


class AddProjectForm : public NForm
{
  public:
    AddProjectForm(int lines, int rows, Srv* srv, const char* projname, bool userexist, bool byurl);
    void genfields(int& line, Item* project); // Create an array of fields
    virtual void eventhandle(NEvent* ev); // Event handler
  protected:
    int   projurlfield;
    int		emailfield;
    int		passwfield;
    int		errmsgfield;
    int		usernamefield;
    int		teamfield;
    Srv*	srv;
    std::string projname;	// Project name
    std::string projurl;	// Project URL
    bool	userexist;	// True if the user is already created
    bool  byurl; //сделать строку url редактируемой
};


#endif //ADDPROJECTFORM_H
