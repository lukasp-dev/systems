#include <iostream>

#include "minios/process_state.hpp"
#include "minios/process_table.hpp"
#include "minios/scheduler.hpp"

namespace {

void printPs(const minios::ProcessTable& table) {
    std::cout << "PID\tPPID\tSTATE\tCPU\tNAME\n";
    for (const minios::Process& p : table.listProcesses()) {
        std::cout << p.pid << '\t' << p.parent_pid << '\t' << minios::to_string(p.state)
                  << '\t' << p.cpu_time_ticks << '\t' << p.name << '\n';
    }
}

}  // namespace

int main() {
    minios::ProcessTable table;

    const int init_pid = table.createInit("init");
    std::cout << "=== createInit() -> pid " << init_pid << " ===\n";
    printPs(table);

    const int child_pid = table.fork(init_pid);
    std::cout << "\n=== fork(" << init_pid << ") -> child pid " << child_pid << " ===\n";
    printPs(table);

    minios::RoundRobinScheduler sched(table, /*quantum=*/2);

    std::cout << "\n=== RR scheduler: 6 ticks (quantum=2) ===\n";
    for (int t = 0; t < 6; ++t) {
        sched.tick();
        std::cout << "tick " << t << "  running=" << sched.runningPid() << '\n';
        printPs(table);
        std::cout << '\n';
    }

    table.exitProcess(child_pid, 42);
    std::cout << "=== child exit(42) -> ZOMBIE until wait ===\n";
    printPs(table);

    const int reaped = table.wait(init_pid);
    std::cout << "\n=== wait(" << init_pid << ") reaped pid " << reaped << " ===\n";
    printPs(table);

    std::cout << "\nSlots used: " << table.usedSlots() << " / " << table.maxSlots() << '\n';
    return 0;
}
