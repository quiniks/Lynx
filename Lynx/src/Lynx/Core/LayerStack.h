#pragma once
#include "Lynx/Core/Layer.h"
#include <memory>

namespace Lynx {
	class LayerStack {
	public:
		LayerStack() = default;
		~LayerStack();

		/*
		void PushLayerU(std::unique_ptr<Layer> layer) {
			layer->OnAttach();
			m_LayersU.emplace(m_LayersU.begin() + m_LayerInsertIndex, std::move(layer));
			m_LayerInsertIndex++;
		}
		std::vector<std::unique_ptr<Layer>> m_LayersU;
		*/

		void PushLayer(std::shared_ptr<Layer> layer);
		void PushOverlay(std::shared_ptr<Layer> overlay);
		void PopLayer(std::shared_ptr<Layer> layer);
		void PopOverlay(std::shared_ptr<Layer> overlay);

		std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return m_Layers.rend(); }
	private:
		std::vector<std::shared_ptr<Layer>> m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}

