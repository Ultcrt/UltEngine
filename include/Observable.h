//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_OBSERVABLE_H
#define ULTENGINE_OBSERVABLE_H

#include <vector>
#include <memory>
#include "Observer.h"

namespace UltEngine {
    template<class... Args>
    class Observable {
    private:
        std::vector<std::shared_ptr<Observer<Args...>>> pObservers_;

    public:
        std::size_t add(const std::shared_ptr<Observer<Args...>>& pObserver);
        void remove(std::size_t idx);

        void notifyAll(Args... args);
    };

    template<class ...Args>
    inline std::size_t Observable<Args...>::add(const std::shared_ptr<Observer<Args...>>& pObserver) {
        pObservers_.emplace_back(pObserver);
        return pObservers_.size() - 1;
    }

    template<class ...Args>
    inline void Observable<Args...>::remove(std::size_t idx) {
        pObservers_.erase(pObservers_.begin() + idx);
    }

    template<class ...Args>
    inline void Observable<Args...>::notifyAll(Args... args) {
        for (const auto& pObserver : pObservers_) {
            pObserver->update(std::forward<Args>(args)...);
        }
    }
} // UltEngine

#endif //ULTENGINE_OBSERVABLE_H
