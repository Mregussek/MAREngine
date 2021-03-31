/***********************************************************************
* @internal @copyright
*
*  				MAREngine - open source 3D game engine
*
* Copyright (C) 2020-present Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
************************************************************************/


#ifndef MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H
#define MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H


#include "MeshDefinitions.h"
#include "MeshBatchStatic.h"


namespace marengine {

	class Entity;
	struct ColorComponent;
	class FMeshBatchStaticColor;


	typedef std::vector<FMeshBatchStaticColor> FMeshBatchStaticColorArray;


	class FMeshBatchStaticColor : public FMeshBatchStatic {

		friend class FEventsMeshBatchStatic;

	public:

		void reset() override;
        MAR_NO_DISCARD bool canBeBatched(const Entity& entity) const override;
		void submitToBatch(const Entity& entity) override;

        MAR_NO_DISCARD const FColorsArray& getColors() const;

        MAR_NO_DISCARD uint32_t getUniqueColorsID() const;
		void setUniqueColorsID(uint32_t id);

		MAR_NO_DISCARD EMeshBatchType getBatchType() const override;

	private:

		void submitColor(const ColorComponent& colorComponent);


		FColorsArray m_colors;
		uint32_t m_uniqueColorsID{ 0 };

	};


}


#endif // !MAR_ENGINE_F_MESH_BATCH_STATIC_COLOR_H
