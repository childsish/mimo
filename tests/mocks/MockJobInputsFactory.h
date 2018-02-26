/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_MOCKJOBINPUTSFACTORY_H
#define MIMO_MOCKJOBINPUTSFACTORY_H

#include <gmock/gmock.h>
#include <workflow/Step.h>
#include "IFactory.h"

namespace mimo {

    class JobInputs;

    class MockJobInputsFactory : public IJobInputsFactory {
    public:
        IJobInputs *make_raw(const workflow::InputMap &inputs) const override {
            return this->make_raw_proxy(inputs);
        }

        std::shared_ptr<IJobInputs> make_shared(const workflow::InputMap &inputs) const override {
            return std::shared_ptr<IJobInputs>(this->make_shared_proxy(inputs));
        }

        std::unique_ptr<IJobInputs> make_unique(const workflow::InputMap &inputs) const override {
            return std::unique_ptr<IJobInputs>(this->make_unique_proxy(inputs));
        }

        MOCK_CONST_METHOD1(make_raw_proxy, IJobInputs*(const workflow::InputMap &));
        MOCK_CONST_METHOD1(make_shared_proxy, IJobInputs*(const workflow::InputMap &));
        MOCK_CONST_METHOD1(make_unique_proxy, IJobInputs*(const workflow::InputMap &));
    };
}

#endif //MIMO_MOCKJOBINPUTSFACTORY_H
