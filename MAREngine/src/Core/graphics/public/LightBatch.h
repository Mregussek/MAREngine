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


#ifndef MARENGINE_FLIGHTBATCH_H
#define MARENGINE_FLIGHTBATCH_H


#include "ILightBatch.h"
#include "Light.h"


namespace marengine {


    class FLightBatch : public ILightBatch {
    public:

        MAR_NO_DISCARD int32 getLightSSBO() const final;
        void passLightSSBO(int32 index) final;

    protected:

        uint32_t p_lightSSBO{ 0 };

    };


	class FPointLightBatch : public FLightBatch {
	public:

		void reset();

        MAR_NO_DISCARD const float* getBatchData() const final;
        MAR_NO_DISCARD uint32 getCountLight() const final;

        MAR_NO_DISCARD bool shouldBeBatched(const Entity& entity) const final;
		MAR_NO_DISCARD bool canBeBatched(const Entity& entityWithLight) const final;
		void submitToBatch(const Entity& entity) final;

		void updateLight(const Entity& entity);

		MAR_NO_DISCARD ELightBatchType getType() const final;

	private:

		std::vector<FPointLight> m_lights;


	};


	class FLightBatchStorage : public ILightBatchStorage {
	public:

        void reset() final;

        MAR_NO_DISCARD FPointLightBatch* getPointLightBatch() const final;
        MAR_NO_DISCARD uint32 getCountPointLightBatch() const final;

	private:

	    FPointLightBatch m_pointLightBatch;

	};


	class FLightBatchFactory : public ILightBatchFactory {
	public:

	    MAR_NO_DISCARD FPointLightBatch* emplacePointLightBatch() final;

	    MAR_NO_DISCARD FLightBatchStorage* getStorage() const;

	private:

        FLightBatchStorage m_storage;

	};



	

}


#endif // !MARENGINE_FLIGHTBATCH_H
