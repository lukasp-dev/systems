#pragma once

#include "minios/process_table.hpp"

namespace minios {

class RoundRobinScheduler {
public:
    explicit RoundRobinScheduler(ProcessTable& table, int quantum = 2);

    int schedule();
    void tick();

    int runningPid() const {return running_pid_;}

private:
    ProcessTable& table_;
    int quantum_;
    int ticks_on_cpu_;
    int running_pid_;

    int findNextReady();
    int rr_index_;
};

} // namespace minios