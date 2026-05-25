// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLProgramTexture.java (static definitions)
#include "rr_gl_program_texture.h"

namespace PinkReader {

const char* RRGLProgramTexture::vertexShaderSource =
	"uniform mat4 u_Matrix; \n"
	"uniform mat4 u_PixelMatrix; \n"
	"attribute vec4 a_Position; \n"
	"attribute vec2 a_TexCoordinate; \n"
	"varying vec2 v_TexCoordinate; \n"
	"void main() {\n"
	"  v_TexCoordinate = a_TexCoordinate; \n"
	"  gl_Position = u_PixelMatrix * (u_Matrix * a_Position);\n"
	"} \n";

const char* RRGLProgramTexture::fragmentShaderSource =
	"precision mediump float; \n"
	"uniform sampler2D u_Texture; \n"
	"varying vec2 v_TexCoordinate; \n"
	"void main() { \n"
	"  gl_FragColor = texture2D(u_Texture, v_TexCoordinate); \n"
	"} \n";

} // namespace PinkReader
