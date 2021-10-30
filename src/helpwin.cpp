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

#include <string.h>
#include "helpwin.h"
#include "tuievent.h"


HelpWin::HelpWin(int rows, int cols) : NGroup(NRect(rows, cols, getmaxy(stdscr)/2-rows/2,getmaxx(stdscr)/2-cols/2))
{
    caption = strdup(" Hot keys list ");
    modalflag = true;
    resize(18,60);
    wattrset(win,getcolorpair(COLOR_WHITE, -1) | A_BOLD);
    if(asciilinedraw == 1)
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    else
	box(win,0,0);
    mvwprintw(win,0,getwidth()/2-(strlen(caption)/2),caption);
    text1 = new NStaticText(NRect(getheight()-2,getwidth()-2,/*rect.begrow+*/1,/*rect.begcol+*/1));
    int attr1 = getcolorpair(COLOR_YELLOW, -1) | A_BOLD;
    int attr2 = getcolorpair(COLOR_WHITE, -1) | A_BOLD;
    text1->setstring(attr1,   "\n   Common Controls:\n");
    text1->appendstring(attr2,"       \"N\"           - View next BOINC host\n");
    text1->appendstring(attr2,"       \"P\"           - View previous BOINC host\n");
    text1->appendstring(attr2,"       \"C\"           - Edit configuration\n");
    text1->appendstring(attr2,"       \"Q\"           - Quit boinctui\n");
    text1->appendstring(attr2,"       \"F9\"/\"M\"      - Toggle main menu\n");
    text1->appendstring(attr2,"       \"PgUp\"/\"PgDn\" - Scroll Messages Window\n");
    text1->appendstring(attr2,"\n");
    text1->appendstring(attr1,"   Task Controls:\n");
    text1->appendstring(attr2,"       \"Up\"/\"Dn\"     - Select task\n");
    text1->appendstring(attr2,"       \"S\"           - Suspend selected running task\n");
    text1->appendstring(attr2,"       \"R\"           - Resume selected suspended task\n");
    text1->appendstring(attr2,"       \"A\"           - Abort selected task\n");
    text1->appendstring(attr2,"       \"Enter\"       - View selected task's raw info\n");
    insert(text1);
}


void HelpWin::eventhandle(NEvent* ev) // Event handler
{
    NGroup::eventhandle(ev); //предок
    if ( ev->done )
	return;
    // Close window on any click regardless of coordinates
    NMouseEvent* mevent = (NMouseEvent*)ev;
    if (( ev->type == NEvent::evMOUSE ) && (mevent->cmdcode & (BUTTON1_CLICKED | BUTTON1_DOUBLE_CLICKED)))
	putevent(new TuiEvent(evKEYBIND));

    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
	switch(ev->cmdcode)
	{
	    case 27:
	    case KEY_ENTER:
	    case ' ':
	    case '\n':
		putevent(new TuiEvent(evKEYBIND)); //NEvent(NEvent::evPROG, 4)); // Create event with code 4 (help window)
		break;
	} //switch
	if (ev->done) // Redraw the screen if processing is finished
	    refresh();
    }
}
