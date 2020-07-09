#include "Shader.h"

#include <fstream>

#include <glm/gtc/type_ptr.hpp>

// Constructor
Shader::Shader(const std::string &name) : m_name(name), m_progId(0), m_vertId(0), m_fragId(0) {}

// Destructor
Shader::~Shader() {
  glDetachShader(m_progId, m_vertId);
  glDetachShader(m_progId, m_fragId);
  glDeleteProgram(m_progId);
  glDeleteShader(m_vertId);
  glDeleteShader(m_fragId);
}

// Load all shader files and compiles them
void Shader::compile(ERROR &errCode, bool useShader) {
  // Get file paths
  const std::string vertPath = "Shaders/" + m_name + ".vert";
  const std::string fragPath = "Shaders/" + m_name + ".frag";

  // Load vertex and fragment shaders
  load(vertPath, GL_VERTEX_SHADER, m_vertId, errCode);
  load(fragPath, GL_FRAGMENT_SHADER, m_fragId, errCode);
  linkPrograms(errCode);
  bindUniforms(errCode);

  if (errCode == ERROR_OK) {
    m_isCompiled = true;
    if (useShader)
      use(); // switch current program to this shader
  }
}

// Loads shader from file
void Shader::load(const std::string &filename, GLenum type, GLuint &shaderId, ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  // Open the file
  std::ifstream file(filename.c_str());

  if (file) {
    // Get shader source code from file
    std::string source;
    source.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Find and set any preprocessor values
    preprocess(source, type, errCode);
    const char *cSource = source.c_str();

    // Create and compile shader
    shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, (const GLchar **)&cSource, 0);
    glCompileShader(shaderId);

    // Perform error-checking on shaders
    GLint isCompiled = GL_FALSE;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

    if (!isCompiled) {
      errCode = ERROR_SHADER_COMPILE_FAILED;
      printErrorMsg(errCode, shaderId, getShaderErrorLog(shaderId));
    }
  } else {
    errCode = ERROR_FILE_LOAD_FAILED;
    printErrorMsg(errCode, filename.c_str());
  }
}

// Compiles all loaded shaders into the program
void Shader::linkPrograms(ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  m_progId = glCreateProgram();
  if (!m_progId) {
    errCode = ERROR_SHADER_PROGRAM_CREATE_FAILED;
    printErrorMsg(errCode, m_progId);
    return;
  }

  // Attach all shaders to the program
  glAttachShader(m_progId, m_fragId);
  glAttachShader(m_progId, m_vertId);

  // Link the program
  glLinkProgram(m_progId);
  GLint isLinked, isValid;
  glGetProgramiv(m_progId, GL_LINK_STATUS, &isLinked);

  if (isLinked) {
    // Validate the program
    glValidateProgram(m_progId);
    glGetProgramiv(m_progId, GL_VALIDATE_STATUS, &isValid);

    if (!isValid)
      errCode = ERROR_SHADER_PROGRAM_INVALID;
  } else {
    errCode = ERROR_SHADER_PROGRAM_LINKING_FAILED;
  }

  // Report any errors that occurred
  if (errCode != ERROR_OK)
    printErrorMsg(errCode, getProgramErrorLog(m_progId));
}

// Finds all uniforms in the shader program and binds each to an id
void Shader::bindUniforms(ERROR &errCode) {
  if (errCode != ERROR_OK)
    return;

  GLint count;
  GLchar name[MAX_PARAM_LENGTH];

  // Get uniforms
  glGetProgramiv(m_progId, GL_ACTIVE_UNIFORMS, &count);
  for (GLuint i = 0; i < (GLuint)count; ++i) {
    glGetActiveUniform(m_progId, i, MAX_PARAM_LENGTH, NULL, NULL, NULL, name);
    m_uniformMap[name] = glGetUniformLocation(m_progId, name);
  }
}

// Set preprocessor values into shader source code string
void Shader::preprocess(std::string &shaderSource, GLenum type, ERROR &errCode) const {
  if (errCode != ERROR_OK)
    return;

  // Check if target shader type exists in the map
  const auto preprocessorIt = m_preprocessorMap.find(type);
  if (preprocessorIt == m_preprocessorMap.end())
    return;

  for (const auto &[name, value] : preprocessorIt->second) {
    // Try finding the preprocessor string in source code
    const std::string preprocessorStr = "#define " + name;
    size_t strStartPos = shaderSource.find(preprocessorStr);

    if (strStartPos == std::string::npos) {
      errCode = ERROR_SHADER_PREPROCESSOR_NOT_FOUND;
      printErrorMsg(errCode, name.c_str(), m_name.c_str());
    }

    // Find the next newline char
    size_t strEndPos = strStartPos + preprocessorStr.size();
    size_t newlinePos = shaderSource.find("\n", strEndPos);
    size_t replaceLen = newlinePos - strEndPos;

    // Replace all characters after the name string to the newline with the preprocessor value
    shaderSource.replace(strEndPos, replaceLen, " " + value);
  }
}

// Use this shader
void Shader::use() const { glUseProgram(m_progId); }

// Gets the id of a uniform from the shader program
GLint Shader::getUniformId(const std::string &name, ERROR &errCode) const {
  if (errCode != ERROR_OK)
    return -1;

  const auto it = m_uniformMap.find(name);
  if (it == m_uniformMap.end()) {
    errCode = ERROR_SHADER_MISSING_PARAMETER;
    printErrorMsg(errCode, name.c_str());
    return -1;
  }

  return it->second;
}

// Uniform setter utility functions. The shader must first be compiled and in current usage
// for any of these functions to be used.
void Shader::setBool(const std::string &name, bool value, ERROR &errCode) const {
  glUniform1i(getUniformId(name, errCode), value);
}

void Shader::setInt(const std::string &name, int value, ERROR &errCode) const {
  glUniform1i(getUniformId(name, errCode), value);
}

void Shader::setUint(const std::string &name, unsigned int value, ERROR &errCode) const {
  glUniform1ui(getUniformId(name, errCode), value);
}

void Shader::setFloat(const std::string &name, float value, ERROR &errCode) const {
  glUniform1f(getUniformId(name, errCode), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value, ERROR &errCode) const {
  glUniform2fv(getUniformId(name, errCode), 1, glm::value_ptr(value));
}

void Shader::setVec2(const std::string &name, GLfloat x, GLfloat y, ERROR &errCode) const {
  glUniform2f(getUniformId(name, errCode), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value, ERROR &errCode) const {
  glUniform3fv(getUniformId(name, errCode), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, GLfloat x, GLfloat y, GLfloat z,
                     ERROR &errCode) const {
  glUniform3f(getUniformId(name, errCode), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value, ERROR &errCode) const {
  glUniform4fv(getUniformId(name, errCode), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w,
                     ERROR &errCode) const {
  glUniform4f(getUniformId(name, errCode), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &value, ERROR &errCode) const {
  glUniformMatrix2fv(getUniformId(name, errCode), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &value, ERROR &errCode) const {
  glUniformMatrix3fv(getUniformId(name, errCode), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value, ERROR &errCode) const {
  glUniformMatrix4fv(getUniformId(name, errCode), 1, GL_FALSE, glm::value_ptr(value));
}

// Adds an entry to the preprocessor values map to use when compiling the shader.
// NOTE: Unlike the uniform setter functions, preprocessor values are applied before shader
// compilation, so this method will have no effect if this shader is already compiled!
void Shader::setPreprocessor(GLenum type, const std::string &name, const std::string &value) {
  m_preprocessorMap[type][name] = value;
}

void Shader::setPreprocessor(GLenum type, const std::string &name, bool value) {
  m_preprocessorMap[type][name] = std::to_string(value);
}

void Shader::setPreprocessor(GLenum type, const std::string &name, int value) {
  m_preprocessorMap[type][name] = std::to_string(value);
}

void Shader::setPreprocessor(GLenum type, const std::string &name, float value) {
  m_preprocessorMap[type][name] = std::to_string(value);
}

// Prints the error log of a shader
char *Shader::getShaderErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetShaderInfoLog(id, errorLength, &errorLength, errorLog.data());

  // Copy errLog to output
  size_t len = errorLog.size() + 1;
  char *outStr = new char[len];
  strcpy_s(outStr, len, errorLog.data());

  return outStr;
}

// Prints the error log of the program
char *Shader::getProgramErrorLog(GLuint id) const {
  GLint errorLength = 0;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
  std::vector<GLchar> errorLog(errorLength);
  glGetProgramInfoLog(id, errorLength, &errorLength, errorLog.data());

  // Copy errLog to output
  size_t len = errorLog.size() + 1;
  char *outStr = new char[len];
  strcpy_s(outStr, len, errorLog.data());

  return outStr;
}
