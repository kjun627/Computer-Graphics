#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
CLASS_PTR(Context)
class Context{
public:
    static ContextUPtr Create();
    void Render();
private:
    Context(){}
    bool Init();
    ProgramUPtr m_program;

    // VBO 를 담아둘 uint32_t
    uint32_t m_vertexBuffer;
    // VAO 를 담아둘 uint32_t
    uint32_t m_vertexArrayObject;
    // index buffer를 위한 uint32_t
    uint32_t m_indexBuffer;
};
#endif