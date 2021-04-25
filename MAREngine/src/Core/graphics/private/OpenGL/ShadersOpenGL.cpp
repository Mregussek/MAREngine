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


#include "ShadersOpenGL.h"
#include "../../../filesystem/FileManager.h"
#include "../../../../Logging/Logger.h"


namespace marengine {


    template<GLenum TShaderType>
    static GLuint compileGL(const std::string& shaderSourceCode) {
        GL_FUNC_ASSIGN( const GLuint shaderID = glCreateShader(TShaderType) );
        const char* sourceCode{ shaderSourceCode.c_str() };
        GL_FUNC( glShaderSource(shaderID, 1, &sourceCode, nullptr) );
        GL_FUNC( glCompileShader(shaderID) );

        GLint compiledCorrectly{ GL_FALSE };
        GL_FUNC( glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiledCorrectly) );
        if(!compiledCorrectly) {
            GLchar message[256];
			glGetShaderInfoLog(shaderID, sizeof(message), nullptr, message);
			GL_FUNC( glDeleteShader(shaderID) );
			MARLOG_CRIT(ELoggerType::PLATFORMS, "Shader compilator failed: {}", message);
            return GL_FALSE;
        }

        return shaderID;
    }

    template<GLenum TShaderType>
    static GLuint validateThenCompileGL(const char* path) {
        if(path == nullptr) {
            return GL_FALSE;
        }
        else {
            std::string shaderSourceCode{};
            FFileManager::loadFile(shaderSourceCode, path);
            return compileGL<TShaderType>(shaderSourceCode);
        }   
    }

    template<GLenum TShaderType>
    static GLuint compileThenAttachGL(GLuint shaderPipelineID, const char* path) {
        const GLuint id{ validateThenCompileGL<TShaderType>(path) };
        
        if(id != GL_FALSE) {
            GL_FUNC( glAttachShader(shaderPipelineID, id) );
        }
        return id;
    }



    void FShadersOpenGL::compile() {
        GL_FUNC_ASSIGN( GLuint shaderPipelineID = glCreateProgram() );

        const GLuint vertexID =
            compileThenAttachGL<GL_VERTEX_SHADER>(shaderPipelineID, p_vertexPath);
        const GLuint fragmentID =
            compileThenAttachGL<GL_FRAGMENT_SHADER>(shaderPipelineID, p_fragPath);
        const GLuint computeID =
            compileThenAttachGL<GL_COMPUTE_SHADER>(shaderPipelineID, p_computePath);
        const GLuint tessEvalID =
            compileThenAttachGL<GL_TESS_EVALUATION_SHADER>(shaderPipelineID, p_tessEvalPath);
        const GLuint tessControlID =
            compileThenAttachGL<GL_TESS_CONTROL_SHADER>(shaderPipelineID, p_tessControlPath);
        const GLuint geometryID =
            compileThenAttachGL<GL_GEOMETRY_SHADER>(shaderPipelineID, p_geometryPath);

        GL_FUNC( glLinkProgram(shaderPipelineID) );
        GL_FUNC( glValidateProgram(shaderPipelineID) );

        GLint linkStatus{ GL_FALSE };
        GL_FUNC( glGetProgramiv(shaderPipelineID, GL_LINK_STATUS, &linkStatus) );
        if(linkStatus == GL_FALSE) {
            GLchar message[256];
			glGetProgramInfoLog(shaderPipelineID, sizeof(message), nullptr, message);
			MARLOG_CRIT(ELoggerType::PLATFORMS, "Shader link failed: {}", message);
            shaderPipelineID = GL_FALSE;
        }

        if(vertexID != GL_FALSE) { 
            GL_FUNC( glDeleteShader(vertexID) );
        }
        if(fragmentID != GL_FALSE) {
            GL_FUNC( glDeleteShader(fragmentID) );
        }
        if(computeID != GL_FALSE) {
            GL_FUNC( glDeleteShader(computeID) );
        }
        if(tessEvalID != GL_FALSE) {
            GL_FUNC( glDeleteShader(tessEvalID) );
        }
        if(tessControlID != GL_FALSE) {
            GL_FUNC( glDeleteShader(tessControlID) );
        }
        if(geometryID != GL_FALSE) {
            GL_FUNC( glDeleteShader(geometryID) );
        }

        m_id = shaderPipelineID;
    }

    void FShadersOpenGL::close() {
        GL_FUNC( glDeleteProgram(m_id) );
    }

    void FShadersOpenGL::bind() {
        GL_FUNC( glUseProgram(m_id) );
    }

    uint32 FShadersOpenGL::getID() const {
        return m_id;
    }


    uint32 FShadersStorageOpenGL::getCount() const {
        return m_shadersArray.size();
    }

    FShaders* FShadersStorageOpenGL::get(int32 index) const {
        return (FShaders*)&m_shadersArray.at(index);
    }

    void FShadersStorageOpenGL::reset() {
        for(auto& shaders : m_shadersArray) {
            shaders.close();
        }
        m_shadersArray.clear();
    }


    template<typename TReturnType, typename TShadersArray>
    static TReturnType* emplaceShaderAtArray(TShadersArray& array) {
        auto& shaders{ array.emplace_back() };
        const int8_t currentSize{ (int8_t)array.size() };
        shaders.setIndex( currentSize - 1);
        return (TReturnType*)&shaders;
    }

    FShaders* FShadersFactoryOpenGL::emplace() {
        return emplaceShaderAtArray<FShaders>(m_storage.m_shadersArray);
    }


}
