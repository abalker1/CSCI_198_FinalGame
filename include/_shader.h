#ifndef _SHADER_H
#define _SHADER_H

#include <_common.h>

class _shader
{
    public:
        _shader();
        virtual ~_shader();

        void loadFile(char* src, string& str);  // Load the specified shader file
        unsigned vs;
        unsigned fs;
        unsigned prog;

        unsigned int loadShader(string& src, unsigned int mode);    // Load the specified shader
        void init(char* vFilename, char* fFilename);                // Initialize the shaders
        void cleanUp();                                             // Clean up shaders



    protected:

    private:
};

#endif // _SHADER_H
