
#ifndef SHADER_H
#define SHADER_H

#include "mre.h"

class Shader {
public:
	struct ShaderProgramSource {
		std::string _vertexSource;
		std::string _fragmentSource;
	};

	static ShaderProgramSource parseShader(const std::string& filePath);
	static unsigned int compileShader(unsigned int type, const std::string& source);
	static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};

#endif // SHADER_H