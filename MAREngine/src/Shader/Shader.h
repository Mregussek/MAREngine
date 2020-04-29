
#ifndef SHADER_H
#define SHADER_H

#include "../mre.h"

class Shader {
	struct ShaderProgramSource {
		std::string _vertexSource;
		std::string _fragmentSource;
	};

	unsigned int _rendererId;
	std::string _filePath;
	std::unordered_map<std::string, int> _uniformLocationCache;
	ShaderProgramSource _programSource;

public:
	Shader(const std::string& filePath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform4f(const std::string& name, float red, float green, float blue, float alpha);

private:
	int getUniformLocation(const std::string& name);

	ShaderProgramSource parseShader();
	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader();
};

#endif // SHADER_H