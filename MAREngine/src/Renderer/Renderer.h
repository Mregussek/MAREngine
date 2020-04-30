
#ifndef RENDERER_H
#define RENDERER_H

#include "../mar.h"

namespace mar {
	class Renderer {
		size_t _numberOfPositions;
		size_t _numberOfIndices;

	public:
		Renderer(const size_t& numberPositions, const size_t& numberIndices = -1);

		void draw() const;
		void clear() const;
	};
}

#endif // !RENDERER_H