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
#include "addmgrform.h"
#include "mbstring.h"
#include "kclog.h"
#include "tuievent.h"


char* strlowcase(char* s); // To lower case


AddAccMgrForm::AddAccMgrForm(int rows, int cols,  Srv* srv, const char* mgrname) : NForm(rows,cols)
{
    this->srv = srv;
    settitle(mgrname);
    this->mgrname = mgrname;
    Item* account_manager = NULL;
    if (srv !=NULL)
	account_manager = srv->findaccountmanager(mgrname);
    // Margins
    int row = 0;
    genfields(row,account_manager);
    // Recalculate the height of the form, so that all the fields look right
    int r,c =0;
    scale_form(frm, &r, &c);
    kLogPrintf("field_count=%d scale_form()->%d,%d\n", field_count(frm), r, c);
    resize(r+3,c+2);

    set_current_field(frm, fields[0]); // Set focus on first field

    post_form(frm);
    this->refresh();
}


void AddAccMgrForm::genfields(int& line, Item* mgr) //создаст массив полей
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
    // Get the url and the name of the manager (either from the config or from the boinc client)
    if (mgr != NULL)
    {
        Item* url = mgr->findItem("url");
        if (url !=NULL)
	    mgrurl = url->getsvalue();
    }
    else
    {
        // Take the url from the config (if any)
	Item* boinctui_cfg = gCfg->getcfgptr();
	if (boinctui_cfg != NULL)
	{
	    std::vector<Item*> mgrlist = boinctui_cfg->getItems("accmgr");
	    std::vector<Item*>::iterator it;
	    for (it = mgrlist.begin(); it != mgrlist.end(); it++)
	    {
		Item* name = (*it)->findItem("name");
		if (name != NULL)
		    if (name->getsvalue() == mgrname)
		    {
			Item* url = (*it)->findItem("url");
			if (url != NULL)
			{
			    mgrurl = url->getsvalue();
			    break;
			}
		    }
	    }
	}
    }
    // Manager name
    f = addfield(new_field(1, getwidth()-4, line, 2, 0, 0));
    set_field_buffer(f, 0, "Description  ");
    set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    namefield = getfieldcount();
    f = addfield(new_field(1, 40, line++, 15, 0, 0));
    if (mgr != NULL)
    {
	field_opts_off(f, O_STATIC);
	field_opts_off(f, O_ACTIVE); // Static text
    }
    else
	set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
    field_opts_off(f, O_AUTOSKIP);
    set_max_field(f,128); // Max width 128
    char buf[129];
    strncpy(buf, gettitle(), 128);
    buf[128] = '\0';
    char* p;
    p = ltrim(buf);
    rtrim(buf);
    set_field_buffer(f, 0, p);
    // url
    line++;
    f = addfield(new_field(1, getwidth()-4, line, 2, 0, 0));
    set_field_buffer(f, 0, "URL          ");
    set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    urlfield = getfieldcount();
    f = addfield(new_field(1, 40, line++, 15, 0, 0));
    if (mgr != NULL)
    {
	field_opts_off(f, O_STATIC);
	field_opts_off(f, O_ACTIVE); // Static text
    }
    else
	set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
    field_opts_off(f, O_AUTOSKIP);
    set_max_field(f,128); // Max width 128
    set_field_buffer(f, 0, mgrurl.c_str());
    // Help text
    line++;
    f = addfield(new_field(3, getwidth()-4, line++, 2, 0, 0));
    set_field_buffer(f, 0,  "If you have not yet registered with this account manager" \
    			"     please do so before proceeding.");
    set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    line = line + 2;
    // User name
    line++;
    f = addfield(new_field(1, 10, line, 2 , 0, 0));
    set_field_buffer(f, 0, "username");
    set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    usernamefield = getfieldcount();
    f = addfield(new_field(1, 40, line++, 15, 0, 0));
    field_opts_off(f, O_AUTOSKIP);
    set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
    // Password
    line++;
    f = addfield(new_field(1, 10, line, 2 , 0, 0));
    set_field_buffer(f, 0, "password");
    set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    passwfield = getfieldcount();
    f = addfield(new_field(1, 40, line++, 15, 0, 0));
    set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD);
    field_opts_off(f, O_AUTOSKIP);
    // Control keys
    line++;
    f = addfield(new_field(1, getwidth()-25, line++, 20 , 0, 0));
    set_field_buffer(f, 0, "Enter-Ok    Esc-Cancel");
    set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD);
    field_opts_off(f, O_ACTIVE); // Static text
    // Finalise the list of fields
    addfield(NULL);
}


void AddAccMgrForm::eventhandle(NEvent* ev) // Event handler
{
    if ( ev->done )
	return;
    //NMouseEvent* mevent = (NMouseEvent*)ev;
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
		char* username = rtrim(field_buffer(fields[usernamefield],0));
		char* passw = rtrim(field_buffer(fields[passwfield],0));
		mgrurl = rtrim(field_buffer(fields[urlfield],0));
		char* mgrname = rtrim(field_buffer(fields[namefield],0));
		kLogPrintf("AddAccMgrForm OK username=[%s] passw=[%s]\n", username, passw);
		if (srv!=NULL)
		{
		    std::string errmsg;
		    bool success = srv->accountmanager(mgrurl.c_str(), username, passw, false, errmsg);
		    if (success)
		    {
			Item* account_manager = NULL;
			if (srv !=NULL)
			    account_manager = srv->findaccountmanager(mgrname);
			if (account_manager == NULL) // For custom managers we save in configs
			{
			    // Check if the account manager is already in the config
			    // Then we either update the existing record, otherwise add a new one
			    bool exist = false;
			    Item* boinctui_cfg = gCfg->getcfgptr();
			    if (boinctui_cfg != NULL)
			    {
				std::vector<Item*> mgrlist = boinctui_cfg->getItems("accmgr");
				std::vector<Item*>::iterator it;
				for (it = mgrlist.begin(); it != mgrlist.end(); it++)
				{
				    Item* namecfg = (*it)->findItem("name");
				    Item* urlcfg =  (*it)->findItem("url");
				    if (namecfg != NULL)
					if (strcmp(namecfg->getsvalue(),mgrname) == 0)
					{
					    exist = true;
					    // Update the URL value in the config
					    Item* urlcfg = (*it)->findItem("url");
					    if (urlcfg != NULL)
						urlcfg->setsvalue(mgrurl.c_str());
					}
				    if (urlcfg != NULL)
				    {
					if (strcmp(urlcfg->getsvalue(),mgrurl.c_str()) == 0)
					{
					    exist = true;
					    // Update the value of the name in the config
					    Item* namecfg = (*it)->findItem("name");
					    if (namecfg != NULL)
						namecfg->setsvalue(mgrname);
					}
				    }
				    if (exist)
					break;
				}
				if (!exist)
				{
				    // Write to config as new
				    Item* accmgr  = new Item("accmgr");
				    boinctui_cfg->addsubitem(accmgr);
				    Item* name  = new Item("name");
				    name->setsvalue(mgrname);
				    Item* url  = new Item("url");
				    url->setsvalue(mgrurl.c_str());
				    accmgr->addsubitem(name);
				    accmgr->addsubitem(url);
				}
			    }
			}
			putevent(new TuiEvent(evADDACCMGR)); // Create an event to close the form
		    }
		    else
		    {
			// Error message
			errmsg = " Error: " + errmsg;
			set_field_buffer(fields[errmsgfield], 0, errmsg.c_str());
			field_opts_on(fields[errmsgfield], O_VISIBLE); // Make error line visible
			this->refresh();
		    }
		}
		break;
	    }
	    case 27:
		putevent(new TuiEvent(evADDACCMGR, srv, mgrname.c_str())); // Window closing code
		break;
	    default:
		kLogPrintf("AddAccMgrForm::KEYCODE=%d\n", ev->keycode);
		ev->done = false;
		NForm::eventhandle(ev); //предок
		break;
	} //switch
    }
}
