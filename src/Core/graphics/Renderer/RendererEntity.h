/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#ifndef MAR_ENGINE_RENDERER_ENTITY_H
#define MAR_ENGINE_RENDERER_ENTITY_H

#include "../../../mar.h"

#include "RendererColor.h"
#include "RendererTexture.h"
#include "Renderer.h"


namespace mar {
	namespace graphics {

		struct RendererStatistics;


		class RendererEntity {
			// --- SPECIFIC RENDERERS --- //
			
			RendererColor m_renColor;
			RendererTexture m_ren2D;
			RendererTexture m_renCube;

			// --- OTHER --- //

			static RendererStatistics s_stats;

		public:
			
			void initialize();

			void close();
			
			void update(ecs::Scene* scene);

			// --- STATISTICS --- //
			
			static RendererStatistics& getStatistics();
			static void clearStatistics();
		};


} }


#endif // !MAR_ENGINE_RENDERER_ENTITY_H
