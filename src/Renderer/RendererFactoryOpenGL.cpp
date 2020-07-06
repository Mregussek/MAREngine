/*
 *	Mateusz Rzeczyca
 *	Copyright (C) 2020 Mateusz Rzeczyca <info@mateuszrzeczyca.pl>
 */

#include "RendererOpenGLFactory.h"


namespace mar {
	namespace graphics {


		std::vector<ShaderType> RendererOpenGLFactory::s_shaderTypes;
		std::vector<Ref<ShaderOpenGL>> RendererOpenGLFactory::s_storedShaders;


} }