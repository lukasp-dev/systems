#pragma once

namespace minios {

// OSTEP §5: Ready, Running, Blocked — plus lifecycle states from xv6 / §14.
enum class ProcessState {
    Embryo,     // being created (xv6 EMBRYO) => example: fork 후 child process가 생성되는 순간
    Ready,      // can run, waiting for CPU (RUNNABLE) => main 에서 처음 생성된 process가 준비 상태가 되는 순간
    Running,    // on CPU now => process가 CPU를 사용하고 있는 상태
    Blocked,    // waiting for an event (SLEEPING) => process가 대기 상태가 되는 순간
    Zombie,     // exited, exit status not yet collected (ZOMBIE) => process가 종료된 상태
    Terminated  // MiniOS: slot reclaimed after wait/reap => process가 종료된 상태이지만, 아직 종료 상태가 아닌 상태
};

const char* to_string(ProcessState state);

}  // namespace minios
