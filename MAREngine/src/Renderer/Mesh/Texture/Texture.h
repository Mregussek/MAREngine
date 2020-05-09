/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../mar.h"

namespace mar {
	class Texture : std::enable_shared_from_this<Texture> {
	public:
		virtual void shutdown() { }

		virtual void loadTexture(const std::string& path) { }

		virtual void bind(const float& shapeId, const unsigned int& texID) const { }
		virtual void unbind() const { }

		virtual const unsigned int& getID(int index) const { return unsigned int(0); }
		virtual void removeID(const unsigned int& index) { }

		virtual const int& getWidth() const { return int(-1); }
		virtual const int& getHeight() const { return int(-1); }
	};
}

#endif // !TEXTURE_H
