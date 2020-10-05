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
    PrefForm(int lines, int rows, Srv* srv, std::string s);
    void genfields(int& line); // Create an array of fields
    virtual void eventhandle(NEvent* ev); // Event handler
  protected:
    int         errmsgfield;
    Srv*        srv;
    std::string type;
    std::vector<std::string> computing_preferences =
    {
        "cpu_usage_limit",                   // Use at most % of the CPUs
        "max_ncpus_pct",                     // Use at most % of CPU time
        "run_on_batteries",                  // Suspend when computer is on battery
        "run_if_user_active",                // Suspend when computer is in use
        "run_gpu_if_user_active",            // Suspend GPU computing when computer is in use
        "idle_time_to_run",                  // 'In use' means mouse/keyboard input in last minutes
        "suspend_cpu_usage",                 // Suspend when non-BOINC CPU usage is above %
        "work_buf_additional_days",          // Store up to an additional days of work
        "work_buf_min_days",                 // Store at least days of work
        "cpu_scheduling_period_minutes",     // Switch between tasks every minutes
        "disk_interval"                      // Request tasks to checkpoint at most every seconds
    };
    std::vector<std::string> networking_preferences =
    {
        "max_bytes_sec_down",                // Limit download rate to KB/second
        "max_bytes_sec_up",                  // Limit upload rate to KB/second
        "dont_verify_images",                // Skip data verification of image files
        "confirm_before_connecting",         // Confirm before connecting to Internet
        "hangup_if_dialed"                   // Disconnect when done
    };
    std::vector<std::string> disk_memory_preferences =
    {
        "disk_max_used_gb",                  // Use no more than GB
        "disk_min_free_gb",                  // Leave at least GB free
        "disk_max_used_pct",                 // Use no more than % of total
        "ram_max_used_busy_pct",             // When computer is in use, use at most %
        "ram_max_used_idle_pct",             // When computer is not in use, use at most %
        "leave_apps_in_memory",              // Leave non-GPU tasks in memory while suspended
        "vm_max_used_pct"                    // Page/swap file: use at most %
    };
    std::vector<std::string> schedule_preferences =
    {
        "daily_xfer_limit_mb",
        "daily_xfer_period_days",
        "end_hour",
        "net_end_hour",
        "net_start_hour",
        "start_hour",
    };
    std::map<std::string, int> preference_fields;
    std::map<std::string, char *> preference_settings;
};


#endif //PREFFORM_H
