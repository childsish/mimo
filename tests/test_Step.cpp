#include "gtest/gtest.h"

#include "Step.h"
#include "entities/Integer.h"
#include "streams/Capture.h"
#include "streams/Release.h"
#include "streams/Sink.h"


class DoNothing : public Step {
public:

    DoNothing() : Step("do_nothing", {"input"}, {"output"}) {}

    void run() {
        Entity *entity;
        while (can_run()) {
            entity = inputs["input"].pop();
            outputs["output"].push(entity);
        }
    }
};


TEST(StreamTest, test_inherit) {
    Step *stream = new DoNothing();

    EXPECT_EQ(stream->name, "do_nothing");

    delete stream;
}

TEST(StreamTest, test_run) {
    Step *stream = new Sink();

    ASSERT_FALSE(stream->can_run());
    stream->push(new Entity(), "input");
    ASSERT_TRUE(stream->can_run());
    stream->run();
    ASSERT_FALSE(stream->can_run());

    delete stream;
}

TEST(StreamTest, test_pipe) {
    Step *release = new Release({
        new Integer(0),
        new Integer(1),
        new Integer(2),
        new Integer(3),
        new Integer(4)
    });
    Step *capture = new Capture();

    release->pipe(*capture);

    while (release->can_run() || capture->can_run()) {
        if (release->can_run()) {
            release->run();
        }
        else if (capture->can_run()) {
            capture->run();
        }
    }

    EXPECT_EQ(static_cast<Integer *>(static_cast<Capture *>(capture)->captured[0])->value, 0);
    EXPECT_EQ(static_cast<Integer *>(static_cast<Capture *>(capture)->captured[1])->value, 1);
    EXPECT_EQ(static_cast<Integer *>(static_cast<Capture *>(capture)->captured[2])->value, 2);
    EXPECT_EQ(static_cast<Integer *>(static_cast<Capture *>(capture)->captured[3])->value, 3);
    EXPECT_EQ(static_cast<Integer *>(static_cast<Capture *>(capture)->captured[4])->value, 4);

    delete release;
    delete capture;
}
