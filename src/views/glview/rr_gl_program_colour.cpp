// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLProgramColour.java (static definitions)
#include "rr_gl_program_colour.h"

namespace PinkReader {

const char* RRGLProgramColour::vertexShaderSource =
	"uniform mat4 u_Matrix; \n"
	"uniform mat4 u_PixelMatrix; \n"
	"attribute vec4 a_Position; \n"
	"attribute vec2 a_TexCoordinate; \n"
	"varying vec2 v_TexCoordinate; \n"
	"void main() {\n"
	"  v_TexCoordinate = a_TexCoordinate; \n"
	"  gl_Position = u_PixelMatrix * (u_Matrix * a_Position);\n"
	"} \n";

const char* RRGLProgramColour::fragmentShaderSource =
	"precision mediump float; \n"
	"uniform vec4 u_Color; \n"
	"void main() { \n"
	"  gl_FragColor = u_Color; \n"
	"} \n";

} // namespace PinkReader
