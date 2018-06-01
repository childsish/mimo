/** @author: Liam Childs (liam.h.childs@gmail.com) */

#ifndef MIMO_ANYITERATOR_H
#define MIMO_ANYITERATOR_H

#include <utility>


namespace mimo {

    template<typename Type>
    class IForwardIterator {
    public:
        IForwardIterator() = default;

        IForwardIterator(const IForwardIterator<Type> &that) :
            body(that.body ? that.body->clone () : 0) {}

        template<typename Iterator>
        IForwardIterator(const Iterator &that) : body(new adaptor<Iterator>(that)) {}

        IForwardIterator<Type> &operator=(const IForwardIterator<Type> &that) {
            this->swap(IForwardIterator<Type>(that));
            return *this;
        }

        bool operator==(const IForwardIterator<Type> &that) {
            return this->body->equal(that.body);
        }

        bool operator!=(const IForwardIterator<Type> &that) {
            return !(*this == that);
        }

        const Type &operator*() const {
            return body->current();
        }

        Type *operator->() const {
            return &body->current();
        }

        const IForwardIterator<Type> operator++() {
            this->body->next();
            return *this;
        }

        const IForwardIterator<Type> operator++(int) {
            IForwardIterator<Type> result(*this);
        }

    private:
        class wrapper {
        public:
            virtual void next() = 0;
            virtual const Type &current() const = 0;
            virtual bool equal(const wrapper *rhs) const = 0;
            virtual void assign(const wrapper *rhs) = 0;
            virtual wrapper *clone() const = 0;
        };

        template<typename Iterator>
        class adaptor : public wrapper {
        public:
            explicit adaptor(const Iterator &that) : adaptee(that) {}
            void next() override {
                ++this->adaptee;
            }
            const Type &current() const override {
                return *this->adaptee;
            }
            bool equal(const wrapper *that) const override {
                auto tmp = dynamic_cast<const adaptor<Iterator> *>(that);
                return tmp && this->adaptee == tmp->adaptee;
            }
            void assign(const wrapper *that) override {
                this->adaptee = static_cast<const adaptor<Iterator> *>(that)->adaptee;
            }
            wrapper *clone() const override {
                return new adaptor<Iterator>(this->adaptee);
            }
        private:
            Iterator adaptee;
        };

        IForwardIterator<Type> &swap(IForwardIterator<Type> &that) {
            std::swap<wrapper>(*this->body, *that.body);
            return *this;
        }

        wrapper *body;
    };
}
#endif //MIMO_ANYITERATOR_H
