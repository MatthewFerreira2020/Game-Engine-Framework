#include "ShaderHandler.h"

std::unique_ptr<ShaderHandler>ShaderHandler::shaderInstance = nullptr;
std::map<std::string, GLuint>ShaderHandler::programs = std::map<std::string, GLuint >();

ShaderHandler::ShaderHandler()
{

}


ShaderHandler::~ShaderHandler()
{
	if (programs.size() > 0) {
		for (auto entry : programs) {
			glDeleteProgram(entry.second);
		}
		programs.clear();
	}
}

std::string ShaderHandler::ReadShader(const std::string & filename_)
{
	std::string shaderCode;
	std::ifstream file;
	file.exceptions(std::ifstream::badbit);

	try
	{
		file.open(filename_);
		std::stringstream tmpStream;
		tmpStream << file.rdbuf();
		file.close();
		shaderCode = tmpStream.str();
	}
	catch (std::ifstream::failure error_)
	{
		Debug::Error("Couldn't Read Shader File", __FILE__, __LINE__);
		return "";
	}

	return shaderCode;
}

GLuint ShaderHandler::CreateShader(GLenum shaderType_, const std::string & source, const std::string & shaderName_)
{
	GLint compile_result = 0;
	GLuint shader = glCreateShader(shaderType_);
	const char* shaderCodePtr = source.c_str();
	const int shaderCodeSize = source.size();

	glShaderSource(shader, 1, &shaderCodePtr, &shaderCodeSize);

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	if (!compile_result)
	{
		GLint infoLogLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char shaderLog[512];
		glGetShaderInfoLog(shader, infoLogLength, NULL, &shaderLog[0]);
		Debug::Error("ERROR Compiling Shader" + shaderName_ + "\n" + shaderLog, __FILE__, __LINE__);

		return 0;
	}

	return shader;
}

ShaderHandler * ShaderHandler::GetInstance()
{
	if (shaderInstance.get() == nullptr) {
		shaderInstance.reset(new ShaderHandler);
	}
	return shaderInstance.get();
}

void ShaderHandler::CreateProgram(const std::string & shaderName_, const std::string & vertexShaderName_, const std::string & fragmentShaderName_)
{
	std::string vertexShaderCode = ReadShader(vertexShaderName_);
	std::string fragmentShaderCode = ReadShader(fragmentShaderName_);

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexShaderCode, vertexShaderName_);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode, fragmentShaderName_);

	GLint linkResult = 0;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);

	if (!linkResult) 
	{
		GLint infoLogLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		char programInfo[512];
		glGetProgramInfoLog(program, infoLogLength, NULL, &programInfo[0]);
		Debug::Error("Shader Linker: LINK ERROR" + shaderName_ + ": " + programInfo, __FILE__, __LINE__);
		return;
	}

	programs[shaderName_] = program;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

const GLuint ShaderHandler::GetShader(const std::string & shaderName_)
{
	if (programs.find(shaderName_) != programs.end())
	{
		return programs[shaderName_];
	}
	return -1;
}
