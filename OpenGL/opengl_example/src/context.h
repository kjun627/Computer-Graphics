#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "common.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"

CLASS_PTR(Context)
class Context{
public:
    static ContextUPtr Create();
    void Render();
private:
    Context(){}
    bool Init();
    ProgramUPtr m_program;

    // VAO 를 담아둘 uint32_t
    uint32_t m_vertexArrayObject;
    BufferUPtr m_vertexBuffer;
    BufferUPtr m_indexBuffer;
};
#endif