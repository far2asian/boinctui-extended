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
    std::vector<std::string> preferences =
        {
            "cpu_usage_limit",
            "max_ncpus_pct",
/*            "confirm_before_connecting",
            "cpu_scheduling_period_minutes",
            "daily_xfer_limit_mb",
            "daily_xfer_period_days",
            "disk_interval",
            "disk_max_used_gb",
            "disk_max_used_pct",
            "disk_min_free_gb",
            "dont_verify_images",
            "end_hour",
            "hangup_if_dialed",
            "idle_time_to_run",
            "leave_apps_in_memory",
            "max_bytes_sec_down",
            "max_bytes_sec_up",
            "net_end_hour",
            "net_start_hour",
            "ram_max_used_busy_pct",
            "ram_max_used_idle_pct",
            "run_gpu_if_user_active",
            "run_if_user_active",
            "run_on_batteries",
            "start_hour",
            "suspend_cpu_usage",
            "vm_max_used_pct",
            "work_buf_additional_days",*/
            "work_buf_min_days"
	};
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
    for (int i = 0; i < preferences.size(); i++)
    {
        f = addfield(new_field(1, 25, line, 2 , 0, 0));
        set_field_buffer(f, 0, preferences[i].c_str());
        set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
        field_opts_off(f, O_ACTIVE); // Static text
        max_ncpus_pct_field = getfieldcount();
        f = addfield(new_field(1, 15, line++, 25, 0, 0));
        set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
        strncpy(buf, srv->statedom.hookptr()->findItem(preferences[i].c_str())->getsvalue(), 128);
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
		char* max_ncpus_pct = rtrim(field_buffer(fields[max_ncpus_pct_field],0));
		char* cpu_usage_limit = rtrim(field_buffer(fields[cpu_usage_limit_field],0));
		kLogPrintf("PrefForm OK max_ncpus_pct=[%s] cpu_usage_limit=[%s]\n", max_ncpus_pct, cpu_usage_limit);
		if (srv!=NULL)
		{
		    std::string errmsg;
		    bool success = srv->prefupdate(max_ncpus_pct, cpu_usage_limit, errmsg);
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
