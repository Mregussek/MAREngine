/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_STACK_H
#define MAR_ENGINE_LAYER_STACK_H


#include "../mar.h"
#include "Layer.h"
#include "../Debug/Log.h"


namespace mar {
	namespace layers {


		class LayerStack {
			std::vector<Layer*> m_layers;

		public:
			void pushLayer(Layer* layer);
			void pushOverlayer(Layer* layer);
			void flush();
		};


} }


#endif // !MAR_ENGINE_LAYER_STACK_H

