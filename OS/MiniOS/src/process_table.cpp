#include "minios/process_table.hpp"

#include <algorithm>

namespace minios {

ProcessTable::ProcessTable() : next_pid_(1) {}

int ProcessTable::createInit(const std::string& name) {
    const int slotIndex = findUnusedSlotIndex();
    if (slotIndex < 0) {
        return -1;
    }

    Process init;
    init.pid = allocatePid();  // first pid == 1
    init.parent_pid = 0;
    init.name = name;
    init.state = ProcessState::Ready;

    // std::move(init) is not required, but without it init would be copied.
    // Use std::move when init is no longer needed and ownership can be transferred.
    if (!insertProcess(slotIndex, std::move(init))) {
        return -1;
    }
    return 1;
}

int ProcessTable::fork(int parent_pid) {
    Process* parent = findProcess(parent_pid);
    if (parent == nullptr) {
        return -1;
    }

    const int slotIndex = findUnusedSlotIndex();
    if (slotIndex < 0) {
        return -1;
    }

    Process child = *parent;
    child.pid = allocatePid();
    child.parent_pid = parent_pid;
    child.name = parent->name + "_child";
    child.state = ProcessState::Ready;
    child.cpu_time_ticks = 0;
    child.exit_status = 0;

    if (!insertProcess(slotIndex, std::move(child))) {
        return -1;
    }
    return slots_[slotIndex]->pid;
}

bool ProcessTable::exitProcess(int pid, int exit_status) {
    Process* process = findProcess(pid);
    if (process == nullptr) {
        return false;
    }
    if (process->state == ProcessState::Zombie) {
        return false;
    }

    process->exit_status = exit_status;
    process->state = ProcessState::Zombie;
    return true;
}

int ProcessTable::wait(int parent_pid) {
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        if (!slots_[i].has_value()) {
            continue;
        }
        Process& child = *slots_[i];
        if (child.parent_pid == parent_pid && child.state == ProcessState::Zombie) {
            const int reaped_pid = child.pid;
            reapSlot(static_cast<int>(i));
            return reaped_pid;
        }
    }
    return -1;
}

void ProcessTable::setState(int pid, ProcessState state) {
    if (Process* process = findProcess(pid)) {
        process->state = state;
    }
}

std::optional<Process> ProcessTable::getProcess(int pid) const {
    for (const auto& slot : slots_) {
        if (slot.has_value() && slot->pid == pid) {
            return slot;
        }
    }
    return std::nullopt;
}

Process* ProcessTable::findProcess(int pid) {
    for (auto& slot : slots_) {
        if (slot.has_value() && slot->pid == pid) {
            return &(*slot);
        }
    }
    return nullptr;
}

int ProcessTable::findReadyPid(int start_slot, int& found_slot) const {
    const int n = static_cast<int>(slots_.size());
    if (n <= 0) {
        return -1;
    }
    if(start_slot < 0) {
        start_slot = 0;
    }
    start_slot %= n;

    for (int offset = 0; offset < n; ++offset) {
        const int i = (start_slot + offset) % n;
        const auto& slot = slots_[static_cast<std::size_t>(i)];
        if (slot.has_value() && slot->state == ProcessState::Ready) {
            found_slot = i;
            return slot->pid;
        }
    }
    return -1;
}

std::vector<Process> ProcessTable::listProcesses() const {
    std::vector<Process> out;
    for (const auto& slot : slots_) {
        if (slot.has_value()) {
            out.push_back(*slot);
        }
    }
    std::sort(out.begin(), out.end(),
              [](const Process& a, const Process& b) { return a.pid < b.pid; });
    return out;
}

std::size_t ProcessTable::usedSlots() const {
    return static_cast<std::size_t>(
        std::count_if(slots_.begin(), slots_.end(),
                      [](const std::optional<Process>& slot) { return slot.has_value(); }));
}

int ProcessTable::allocatePid() { return next_pid_++; }

int ProcessTable::findUnusedSlotIndex() const {
    for (std::size_t i = 0; i < slots_.size(); ++i) {
        if (!slots_[i].has_value()) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool ProcessTable::insertProcess(int slot_index, Process process) {
    if (slot_index < 0 || static_cast<std::size_t>(slot_index) >= slots_.size()) {
        return false;
    }
    if (slots_[slot_index].has_value()) {
        return false;
    }
    slots_[slot_index] = std::move(process);
    return true;
}

void ProcessTable::reapSlot(int slot_index) {
    if (slot_index >= 0 && static_cast<std::size_t>(slot_index) < slots_.size()) {
        slots_[slot_index].reset();
    }
}

}  // namespace minios
