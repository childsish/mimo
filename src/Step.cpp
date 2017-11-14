#include "Stream.h"

#include <sstream>

Stream::Stream() : name("stream") {}

Stream::Stream(const std::string &name_, const std::vector<std::string> &ins_, const std::vector<std::string> &outs_) :
    name(name_), ins(ins_), outs(outs_) {
    _init_queues();
}

Stream::~Stream() {
    for (auto key : ins) {
        while (inputs.at(key).can_pop()) {
            collect(inputs.at(key).pop());
        }
    }
}

Stream &Stream::pipe(Stream &stream, const std::string &output, const std::string &input) {
    if (outputs.size() == 0) {
        std::stringstream buffer;
        buffer << name << " has no output to pipe from.";
        throw std::runtime_error(buffer.str());
    } else if (stream.inputs.size() == 0) {
        std::stringstream buffer;
        buffer << stream.name << " has no input to pipe to.";
        throw std::runtime_error(buffer.str());
    }

    // Get the output queue
    QueuePusher *output_;
    if (output == "" && outputs.size() == 1) {
        output_ = &outputs.begin()->second;
    }
    else if (outputs.find(output) == outputs.end()) {
        std::stringstream buffer;
        buffer << "Stream " << name << " has no output named " << output;
        throw std::runtime_error(buffer.str());
    }
    else {
        output_ = &outputs[output];
    }

    // Get the input queue
    Queue *input_;
    if (input == "" && stream.inputs.size() == 1) {
        input_ = &stream.inputs.begin()->second;
    }
    else if (stream.inputs.find(input) == stream.inputs.end()) {
        std::stringstream buffer;
        buffer << "Stream " << stream.name << " has no input named " << input;
        throw std::runtime_error(buffer.str());
    }
    else {
        input_ = &stream.inputs[input];
    }

    output_->add_queue(input_);
    return stream;
}

bool Stream::can_run() const {
    bool can_pop = true;
    for (auto input : ins) {
        can_pop = can_pop && inputs.at(input).can_pop();
    }
    bool can_push = true;
    for (auto output : outs) {
        can_push = can_push && outputs.at(output).can_push();
    }
    return can_pop && can_push;
}

void Stream::push(Entity *entity, const std::string &input) {
    if (input == "" && inputs.size() == 1) {
        inputs.begin()->second.push(entity);
    }
    else if (inputs.find(input) == inputs.end()) {
        std::stringstream buffer;
        buffer << "Stream " << name << " has no input named " << input;
        throw std::runtime_error(buffer.str());
    }
    else {
        inputs[input].push(entity);
    }
}

bool Stream::collect(Entity *entity) {
    entity->reference_count -= 1;
    if (entity->reference_count == 0) {
        delete entity;
        return true;
    }
    return false;
}

void Stream::_init_queues() {
    for (auto in : ins) {
        inputs.emplace(in, Queue{});
    }
    for (auto out : outs) {
        outputs.emplace(out, QueuePusher{});
    }
}
