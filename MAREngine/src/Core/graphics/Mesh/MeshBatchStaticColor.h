/**
 *           MAREngine - open source 3D game engine
 * Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
**/


#ifndef MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H
#define MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H


#include "MeshDefinitions.h"
#include "MeshBatchStatic.h"


namespace marengine {

	class Entity;
	struct ColorComponent;


	class FMeshBatchStaticColor : public FMeshBatchStatic {

		friend class RenderEvents;

	public:

		virtual void reset() override;
		virtual bool canBeBatched(const Entity& entity) const override;
		virtual void submitToBatch(const Entity& entity) override;

		const FColorsArray& getColors() const;

		uint32_t getUniqueColorsID() const;
		void setUniqueColorsID(uint32_t id);

	private:

		void submitColor(const ColorComponent& colorComponent);


		FColorsArray m_colors;
		uint32_t m_uniqueColorsID{ 0 };
		static const EMeshBatchStaticType s_meshBatchType{ EMeshBatchStaticType::COLOR };

	};



}


#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H
