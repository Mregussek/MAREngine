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


		class Layer {
		public:
			Layer(std::string name) : m_debugName(name) { }

			virtual void prepareFrame() { }

			virtual void update() { }

			virtual void endFrame() { }

		protected:
			std::string m_debugName;
		};


} }


#endif // !MAR_ENGINE_LAYER_H
