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
#include "addprojectform.h"
#include "mbstring.h"
#include "kclog.h"
#include "tuievent.h"


//стрингификатор
#define XSTR(S)		STR(S)
#define STR(S)		#S
#define ERROREX(msg)   throw __FILE__ ":" XSTR(__LINE__) "[" msg "]";


char* strupcase(char* s) // To upper case
{
    char	*p;
    for (p = s; *p != '\0'; p++)
    *p = (char) toupper(*p);
    return s;
}


char* strlowcase(char* s) // To lower case
{
    char	*p;
    for (p = s; *p != '\0'; p++)
    *p = (char) tolower(*p);
    return s;
}


AddProjectForm::AddProjectForm(int rows, int cols,  Srv* srv, const char* projname, bool userexist, bool byurl) : NForm(rows,cols)
{
    this->srv = srv;
    settitle(projname);
    this->projname = projname;
    this->userexist = userexist;
    this->byurl = byurl;
    Item* project = NULL;
    if (srv !=NULL)
	project = srv->findprojectbynamefromall(projname);
    int row = 0;
    // Margins
    try
    {
	genfields(row,project);
    }
    catch(const char* err)
    {
	kLogPrintf("ERROR EXCEPTION %s\n",err);
    }
    // Recalculate the height of the form, so that all the fields look right
    int r,c =0;
    scale_form(frm, &r, &c);
    kLogPrintf("field_count=%d scale_form()->%d,%d\n", field_count(frm), r, c);
    resize(r+3,c+2);
    post_form(frm);
    this->refresh();
}


void AddProjectForm::genfields(int& line, Item* project) // Create an array of fields
{
    FIELD* f;
    delfields();
    //сообщение об ошибке
    errmsgfield = getfieldcount();
    f = addfield(new_field(1, getwidth()-2, line++, 0, 0, 0));
    if (!f)
	ERROREX();
    if (E_OK != set_field_buffer(f, 0, "Error"))
	ERROREX();
    if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_RED) | A_BOLD))
	ERROREX();
    if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	ERROREX();
    if (E_OK != field_opts_off(f, O_VISIBLE)) // Default is invisible
	ERROREX();
    if (project != NULL)
    {
	//url
	Item* url = project->findItem("url");
	std::string s = "url          : ";
	if (url !=NULL)
	    projurl = url->getsvalue();
	s = s + projurl;
	f = addfield(new_field(1, getwidth()-4, line++, 1, 0, 0));
	if (E_OK != set_field_buffer(f, 0, s.c_str()))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	//area
	Item* general_area = project->findItem("general_area");
	s = "General area : ";
	if (general_area !=NULL)
	    s = s + general_area->getsvalue();
	f = addfield(new_field(1, getwidth()-4, line++, 1, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, s.c_str()))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	//specific area
	Item* specific_area = project->findItem("specific_area");
	s = "Specific area: ";
	if (specific_area !=NULL)
	    s = s + specific_area->getsvalue();
	f = addfield(new_field(1, getwidth()-4, line++, 1, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, s.c_str()))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	//home
	s = "Home         : ";
	Item* home = project->findItem("home");
	if (home !=NULL)
	    s = s + home->getsvalue();
	f = addfield(new_field(1, getwidth()-4, line++, 1, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, s.c_str()))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	//description
	s = "Description  : ";
	line++;
	Item* description = project->findItem("description");
	if (description !=NULL)
	    s = s + description->getsvalue();
	int h = s.size()/(getwidth() - 4) + 1;
	if (h > 4)
	    h = 4;
	f = addfield(new_field(h, getwidth()-4, line, 1, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, s.c_str()))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	line += h+1;
	//platforms
	Item* platforms = project->findItem("platforms");
	s = "Platforms    : ";
	if (platforms !=NULL)
	{
	    std::vector<Item*> namelist = platforms->getItems("name"); // List of platform names
	    std::vector<Item*>::iterator it;
	    for (it = namelist.begin(); it!=namelist.end(); it++)
	    {
		Item* name = (*it)->findItem("name");
		if (it != namelist.begin())
		    s = s + ',';
		s = s + name->getsvalue();
	    }
	}
	h = s.size()/(getwidth() - 4) + 1;
	if (h > 5)
	    h = 5;
	f = addfield(new_field(h, getwidth()-4, ++line, 1, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, s.c_str()))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	line += h + 1;
    }

    //project URL
    if (byurl)
    {
	line++;
	f = addfield(new_field(1, 10, line, 1 , 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, "ProjectURL"))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	projurlfield = getfieldcount();
	f = addfield(new_field(1, 40, line++, 15, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != field_opts_off(f, O_AUTOSKIP))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD))
	    ERROREX();
    }
    //email
    line++;
    f = addfield(new_field(1, 10, line, 1 , 0, 0));
    if (!f)
	ERROREX();
    if (E_OK != set_field_buffer(f, 0, "email"))
	ERROREX();
    if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	ERROREX();
    if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	ERROREX();
    emailfield = getfieldcount();
    f = addfield(new_field(1, 40, line++, 15, 0, 0));
    if (!f)
	ERROREX();
    if (E_OK != field_opts_off(f, O_AUTOSKIP))
	ERROREX();
    if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD))
	ERROREX();
    // Password
    line++;
    f = addfield(new_field(1, 10, line, 1 , 0, 0));
    if (!f)
	ERROREX();
    if (E_OK != set_field_buffer(f, 0, "password"))
	ERROREX();
    if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	ERROREX();
    if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	ERROREX();
    passwfield = getfieldcount();
    f = addfield(new_field(1, 40, line++, 15, 0, 0));
    if (!f)
	ERROREX();
    if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD))
	ERROREX();
    if (E_OK != field_opts_off(f, O_AUTOSKIP))
	ERROREX();
    if (!userexist)
    {
	// Username
	line++;
	f = addfield(new_field(1, 10, line, 1 , 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, "username"))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	usernamefield = getfieldcount();
	f = addfield(new_field(1, 40, line++, 15, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != field_opts_off(f, O_AUTOSKIP))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD))
	    ERROREX();
	// Team name
	line++;
	f = addfield(new_field(1, 10, line, 1 , 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_buffer(f, 0, "teamname"))
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	    ERROREX();
	teamfield = getfieldcount();
	f = addfield(new_field(1, 40, line++, 15, 0, 0));
	if (!f)
	    ERROREX();
	if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,COLOR_CYAN) | A_BOLD))
	    ERROREX();
	if (E_OK != field_opts_off(f, O_AUTOSKIP))
	    ERROREX();
    }
    // Control keys
    line++;
    f = addfield(new_field(1, getwidth()-25, line++, 20 , 0, 0));
    if (!f)
	ERROREX();
    if (E_OK != set_field_buffer(f, 0, "Enter-Ok    Esc-Cancel"))
	ERROREX();
    if (E_OK != set_field_back(f, getcolorpair(COLOR_WHITE,-1) | A_BOLD))
	ERROREX();
    if (E_OK != field_opts_off(f, O_ACTIVE)) // Static text
	ERROREX();
    // Finalise the list of fields
    addfield(NULL);
}


void AddProjectForm::eventhandle(NEvent* ev) // Event handler
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
	    case '\n': // Enter
	    {
		form_driver(frm, REQ_NEXT_FIELD); // Hack so that the current field doesn't lose value
		char* email = strlowcase(rtrim(field_buffer(fields[emailfield],0)));
		if (byurl)
			projurl = rtrim(field_buffer(fields[projurlfield],0));
		char* passw = rtrim(field_buffer(fields[passwfield],0));
		kLogPrintf("AddProjectForm OK name=[%s] url=[%s] email=[%s] passw=[%s]\n", projname.c_str(), projurl.c_str(), email, passw);
		if (srv!=NULL)
		{
		    std::string errmsg;
		    bool success = true;
		    if (!userexist) // If there's no account we'll create one
		    {
			char* username = strlowcase(rtrim(field_buffer(fields[usernamefield],0)));
			char* teamname = rtrim(field_buffer(fields[teamfield],0));
			success = srv->createaccount(projurl.c_str(),email,passw, username, teamname, errmsg);
		    }
		    if (success)
			success = srv->projectattach(projurl.c_str(), projname.c_str(), email, passw, errmsg); // Connect to project
		    if (success)
			putevent(new TuiEvent(evADDPROJECT)); // Create an event to close the form
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
		putevent(new TuiEvent(evADDPROJECT)); // Window closing code
		break;
	    default:
		kLogPrintf("AddProjectForm::KEYCODE=%d\n", ev->keycode);
		ev->done = false;
		NForm::eventhandle(ev); //предок
		break;
	} //switch
    }
}
