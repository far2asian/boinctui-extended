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

#ifndef TUIEVENT_H
#define TUIEVENT_H

#include "srvdata.h"
#include "nevent.h"


enum TuiEventType
{
    evCFGCH,		// Configuration changed
    evABORTRES,		// Abort selected result
    evABOUT,		// Open About window
    evKEYBIND,		// Open Key Bindings window
    evBENCHMARK,	// Run benchmark
    evADDPROJECT,	// Add project
    evCOLVIEWCH,	// Switch the column visibility
    evVIEWMODECH,	// Switch visibility mode - All/Hide done/Active tasks only
    evSORTMODECH,	// Switch view mode - 0-unsorted 1-state e.t.c.
    evADDACCMGR,	// Add account manager form
    evPROJECTOP,	// Project operations
    evTASKINFO,		//генерируется когда нужно открыть окно детальной информации о задаче
    evTASKSELECTORON,	// Triggers when the task becomes visible
    evTASKSELECTOROFF,	// Triggers when the task selector becomes hidden
    evASCIIMODECHANGE,	// Triggers when ASCII line mode is changed
    evPOPUPMSG          // Triggers when a popup window is opened
};


class TuiEvent : public NEvent // boinctui-specific programming events class
{
  public:
    TuiEvent(TuiEventType type) : NEvent(evPROG, type)	{};
    TuiEvent(TuiEventType type, Srv* srv, const char* prjname, bool userexist) : NEvent(evPROG, type) // Add project event
    {
	this->srv = srv;
	this->sdata1 = prjname;
	this->bdata1 = userexist;
    };
    TuiEvent(TuiEventType type, Srv* srv, const char* mgrname) : NEvent(evPROG, type) // Connect to account manager
    {
	this->srv = srv;
	this->sdata1 = mgrname;
    };
    TuiEvent(TuiEventType type, Srv* srv, const char* projname, const char* projop) : NEvent(evPROG, type) //событие для действий с проектами
    {
	this->srv = srv;
	this->sdata1 = projname;
	this->sdata2 = projop;
	this->bdata1 = false; //true - if a confirmation is received
    };

    TuiEvent(TuiEventType type ,int ncolumn, bool enable) : NEvent(evPROG, type) //событие переключения видимости колонки
    {
	this->idata1 = ncolumn;
	this->bdata1 = enable;
    };
    TuiEvent(TuiEventType type ,int mode) : NEvent(evPROG, type) //событие режим видимости задач
    {
	this->idata1 = mode;
    };
    TuiEvent(TuiEventType type, const char* caption, const char* msg) : NEvent(evPROG, type) // Popup message event
    {
        this->sdata1 = caption;
        this->sdata2 = msg;
    };
    virtual ~TuiEvent() { /*kLogPrintf("~TuiEvent()\n");*/ };
    Srv*		srv;
    std::string		sdata1; //произвольная строка
    std::string		sdata2; //произвольная строка
    bool		bdata1; //произаольная bool переменная
    int			idata1; //произвольное целое
};


#endif //TUIEVENT_H
