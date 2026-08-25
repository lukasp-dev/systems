#include "minios/scheduler.hpp"

namespace minios {
RoundRobinScheduler::RoundRobinScheduler(ProcessTable& table, int quantum)
    : table_(table),
    quantum_(quantum),
    ticks_on_cpu_(0),
    running_pid_(-1),
    rr_index_(0) {}

int RoundRobinScheduler::findNextReady() {
    //TODO: Ready 인 프로세스 pid. 없으면 -1
    int found_slot = -1;
    const int pid = table_.findReadyPid(rr_index_, found_slot);
    if(pid >= 0) {
        rr_index_ = (found_slot + 1) % static_cast<int>(table_.maxSlots());
    }
    return pid;
}

int RoundRobinScheduler::schedule() {
    if (running_pid_ >= 0) {
        if (Process* p = table_.findProcess(running_pid_)) {
            if (p->state == ProcessState::Running) {
                return running_pid_;
            }
        }
    }

    const int next = findNextReady();
    if (next < 0) {
        return -1;
    }

    table_.setState(next, ProcessState::Running);
    running_pid_ = next;
    ticks_on_cpu_= 0;
    return running_pid_;
}

void RoundRobinScheduler::tick() {
    if (running_pid_ < 0) {
        schedule();
        return;
    }

    Process* p = table_.findProcess(running_pid_);
    if (p == nullptr || p->state != ProcessState::Running) {
        running_pid_ = -1;
        schedule();
        return;
    }

    ++p->cpu_time_ticks;
    ++ticks_on_cpu_;
    if(quantum_ == ticks_on_cpu_) {
        table_.setState(running_pid_, ProcessState::Ready);
        running_pid_ = -1;
        schedule();
        return;
    }
}
}
