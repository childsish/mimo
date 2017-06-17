#ifndef MIMO_WORKFLOW_H
#define MIMO_WORKFLOW_H

#include <utility>

#include "Stream.h"


typedef unsigned int uuid;

/**
 * A convenient way to run a set of piped streams.
 */
class Pipeline {
public:

    Pipeline();

    void run();

    bool can_run() const;

    uuid add_stream(Stream &stream);

    void pipe(std::pair<uuid, std::string> from, std::pair<uuid, std::string> to);

private:

    unsigned int _current_identifier;

    std::unordered_map<uuid, Stream *> _streams;

};


#endif //MIMO_WORKFLOW_H
