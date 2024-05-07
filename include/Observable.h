//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_OBSERVABLE_H
#define ULTENGINE_OBSERVABLE_H

#include <vector>

namespace UltEngine {
    template <class Observer>
    class Observable {
    private:
        std::vector<Observer> observers_;

    public:
        std::size_t add(Observer observer);
        void remove(std::size_t idx);

        template<class... Args>
        void notifyAll(Args&&... args);
    };

    template<class Observer>
    template<class... Args>
    void Observable<Observer>::notifyAll(Args &&... args) {
        for (auto&& observer: observers_) {
            observer(args...);
        }
    }

    template<class Observer>
    void Observable<Observer>::remove(std::size_t idx) {
        observers_.erase(observers_.begin() + idx);
    }

    template<class Observer>
    std::size_t Observable<Observer>::add(Observer observer) {
        observers_.emplace_back(observer);
        return observers_.size();
    }
} // UltEngine

#endif //ULTENGINE_OBSERVABLE_H
