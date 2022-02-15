 #include "BatchRenderer.h"

BatchRenderer::BatchRenderer(int size, std::string shader_filepath)
	: index_buffer(size), shader(shader_filepath), vertex_buffer(size * 24 * 10 * sizeof(float)) /* I don't know why size is multiplied by 10.                           */
{}                                                                                               /* It seems like that is making it 10 times bigger than it needs to be. */
																								 /* But weird things happen when that isn't there, so there it stays.     */


void BatchRenderer::add_layout(VertexBufferLayout &_layout)
{
	vertex_array.Bind();
	vertex_buffer.Bind();

	vertex_array.AddBuffer(vertex_buffer, _layout);
}

void BatchRenderer::draw(glm::mat4 proj_matrix)
{
	vertex_array.Bind();
	shader.Bind();
	index_buffer.Bind();


	shader.SetUniformMat4f("u_MVP", proj_matrix);
	shader.SetUniform1iv("u_Textures", 16, samplers);

	GLCall(glDrawElements(GL_TRIANGLES, index_buffer.GetCount(), GL_UNSIGNED_INT, nullptr))
}



