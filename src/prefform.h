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

#ifndef PREFFORM_H
#define PREFFORM_H

#include <string.h>
#include <stdlib.h>
#include "nform.h"
#include "nstatictext.h"
#include "srvdata.h"

class PrefForm : public NForm
{
  public:
    PrefForm(int lines, int rows, Srv* srv);
    void genfields(int& line); // Create an array of fields
    virtual void eventhandle(NEvent* ev); // Event handler
  protected:
    int         max_ncpus_pct_field;
    int         cpu_usage_limit_field;
    int         namefield;
    int         errmsgfield;
    int         urlfield;
    Srv*        srv;
    std::string mgrname; // Manager name
    std::string mgrurl; // Manager URL

};


#endif //PREFFORM_H
