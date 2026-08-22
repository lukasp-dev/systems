#pragma once

#include <string>

#include "minios/process_state.hpp"

namespace minios {

// PCB / process descriptor (OSTEP §6, xv6 struct proc — simplified for v0.1).
struct Process {
    int pid = -1;
    int parent_pid = 0;  // 0 = no parent (init)

    ProcessState state = ProcessState::Embryo;

    std::string name;

    // Placeholders for later CPU / context-switch work.
    int program_counter = 0;
    std::uint64_t cpu_time_ticks = 0;

    // Set when exit(); collected by wait() (§14 zombie).
    int exit_status = 0;
};

}  // namespace minios
