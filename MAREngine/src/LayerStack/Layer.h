/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_LAYER_H
#define MAR_ENGINE_LAYER_H


#include "../mar.h"
#include "../Debug/Log.h"


namespace mar {
	namespace layers {

		class LayerStack;


		class Layer {
		public:
			Layer() = default;
			Layer(const char* name) : m_debugName(name) { }

			virtual void update() { }

			virtual void closeLayer() { }

		protected:
			friend class LayerStack;

			const char* m_debugName;
		};


} }


#endif // !MAR_ENGINE_LAYER_H
