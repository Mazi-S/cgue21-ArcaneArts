#include "egpch.h"
#include "OpenGLTransformFeedback.h"

#include <glad/glad.h>

Engine::OpenGL::GlTransformFeedback::GlTransformFeedback()
{
	glGenTransformFeedbacks(1, &m_RendererID);
}

Engine::OpenGL::GlTransformFeedback::~GlTransformFeedback()
{
	glDeleteTransformFeedbacks(1, &m_RendererID);
}

void Engine::OpenGL::GlTransformFeedback::Bind()
{
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_RendererID);
}

void Engine::OpenGL::GlTransformFeedback::BindBuffer(const Ref<GlVertexBuffer>& buffer, uint32_t index)
{
	BindBuffer(buffer->GetRendererID(), index);
}

void Engine::OpenGL::GlTransformFeedback::BindBuffer(const GlVertexBuffer& buffer, uint32_t index)
{
	BindBuffer(buffer.GetRendererID(), index);
}

void Engine::OpenGL::GlTransformFeedback::BindBuffer(uint32_t bufferID, uint32_t index)
{
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_RendererID);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, bufferID);
}
