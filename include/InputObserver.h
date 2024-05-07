//
// Created by ultcrt on 24-5-7.
//

#ifndef ULTENGINE_INPUTOBSERVER_H
#define ULTENGINE_INPUTOBSERVER_H

#include "GLFW/glfw3.h"
#include "Observer.h"

namespace UltEngine {
	class InputObserver : public Observer<GLFWwindow*> {
	public:
		virtual void update(GLFWwindow* pWindow);
	};
} // UltEngine

#endif //ULTENGINE_INPUTOBSERVER_H