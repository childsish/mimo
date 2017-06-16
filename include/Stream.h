#ifndef MIMO_STREAM_H
#define MIMO_STREAM_H

#include <string>
#include <unordered_map>
#include "QueuePusher.h"


class Stream {
public:

    const std::string name;
    const std::vector<std::string> ins;
    const std::vector<std::string> outs;

    /**
     * Create a new stream using the given parameters. If not provided, the equivalent static variables are used.
     * @param ins_ input entities
     * @param outs_ output entities
     */
    Stream();
    Stream(const std::string &name, const std::vector<std::string> &ins, const std::vector<std::string> &outs);
    virtual ~Stream();

    /**
     * Run the stream.
     * This function must be overloaded to provide the functionality of the stream. Input can be popped from the member
     * variable map `inputs` and output can be popped from the member variable map `outputs`.
     */
    virtual void run() = 0;

    /**
     * Check if the all inputs have pending entities and all outputs can be pushed to.
     * @return true if stream can be run
     */
    virtual bool can_run() const;

    /**
     * Connect stream to another stream
     * @param stream stream to connect to
     * @param output connect from given output entity of this stream
     * @param input connect to given output entity of that stream
     * @return
     */
    Stream &pipe(Stream &stream, const std::string &output = "", const std::string &input = "");

    /**
     * Preload the stream with input
     * @param entity entity to push to stream
     * @param input name of the queue to push to
     */
    void push(Entity *entity, const std::string &input = "");

protected:

    std::unordered_map<std::string, Queue> inputs;

    std::unordered_map<std::string, QueuePusher> outputs;

    /**
     * Garbage collect an entity if it is not being referenced anymore.
     * @param entity entity to delete
     * @return true if entity was deleted
     */
    bool collect(Entity *entity);

private:

    virtual void _init_queues();

};


#endif //MIMO_STREAM_H
