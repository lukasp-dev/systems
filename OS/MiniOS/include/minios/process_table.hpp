#pragma once

#include <array>
#include <cstddef>
#include <optional>
#include <vector>

#include "minios/process.hpp"

namespace minios {

// xv6 NPROC: fixed number of PCB slots in kernel memory (process list).
inline constexpr std::size_t kMaxProcesses = 64;

// Process table = array of PCBs (OSTEP §6 proc[]).
class ProcessTable {
public:
    ProcessTable();

    // Boot: create PID 1 (init). Returns init pid (always 1 on success).
    int createInit(const std::string& name = "init");

    // fork(): allocate child PCB, copy parent's user-visible state.
    // Returns child pid to the caller (parent view). -1 on failure.
    // OSTEP: child would see return value 0 inside its own execution context.
    int fork(int parent_pid);

    // exit() + zombie (§14). Parent must wait() to reap.
    bool exitProcess(int pid, int exit_status = 0);

    // wait(): block until any child of parent exits; returns reaped child pid.
    // MiniOS v0.1: synchronous, no real blocking scheduler yet.
    int wait(int parent_pid);

    void setState(int pid, ProcessState state);

    std::optional<Process> getProcess(int pid) const;
    Process* findProcess(int pid);

    // Snapshot for `ps` command (later: shell).
    std::vector<Process> listProcesses() const;

    std::size_t usedSlots() const;
    std::size_t maxSlots() const { return kMaxProcesses; }

private:
    int allocatePid();
    int findUnusedSlotIndex() const;
    bool insertProcess(int slot_index, Process process);
    void reapSlot(int slot_index);

    int next_pid_;
    std::array<std::optional<Process>, kMaxProcesses> slots_;
};

}  // namespace minios
