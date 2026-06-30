#include "_shader.h"

_shader::_shader()
{
    //ctor
}

_shader::~_shader()
{
    cleanUp();
}

void _shader::loadFile(char* filename, string& str)
{
    char tmp[1024];

    ifstream file(filename);

    if(!file.is_open())
        cerr << "[ERROR] File did not load: " << filename << endl;

    while(file.getline(tmp,1024))
    {
        str +=tmp;
        str +='\n';
    }
}

unsigned int _shader::loadShader(string& src, unsigned int mode)
{
    unsigned int id;

    const char* cSrc;
    char err[1024];

    id = glCreateShader(mode);
    cSrc = src.c_str();

    glShaderSource(id, 1, &cSrc, NULL);
    glCompileShader(id);

    glGetShaderInfoLog(id, 1024, NULL, err);

    cout<< "Compile Status: "<< err << endl;

    return id;
}

void _shader::init(char* vFilename, char* fFilename)
{
    string str;

    loadFile(vFilename, str);
    vs = loadShader(str, GL_VERTEX_SHADER);

    str = ""; // Clear string so it can be reused

    loadFile(fFilename, str);
    fs = loadShader(str, GL_FRAGMENT_SHADER);

    prog = glCreateProgram();

    glAttachShader(prog, vs);
    glAttachShader(prog, fs);

    glLinkProgram(prog);
}

void _shader::cleanUp()
{
    // Detach and delete all shader content
    glDetachShader(prog, vs);
    glDetachShader(prog, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(prog);
}
