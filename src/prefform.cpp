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

#include <ctype.h>
#include "prefform.h"
#include "mbstring.h"
#include "kclog.h"
#include "tuievent.h"


PrefForm::PrefForm(int rows, int cols,  Srv* srv) : NForm(rows,cols)
{
    this->srv = srv;
    settitle("Activity Preferences");
    // Margins
    int row = 0;
    genfields(row);
    // Recalculate the height of the form, so that all the fields look right
    int r,c =0;
    scale_form(frm, &r, &c);
    kLogPrintf("field_count=%d scale_form()->%d,%d\n", field_count(frm), r, c);
    resize(r+3,c+2);

    set_current_field(frm, fields[0]); // Set focus on first field

    post_form(frm);
    this->refresh();
}


void PrefForm::genfields(int& line) //создаст массив полей
{
    FIELD* f;
    delfields();
    // Error message
    errmsgfield = getfieldcount();
    f = addfield(new_field(1, getwidth()-2, line++, 0, 0, 0));
    set_field_buffer(f, 0, "Error");
    set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_RED) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    field_opts_off(f, O_VISIBLE); // Default is invisible
    char buf[129];
    char* p;
    std::string pref_name;
    for (int i = 0; i < preferences.size(); i+=2)
    {
        pref_name = preferences[i];
        f = addfield(new_field(1, 20, line, 2 , 0, 0));
        set_field_buffer(f, 0, preferences[i].c_str());
        set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
        field_opts_off(f, O_ACTIVE); // Static text
        preference_fields[pref_name] = getfieldcount();
        f = addfield(new_field(1, 15, line, 18, 0, 0));
        set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
        strncpy(buf, srv->statedom.hookptr()->findItem(pref_name.c_str())->getsvalue(), 128);
        buf[128] = '\0';
        p = ltrim(buf);
        rtrim(buf);
        set_field_buffer(f, 0, p);
	if ((i + 1) == preferences.size())
	{
	    line++;
	    break;
	}
	pref_name = preferences[i+1];
        f = addfield(new_field(1, 20, line, 35 , 0, 0));
        set_field_buffer(f, 0, preferences[i+1].c_str());
        set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
        field_opts_off(f, O_ACTIVE); // Static text
        preference_fields[pref_name] = getfieldcount();
        f = addfield(new_field(1, 15, line++, 50, 0, 0));
        set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
        strncpy(buf, srv->statedom.hookptr()->findItem(pref_name.c_str())->getsvalue(), 128);
        buf[128] = '\0';
        p = ltrim(buf);
        rtrim(buf);
        set_field_buffer(f, 0, p);
        line++;
    }
    f = addfield(new_field(1, getwidth()-25, line++, 20 , 0, 0));
    set_field_buffer(f, 0, "Enter-Ok    Esc-Cancel");
    set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    // Finalise the list of fields
    addfield(NULL);
}


void PrefForm::eventhandle(NEvent* ev) // Event handler
{
    if ( ev->done )
	return;
    NMouseEvent* mevent = (NMouseEvent*)ev;
    if ( ev->type == NEvent::evMOUSE)
    {
	NForm::eventhandle(ev); //предок
    }
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case KEY_ENTER:
	    case '\n': // ENTER
	    {
		form_driver(frm, REQ_NEXT_FIELD); // Hack so that the current field does not lose value
		kLogPrintf("PrefForm OK\n");
	        for (int i = 0; i < preferences.size(); i++)
		{
		    kLogPrintf("%d\n", i);
		    preference_settings[preferences[i]] = rtrim(field_buffer(fields[preference_fields[preferences[i]]],0));
		}
		for(auto& x : preference_settings)
		{
		    kLogPrintf("%s=[%s]\n", x.first.c_str(), x.second);
		}
		if (srv!=NULL)
		{
		    std::string errmsg;
		    bool success = srv->prefupdate(preference_settings, errmsg);
		    if (!success)
		    {
			// Error message
			errmsg = " Error: " + errmsg;
			set_field_buffer(fields[errmsgfield], 0, errmsg.c_str());
			field_opts_on(fields[errmsgfield], O_VISIBLE); // Make error line visible
			break;
		    }
		}
		putevent(new TuiEvent(evACTPREF));
		break;
	    }
	    case 27:
		kLogPrintf("ESC\n");
		ev->done = false; //нет реакции на этот код (пусть получает владелец)
		break;
	    default:
		kLogPrintf("PrefForm::KEYCODE=%d\n", ev->keycode);
		ev->done = false;
		NForm::eventhandle(ev);
		break;
	} //switch
    }
}
