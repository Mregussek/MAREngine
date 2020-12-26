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


#include "PipelineStorage.h"
#include "../GraphicsLogs.h"
#include "../RenderAPI/RenderContainer.h"


namespace mar::graphics {


	PipelineStorage* PipelineStorage::Instance{ nullptr };


	platforms::PipelineOpenGL& PipelineStorage::createPipeline() {
		GRAPHICS_TRACE("PIPELINE_STORAGE: creating pipeline... current size {}", m_buffers.size() + 1);

		return m_buffers.emplace_back();
	}

	const std::vector<platforms::PipelineOpenGL>& PipelineStorage::getPipelines() const {
		return m_buffers;
	}

	const platforms::PipelineOpenGL& PipelineStorage::getPipeline(uint32_t index) const {
		return m_buffers.at(index);
	}

	void PipelineStorage::fillContainer(RenderContainer& container) const {
		container.m_uniqueContainerID = m_buffers.size() - 1;
	}

	void PipelineStorage::close() {
		GRAPHICS_TRACE("PIPELINE_STORAGE: closing...");

		for (auto& buffer : m_buffers) { buffer.close(); }

		m_buffers.clear();
	}


}