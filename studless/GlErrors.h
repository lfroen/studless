
/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef st_GlErrors_h
#define st_GlErrors_h

#include <assert.h>


#ifndef NDEBUG

#define CHECK_GL_ERRORS() \
do {                                                                                            	\
    GLenum error = glGetError();                                                                	\
    if(error != GL_NO_ERROR) {                                                                   	\
		NSLog(@"OpenGL(objc): %s in %s:%d [error %d]", __FUNCTION__, __FILE__, __LINE__, (int)error);					\
		assert(0); \
	} \
} while(false)

#define CHECK_GL_ERRORS_C()										\
do {															\
GLenum error = glGetError();                                    \
if(error != GL_NO_ERROR) {										\
const char *errstr = 0;												\
printf("OpenGL(cpp): %s in %s:%d [error %d => %s]\n",							\
__FUNCTION__,  __FILE__, __LINE__, (int)error, errstr ? errstr : "undef");		\
} \
} while(false)


#define CHECK_GL_ERRORS_C_DOES_NOT_WORK()										\
do {															\
GLenum error = glGetError();                                    \
if(error != GL_NO_ERROR) {										\
const char *errstr = 0;												\
switch(error) {													\
case GL_INVALID_ENUM: errstr = "GL_INVALID_ENUM"; break;		\
case GL_INVALID_VALUE: errstr = "GL_INVALID_VALUE"; break;		\
case GL_INVALID_OPERATION: errstr = "GL_INVALID_OPERATION"; break;		\
case GL_STACK_OVERFLOW: errstr = "GL_STACK_OVERFLOW"; break;		\
case GL_OUT_OF_MEMORY: errstr = "GL_OUT_OF_MEMORY"; break;		\
}																\
printf("OpenGL(cpp): %s in %s:%d [error %d => %s]\n",							\
	__FUNCTION__,  __FILE__, __LINE__, (int)error, errstr ? errstr : "undef");		\
} \
} while(false)

#else

#define CHECK_GL_ERRORS()
#define CHECK_GL_ERRORS_C()

#endif



#endif
