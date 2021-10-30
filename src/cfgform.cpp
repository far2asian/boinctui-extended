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

#include "cfgform.h"
#include "mbstring.h"
#include "kclog.h"
#include "tuievent.h"


CfgForm::CfgForm(int rows, int cols/*, Config* cfg*/) : NForm(rows,cols)
{
    genfields(false);
    //set_form_fields(frm, fields);
    post_form(frm);
}


void CfgForm::genfields(bool extfields) // Create an array of fields (extfields is set if you need to add a host)
{
    delfields();
    this->extfields = extfields;
    // Read from config
    Item* boinctui_cfg = gCfg->getcfgptr();
    if (boinctui_cfg == NULL)
	return;
    std::vector<Item*> slist = boinctui_cfg->getItems("server");
    if (slist.empty())
	extfields = true;
    // Input field for servers
    nhost = slist.size(); // Number of servers
    if (extfields)
	nhost++; // New additional host
    std::vector<Item*>::iterator it;
    int i  = 0; // Host number
    int nl = 2; // Screen line number
    // Static field header for hosts
    FIELD* field   = addfield(new_field(1, 53, nl, 5, 0, 0));
    field_opts_off(field, O_ACTIVE); // Static text
    set_field_buffer(field, 0, "host             port   pwd                   host id");
    set_field_back(field, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    nl = nl + 1;
    // Fields for hosts
    for (i = 0; i < nhost; i++) // Loop through hosts
    {
	// Host field
	field = addfield(new_field(1, 15, nl, 5, 0, 0));
	set_field_back(field, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
	field_opts_off(field, O_AUTOSKIP);
	field_opts_off(field, O_STATIC);
	set_max_field(field,128); //max width 128
	//set_field_type(field[nf], TYPE_ALNUM, 0);
    if (i < (int)slist.size())
	{
	    Item* host = slist[i]->findItem("host");
	    if (host != NULL)
		set_field_buffer(field, 0, host->getsvalue());
	}
	if (i == 0)
	    set_current_field(frm, field); // Set field focus
	// Port field
	field = addfield(new_field(1, 5, nl, 22, 0, 0));
	set_field_back(field, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
	set_field_type(field, TYPE_INTEGER, 0, 0, 65535);
        field_opts_off(field, O_AUTOSKIP);
    if (i < (int)slist.size())
	{
	    Item* port = slist[i]->findItem("port");
	    if (port != NULL)
		set_field_buffer(field, 0, port->getsvalue());
	}
	// Password field
	field = addfield(new_field(1, 20, nl, 29, 0, 0));
	set_field_back(field, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
	field_opts_off(field, O_AUTOSKIP);
	field_opts_off(field, O_STATIC);
	set_max_field(field,128); //max width 128
    if (i < (int)slist.size())
	{
	    Item* pwd = slist[i]->findItem("pwd");
	    if (pwd != NULL)
		set_field_buffer(field, 0, pwd->getsvalue());
	}
	// hostid field
	field = addfield(new_field(1, 20, nl, 51, 0, 0));
	set_field_back(field, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
	field_opts_off(field, O_AUTOSKIP);
	field_opts_off(field, O_STATIC);
	set_max_field(field,128);
	if (i < slist.size())
	{
	    Item* hostid = slist[i]->findItem("hostid");
	    if (hostid != NULL)
		set_field_buffer(field, 0, hostid->getsvalue());
	}
	nl = nl + 2;
    }
    // Control keys
    nl++;
    field = addfield(new_field(1, 44, nl, 5, 0, 0));
    field_opts_off(field, O_ACTIVE); // Static text
    if (extfields)
	set_field_buffer(field, 0, "Esc-Cancel   Enter-Accept");
    else
	set_field_buffer(field, 0, "Esc-Cancel   Enter-Accept   Ins-Add host");
    set_field_back(field, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    nl = nl + 2;
    // Finalise the list of fields
    addfield(NULL);
    // Recalculaye form height so everything looks right
    resize(nl + 2,getwidth());
    move(getmaxy(stdscr)/2-getheight()/2,getmaxx(stdscr)/2-getwidth()/2);
}


void CfgForm::eventhandle(NEvent* ev) // Event handler
{
    if ( ev->done )
	return;
    //NMouseEvent* mevent = (NMouseEvent*)ev;
    if ( ev->type == NEvent::evMOUSE)
    {
	//if (isinside(mevent->row, mevent->col))
	    NForm::eventhandle(ev); //предок
    }
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case KEY_IC: //INSERT
		if (!extfields)
		{
		    unpost_form(frm);
		    genfields(true);
		    post_form(frm);
		    refresh();
		    kLogPrintf("INSERT NEW HOST\n");
		}
		break;
	    case KEY_ENTER:
	    case '\n': // Enter
	    {
		form_driver(frm, REQ_NEXT_FIELD); // Hack so that the current field does not lose value
		kLogPrintf("ENTER\n");
		updatecfg(); // Update data in config
		gCfg->save(); // Save to disk
		//gsrvlist->refreshcfg();
		//ev->keycode = 27; //костыль чтобы осн программа сдестркутила форму конфига
		NEvent* event = new TuiEvent(evCFGCH);//NEvent(NEvent::evPROG, 1); // Create a software event
		putevent(event);
		break;
	    }
	    case 27:
		kLogPrintf("ESC\n");
		ev->done = false; //нет реакции на этот код (пусть получает владелец)
		break;
	    default:
		kLogPrintf("CfgForm::KEYCODE=%d\n", ev->keycode);
		ev->done = false;
		NForm::eventhandle(ev); //предок
		break;
	} //switch
    }
}


void	CfgForm::updatecfg() // Saves data from form to config
{
    Item* boinctui_cfg = gCfg->getcfgptr();
    if (boinctui_cfg == NULL)
	return;
    if (fields == NULL)
	return;
    // Delete all old server entries from the config
    std::vector<Item*> slist = boinctui_cfg->getItems("server");
    std::vector<Item*>::iterator it;
    for (it = slist.begin(); it != slist.end(); it++)
	boinctui_cfg->delsubitem(*it);
    // Create new records
    //int n = field_count(frm);
    for (int i = 0; i < nhost; i++) // Cycle through hosts from form
    {
	int nf = 1 + i*4; // Hostname field number
	char* shost = rtrim(field_buffer(fields[nf],0));
	char* sport = rtrim(field_buffer(fields[nf+1],0));
	char* spwd  = rtrim(field_buffer(fields[nf+2],0));
	char* shostid = rtrim(field_buffer(fields[nf+3],0));
	kLogPrintf("SERVER %d [%s:%s <%s> ID: %s]\n", i, shost, sport, spwd, shostid);
	gCfg->addhost(shost, sport, spwd, shostid);
    }
}
