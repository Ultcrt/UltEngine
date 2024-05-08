//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_OBSERVER_H
#define ULTENGINE_OBSERVER_H

// Make sure GLFW macros do not conflict with GLAD ones
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace UltEngine {
	template <class... Args>
	class Observer {
	public:
		virtual void update(Args... args) = 0;
	};

    typedef Observer<GLFWwindow*> InputObserver;
} // UltEngine

#endif //ULTENGINE_OBSERVER_H