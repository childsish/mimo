/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBOUTPUTSFACTORY_H
#define MIMO_MOCKJOBOUTPUTSFACTORY_H

#include <gmock/gmock.h>
#include "IFactory.h"
#include "JobOutputFactory.h"


namespace mimo {

    class JobOutputs;

    class MockJobOutputsFactory : public IJobOutputsFactory {
    public:
        IJobOutputs *make_raw(const workflow::OutputMap &outputs) const override {
            return this->make_raw_proxy(outputs);
        }

        std::shared_ptr<IJobOutputs> make_shared(const workflow::OutputMap &outputs) const override {
            return std::shared_ptr<IJobOutputs>(this->make_shared_proxy(outputs));
        }

        std::unique_ptr<IJobOutputs> make_unique(const workflow::OutputMap &outputs) const override {
            return std::unique_ptr<IJobOutputs>(this->make_unique_proxy(outputs));
        }

        MOCK_CONST_METHOD1(make_raw_proxy, IJobOutputs*(const workflow::OutputMap &));
        MOCK_CONST_METHOD1(make_shared_proxy, IJobOutputs*(const workflow::OutputMap &));
        MOCK_CONST_METHOD1(make_unique_proxy, IJobOutputs*(const workflow::OutputMap &));
    };
}

#endif //MIMO_MOCKJOBOUTPUTSFACTORY_H
