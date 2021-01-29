#pragma once

#include "Lynx/Core/Core.h"
#include "Lynx/Events/Event.h"
#include "Lynx/Core/TimeStep.h"

namespace Lynx {
	class Layer {
	public:
		Layer(const std::string& name = "Layer") : m_DebugName(name) {}
		virtual ~Layer() {};
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(TimeStep timeStep) {}
		virtual void OnEvent(Event& event) {}
		const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}