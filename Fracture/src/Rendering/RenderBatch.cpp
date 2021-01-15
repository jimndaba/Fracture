#include "RenderBatch.h"


Fracture::RenderBatch::RenderBatch()
{
   
    

    glGenBuffers(1, &indirect_draw_buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_draw_buffer);
    glBufferData(GL_DRAW_INDIRECT_BUFFER, NUM_DRAWS * sizeof(DrawElementsIndirectCommand),NULL,GL_STATIC_DRAW);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER,0);
}

Fracture::RenderBatch::~RenderBatch()
{
}

void Fracture::RenderBatch::prepare()
{
    DrawElementsIndirectCommand* cmd = (DrawElementsIndirectCommand*)
        glMapBufferRange(GL_DRAW_INDIRECT_BUFFER,0,NUM_DRAWS * sizeof(DrawElementsIndirectCommand),GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);



    glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
}
