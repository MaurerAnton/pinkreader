// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLProgram.java
#pragma once

#include <GLES2/gl2.h>
#include <string>
#include <stdexcept>
#include <cstdio>

namespace PinkReader {

class RRGLProgram {
private:
	int mHandle = 0;
	int mFragmentShaderHandle = -1;
	int mVertexShaderHandle = -1;

	void compileAndAttachShader(int type, const std::string& source) {
		switch (type) {
			case GL_FRAGMENT_SHADER:
				if (mFragmentShaderHandle != -1) {
					throw std::runtime_error("Fragment shader already attached");
				}
				break;
			case GL_VERTEX_SHADER:
				if (mVertexShaderHandle != -1) {
					throw std::runtime_error("Vertex shader already attached");
				}
				break;
			default:
				throw std::runtime_error("Unknown shader type.");
		}

		int shaderHandle = glCreateShader(type);
		if (shaderHandle == 0) {
			throw std::runtime_error("Error creating shader.");
		}

		const char* srcCStr = source.c_str();
		glShaderSource(shaderHandle, 1, &srcCStr, nullptr);
		glCompileShader(shaderHandle);

		int compileStatus = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus == 0) {
			char logBuf[1024] = {};
			glGetShaderInfoLog(shaderHandle, sizeof(logBuf), nullptr, logBuf);
			glDeleteShader(shaderHandle);
			char msg[2048];
			snprintf(msg, sizeof(msg), "Shader compile error: \"%s\".", logBuf);
			throw std::runtime_error(msg);
		}

		glAttachShader(mHandle, shaderHandle);

		switch (type) {
			case GL_FRAGMENT_SHADER:
				mFragmentShaderHandle = shaderHandle;
				break;
			case GL_VERTEX_SHADER:
				mVertexShaderHandle = shaderHandle;
				break;
			default:
				throw std::runtime_error("Unknown shader type.");
		}
	}

	void link() {
		if (mFragmentShaderHandle == -1 || mVertexShaderHandle == -1) {
			throw std::runtime_error("Missing shaders before link");
		}

		glLinkProgram(mHandle);

		int linkStatus = 0;
		glGetProgramiv(mHandle, GL_LINK_STATUS, &linkStatus);

		if (linkStatus == 0) {
			char logBuf[1024] = {};
			glGetProgramInfoLog(mHandle, sizeof(logBuf), nullptr, logBuf);
			glDeleteProgram(mHandle);
			char msg[2048];
			snprintf(msg, sizeof(msg), "Linker error: \"%s\".", logBuf);
			throw std::runtime_error(msg);
		}

		glDetachShader(mHandle, mFragmentShaderHandle);
		glDetachShader(mHandle, mVertexShaderHandle);

		glDeleteShader(mFragmentShaderHandle);
		glDeleteShader(mVertexShaderHandle);

		mFragmentShaderHandle = -1;
		mVertexShaderHandle = -1;
	}

public:
	RRGLProgram(const std::string& vertexShaderSource,
	            const std::string& fragmentShaderSource) {
		mHandle = glCreateProgram();

		if (mHandle == 0) {
			throw std::runtime_error("Error creating program.");
		}

		compileAndAttachShader(GL_VERTEX_SHADER, vertexShaderSource);
		compileAndAttachShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		link();
	}

	virtual ~RRGLProgram() {
		if (mHandle != 0) {
			glDeleteProgram(mHandle);
		}
	}

	// Non-copyable
	RRGLProgram(const RRGLProgram&) = delete;
	RRGLProgram& operator=(const RRGLProgram&) = delete;

	int getAttributeHandle(const std::string& name) {
		return glGetAttribLocation(mHandle, name.c_str());
	}

	int getUniformHandle(const std::string& name) {
		return glGetUniformLocation(mHandle, name.c_str());
	}

	int getHandle() const {
		return mHandle;
	}

	virtual void onActivated() = 0;
	virtual void onDeactivated() = 0;
};

} // namespace PinkReader
