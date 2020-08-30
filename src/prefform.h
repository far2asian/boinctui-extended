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
#include <map>
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
    int         errmsgfield;
    Srv*        srv;
    std::vector<std::string> preferences =
    {
        "cpu_usage_limit",
        "max_ncpus_pct",
//        "confirm_before_connecting",
//        "cpu_scheduling_period_minutes",
//        "daily_xfer_limit_mb",
//        "daily_xfer_period_days",
//        "disk_interval",
//        "disk_max_used_gb",
//        "disk_max_used_pct",
//        "disk_min_free_gb",
//        "dont_verify_images",
//        "end_hour",
//        "hangup_if_dialed",
//        "idle_time_to_run",
//        "leave_apps_in_memory",
//        "max_bytes_sec_down",
//        "max_bytes_sec_up",
//        "net_end_hour",
//        "net_start_hour",
//        "ram_max_used_busy_pct",
//        "ram_max_used_idle_pct",
//        "run_gpu_if_user_active",
//        "run_if_user_active",
//        "run_on_batteries",
//        "start_hour",
//        "suspend_cpu_usage",
//        "vm_max_used_pct",
//        "work_buf_additional_days",
        "work_buf_min_days"
    };
    std::map<std::string, int> preference_fields;
    std::map<std::string, char *> preference_settings;
};


#endif //PREFFORM_H
