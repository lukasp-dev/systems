#include "minios/process_state.hpp"

namespace minios {

const char* to_string(ProcessState state) {
    switch (state) {
        case ProcessState::Embryo:
            return "EMBRYO";
        case ProcessState::Ready:
            return "READY";
        case ProcessState::Running:
            return "RUNNING";
        case ProcessState::Blocked:
            return "BLOCKED";
        case ProcessState::Zombie:
            return "ZOMBIE";
        case ProcessState::Terminated:
            return "TERMINATED";
    }
    return "UNKNOWN";
}

}  // namespace minios
