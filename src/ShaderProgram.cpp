#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

# include <GL/glew.h>

#include "./../include/ShaderProgram.hpp"
#include "./../include/log.hpp"
#include "./../include/gl_helper.hpp"

using namespace std;

int ShaderProgram::null_location = -1;

static void
dump_shader_log( GLuint shader )
{
  GLint logLen;
  glcheck(glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLen ));
  if( logLen > 0 )
    {
      char*  log = (char* )malloc(logLen);
      GLsizei written;
      glcheck(glGetShaderInfoLog( shader, logLen, &written, log));
      LOG( warning, "shader log:\n" << log );
      free(log);
    }
}

static GLint
check_program_status(GLuint program)
{
  int     info_log_length = 0;
  int     chars_written  = 0;
  GLint   status;

  glcheck(glGetProgramiv(program, GL_LINK_STATUS, &status));
  glcheck(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length));

  if (info_log_length > 1)
  {
      std::vector<char> info_log(info_log_length+1);
      glcheck(glGetProgramInfoLog(program, info_log_length, &chars_written, &info_log[0]));
      LOG( info, "program log:\n" << &info_log[0] );
  }
  return status;
}

static GLuint
compile_shader( const std::string& gpu_name, GLuint type )
{
  // open the shader file
  std::ifstream gpu_file( gpu_name );
  if ( !gpu_file.is_open() )
    {
      LOG( error, "cannot open shader file " << gpu_name << ". Are you in the right directory?" );
      return 0;
    }

  // create a new shader object
  glcheck(GLuint shader = glCreateShader( type ));
  if ( !shader )
    {
      LOG( error, "error when creating shader [" << gpu_name << "]");
      return 0;
    }

  // load the shader source in one string
  std::stringstream gpu_data;
  gpu_data << gpu_file.rdbuf();
  gpu_file.close(); // no more needed, so close it now
  const std::string gpu_string = gpu_data.str();

  // set the source of the shader (as one big cstring)
  const char*  strShaderVar = gpu_string.c_str();
  GLint iShaderLen = gpu_string.size();
  glcheck(glShaderSource ( shader, 1,(const GLchar**) &strShaderVar, (GLint*)&iShaderLen ));

  // compile the shader
  glcheck(glCompileShader( shader ));
  GLint result;
  glcheck(glGetShaderiv( shader, GL_COMPILE_STATUS, &result ));
  if( GL_FALSE == result )
    {
      LOG( error, "shader [" << gpu_name << "] compilation failed!");
      dump_shader_log( shader );
      glcheck(glDeleteShader( shader ));
      return 0;
    }
  return shader;
}

ShaderProgram::ShaderProgram()
  : m_programId{0}
{}

ShaderProgram::ShaderProgram(
  const std::string& vertex_file_path,
  const std::string& fragment_file_path )
  : m_programId{0}
{
  load( vertex_file_path, fragment_file_path );
}

ShaderProgram::~ShaderProgram()
{
  if( glIsProgram(m_programId) )
    glcheck(glDeleteProgram(m_programId));
}

void ShaderProgram::load(
    const std::string& vertex_file_path,
    const std::string& fragment_file_path )
{
  // ids of the shaders that we will link together to form a program
  GLuint vertex_shader_id = compile_shader( vertex_file_path, GL_VERTEX_SHADER );
  GLuint fragment_shader_id = compile_shader( fragment_file_path, GL_FRAGMENT_SHADER );
  if( !vertex_shader_id || !fragment_shader_id )
    {
      LOG( error, "cannot load shader program. Program unchanged...");
      if( glIsShader( vertex_shader_id ) )
        glcheck(glDeleteShader( vertex_shader_id ));
      if( glIsShader( fragment_shader_id ) )
        glcheck(glDeleteShader( fragment_shader_id ));
      return;
    }

  // previous program id, to restore in case of failure
  unsigned int previous_id = m_programId;

  //Create, attach, Link the program
  glcheck(m_programId = glCreateProgram());
  glcheck(glAttachShader(m_programId, vertex_shader_id));
  glcheck(glAttachShader(m_programId, fragment_shader_id));
  glcheck(glLinkProgram(m_programId));

  // everything is ok: use this new program
  if( check_program_status(m_programId) )
    {
      // if this is already a program, delete all data
      if( glIsProgram( previous_id ) )
        glcheck(glDeleteProgram( previous_id ));
      m_vertexFilename = vertex_file_path;
      m_fragmentFilename = fragment_file_path;

      // load attributes and uniforms
      LOG( info, "resources info for ShaderProgram "<< this << " (" << vertex_file_path << ", " << fragment_file_path << ")");

      glcheck(const GLubyte *version = glGetString(GL_VERSION));
      if (version[0] >= '4') {
        LOG( info, "introspection  OpenGL 4.x");
        resources_introspection_OpenGL4();
      } else if (version[0] <= '3') {
        LOG( info, "introspection  OpenGL 3.x");
        resources_introspection_OpenGL3();
      } else {
        LOG(error, "unknown GL version: " << version << std::endl);
      }
    }
  // it failed: delete new program and restore to previous state
  else
    {
      LOG( warning, "shader program described by (" << vertex_file_path << ", " << fragment_file_path
           << ") is invalid. ShaderProgram " << this << " remains unchanged...");
      glcheck(glDeleteProgram( m_programId ));
      m_programId = previous_id;
    }

  //Delete vertex & fragment id. We do not need them anymore as they are already
  //"in" this program. The only reason to keep those shaders somewhere would be
  //to reused them in order to build another shader program.
  glDeleteShader( vertex_shader_id );
  glDeleteShader( fragment_shader_id );
}

void
ShaderProgram::reload()
{
  if( !m_vertexFilename.empty() && !m_fragmentFilename.empty() )
    load( m_vertexFilename, m_fragmentFilename );
}

void
ShaderProgram::bind()
{
  glcheck(glUseProgram( m_programId ));
}

void
ShaderProgram::unbind()
{
  glcheck(glUseProgram( 0 ));
}

static const GLenum uniform_properties[3] = {
    GL_BLOCK_INDEX,
    GL_NAME_LENGTH,
    GL_LOCATION
};

static const GLenum attribute_properties[2] = {
    GL_NAME_LENGTH,
    GL_LOCATION
};

GLuint ShaderProgram::programId()
{
    return m_programId;
}

void ShaderProgram::resources_introspection_OpenGL4()
{
  //Clean the maps
  m_uniforms.clear();
  m_attributes.clear();

  GLint values[3];

  GLint num_uniforms = 0;
  glcheck(glGetProgramInterfaceiv( m_programId, GL_UNIFORM, GL_ACTIVE_RESOURCES, &num_uniforms));
  LOG( info, " * uniforms: " << num_uniforms);
  LOG( info, "\tLocation   Name");
  for(int unif = 0; unif < num_uniforms; ++unif)
  {
    glcheck(glGetProgramResourceiv( m_programId, GL_UNIFORM, unif, 3, uniform_properties, 3, NULL, values));

    //Skip any uniforms that are in a block.
    if(values[0] != -1)
      continue;

    char* name = new char[values[1]];
    glcheck(glGetProgramResourceName(m_programId, GL_UNIFORM, unif, values[1], NULL, &name[0]));
    LOG( info, "\t" << std::setw(8) << values[2] << "   " << name );
    m_uniforms.insert( {{name, values[2]}});
    delete[] name;
  }

  GLint num_attributes = 0;
  glcheck(glGetProgramInterfaceiv( m_programId, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &num_attributes ));
  LOG( info, " * attributes: " << num_attributes );
  LOG( info, "\tLocation   Name");
  for( int att = 0; att < num_attributes; ++ att )
    {
      glcheck(glGetProgramResourceiv( m_programId, GL_PROGRAM_INPUT, att, 2, attribute_properties, 2, NULL, values ));
      char* name = new char[values[0]];
      glcheck(glGetProgramResourceName(m_programId, GL_PROGRAM_INPUT, att, values[0], NULL, &name[0]));
      LOG( info, "\t" << std::setw(8) << values[1] << "   " << name );
      m_attributes.insert( {{name, values[1] }} );
      delete[]name;
    }

}

void ShaderProgram::resources_introspection_OpenGL3()
{
  //Clean the maps
  m_uniforms.clear();
  m_attributes.clear();

  // Add-on for GL3.30 compatibility.
  // Basically, replace
  //  glGetProgramInterfaceiv, glGetProgramResourceiv & glGetProgramResourceName
  // by
  //  glGetProgramiv, glGetActiveUniform & glGetActiveAttrib
  // The upper forms are supposed to be more generic, but are not
  // (or not well) supporter in OpenGL versions < 4.0

  GLint count;    // nb of elements
  GLint size;     // size of the variable
  GLenum type;    // type of the variable (float, vec3 or mat4, etc)
  const GLsizei bufSize = 255;  // maximum name length
  GLchar name[bufSize];         // variable name in GLSL
  GLsizei length;               // name length

  glcheck(glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &count));
  LOG( info, " * uniforms: " << count);
  LOG( info, "\tLocation   Name");
  for (GLuint i = 0; i < count; i++) {
    glcheck(glGetActiveUniform(m_programId, i, bufSize, &length, &size, &type, name));
    LOG( info, "\t" << std::setw(8) << i << "   " << name );
    m_uniforms.insert( {{name, glGetUniformLocation(m_programId, name)}});
  }

  glcheck(glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTES, &count));
  LOG( info, " * attributes: " << count);
  LOG( info, "\tLocation   Name");
  for (GLuint i = 0; i < count; i++) {
    glcheck(glGetActiveAttrib(m_programId, i, bufSize, &length, &size, &type, name));
    LOG( info, "\t" << std::setw(8) << i << "   " << name );
    m_attributes.insert( {{name, glGetAttribLocation(m_programId, name)}});
  }

  // std::cout << "Uniforms map: " << this->m_vertexFilename << std::endl;
  // for (auto it = m_uniforms.begin(); it != m_uniforms.end(); ++it )
  //     std::cout << " MC" << it->first << ":" << it->second << std::endl;
}

GLint ShaderProgram::getUniformLocation( const std::string& name ) const
{
    std::unordered_map< std::string, int >::const_iterator search = m_uniforms.find(name);
    if (search != m_uniforms.end())
        return search->second;
    // LOG( error, "there is no uniform named \"" << name << "\" in this program " << this->m_vertexFilename );
    return null_location;
}

GLint ShaderProgram::getAttributeLocation( const std::string& name ) const
{
    std::unordered_map< std::string, int >::const_iterator search = m_attributes.find(name);
    if (search != m_attributes.end())
        return search->second;
    // LOG( error, "there is no attribute named \"" << name << "\" in this program " << this->m_vertexFilename );
    return null_location;
}
