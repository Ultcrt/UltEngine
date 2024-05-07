//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_OBSERVER_H
#define ULTENGINE_OBSERVER_H

namespace UltEngine {
	template <class... Args>
	class Observer {
	public:
		virtual void update(Args&&... args) = 0;
	};
} // UltEngine

#endif //ULTENGINE_OBSERVER_H