#include <iostream>

#include "minios/process_state.hpp"
#include "minios/process_table.hpp"

namespace {

void printPs(const minios::ProcessTable& table) {
    std::cout << "PID\tPPID\tSTATE\tNAME\n";
    for (const minios::Process& p : table.listProcesses()) {
        std::cout << p.pid << '\t' << p.parent_pid << '\t' << minios::to_string(p.state)
                  << '\t' << p.name << '\n';
    }
}

}  // namespace

int main() {
    minios::ProcessTable table;

    // Boot: PID 1 (like xv6 init).
    const int init_pid = table.createInit("init");
    std::cout << "=== createInit() -> pid " << init_pid << " ===\n";
    printPs(table);

    // fork() — OSTEP §8: one call from parent view, new child PCB.
    const int child_pid = table.fork(init_pid);
    std::cout << "\n=== fork(" << init_pid << ") -> child pid " << child_pid
              << " (parent view) ===\n";
    std::cout << "Note: real fork() returns 0 in the child; we model parent API only for now.\n";
    printPs(table);

    // State transitions (scheduler comes in v0.2).
    table.setState(init_pid, minios::ProcessState::Running);
    table.setState(child_pid, minios::ProcessState::Ready);
    std::cout << "\n=== scheduler picks init as RUNNING ===\n";
    printPs(table);

    // exit + wait — OSTEP §9 / §14 zombie + reap.
    table.exitProcess(child_pid, 42);
    std::cout << "\n=== child exit(42) -> ZOMBIE until wait ===\n";
    printPs(table);

    const int reaped = table.wait(init_pid);
    std::cout << "\n=== wait(" << init_pid << ") reaped pid " << reaped << " ===\n";
    printPs(table);

    std::cout << "\nSlots used: " << table.usedSlots() << " / " << table.maxSlots() << '\n';
    return 0;
}
