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


#ifndef MARENGINE_COMMONTYPEHANDLER_INL
#define MARENGINE_COMMONTYPEHANDLER_INL


#include "../../../../mar.h"
#include "CommonTypeHandler.h"


namespace marengine {


    template<size_t TSize>
    bool FCommonTypeHandler::drawStringInputPanel(std::string& displayStr) {
        char strRealTimeModify[TSize]{ "" };

        std::fill(std::begin(strRealTimeModify), std::end(strRealTimeModify), '\0');
        std::copy(displayStr.begin(), displayStr.end(), strRealTimeModify);

        if (ImGui::InputText(" - tag", strRealTimeModify, TSize)) {
            displayStr = std::string(strRealTimeModify);
            return true;
        }

        return false;
    }

    template<typename TVector>
    bool FCommonTypeHandler::drawVectorInputPanel(const char* label, TVector& vectorToInput, float resetValue,
                                                  float columnWidth, float minValue, float maxValue) {
        ImGui::Text("Not supported type for drawVectorInputPanel");
        return false;
    }


    // Here add declarations for specific component event implementations ...

    template<>
    bool FCommonTypeHandler::drawVectorInputPanel<maths::vec3>(const char* label, maths::vec3& vectorToInput,
                                                               float resetValue, float columnWidth, float minValue,
                                                               float maxValue);

    template<>
    bool FCommonTypeHandler::drawVectorInputPanel<maths::vec4>(const char* label, maths::vec4& vectorToInput,
                                                               float resetValue, float columnWidth, float minValue,
                                                               float maxValue);


}


#endif //MARENGINE_COMMONTYPEHANDLER_INL
