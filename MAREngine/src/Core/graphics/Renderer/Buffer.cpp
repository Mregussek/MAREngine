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


#include "Buffer.h"


namespace marengine {


    void FVertexBuffer::pushVariableInfo(const FVertexInputVariableInfo& info) {
        p_inputDescription.inputVariables.push_back(info);
    }

    const FVertexInputDescription& FVertexBuffer::getInputDescription() const {
        return p_inputDescription;
    }

    void FVertexBuffer::setInputDescription(const FVertexInputDescription& inputDescription) {
        p_inputDescription = inputDescription;
    }


    void FIndexBuffer::passIndicesCount(uint32_t indicesCount) {
        p_indicesCount = indicesCount;
    }

    uint32_t FIndexBuffer::getIndicesCount() const {
        return p_indicesCount;
    }


    void FShaderBuffer::pushVariableInfo(const FShaderInputVariableInfo& info) {
        p_inputDescription.inputVariables.push_back(info);
    }
    
    const FShaderInputDescription& FShaderBuffer::getInputDescription() const {
        return p_inputDescription;
    }

    void FShaderBuffer::setInputDescription(const FShaderInputDescription& inputDescription) {
        p_inputDescription = inputDescription;
    }


}
