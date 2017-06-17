#include "Pipeline.h"

Pipeline::Pipeline() : _current_identifier(0) {}

void Pipeline::run() {
    while (can_run()) {
        for (unsigned int i = 0; i < _current_identifier; ++i) {
            if (_streams.at(i)->can_run()) {
                _streams.at(i)->run();
            }
        }
    }
}

bool Pipeline::can_run() const {
    bool can_run = false;
    for (unsigned int i = 0; i < _current_identifier; ++i) {
        can_run = can_run || _streams.at(i)->can_run();
    }
    return can_run;
}

uuid Pipeline::add_stream(Stream &stream) {
    uuid identifier = _current_identifier;
    _current_identifier += 1;
    _streams[identifier] = &stream;
    return identifier;
}

void Pipeline::pipe(std::pair<uuid, std::string> from, std::pair<uuid, std::string> to) {
    _streams.at(from.first)->pipe(*_streams.at(to.first), from.second, to.second);
}
