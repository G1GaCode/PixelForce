#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../headers/stb_image.h"

#include "../headers/shader.h"
#include "../headers/cube.hpp"

void Cube::SettingGL()
{
    this->ourShader = Shader(("shaders/" + this->shader_path + ".vert").c_str(), ("shaders/" + this->shader_path + ".frag").c_str());
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Cube::Cube(GLfloat vertices[180], std::string shader_path)
{
    this->shader_path = shader_path;
    for (int i = 0; i < 180; i++)
        this->vertices[i] = vertices[i];
    this->SettingGL();
}

Cube::~Cube()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    this->ourShader.Delete();
}

void Cube::Draw(glm::vec3 pos, float angle, glm::vec3 axis_rot)
{
    if (this->tex_id != 0)
        glBindTexture(GL_TEXTURE_2D, this->tex_id);
    this->ourShader.Active();
    // random position
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(angle), axis_rot);
    model = glm::translate(model, pos);
    this->model = model;
    // Get their uniform location
    GLint MatrixID = glGetUniformLocation(this->ourShader.ID, "model");
    // Pass them to the shaders
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(this->model[0][0]));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::Add(glm::vec3 pos, float angle, glm::vec3 axis_rot)
{
    glBindVertexArray(this->VAO);
    glm::mat4 new_model = this->model;
    new_model = glm::translate(new_model, pos);
    new_model = glm::rotate(new_model, glm::radians(angle), axis_rot);
    GLint MatrixID = glGetUniformLocation(this->ourShader.ID, "model");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &(new_model[0][0]));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Cube::SetTexture(std::string name_image)
{
    GLuint texture;
    glGenTextures(1, &texture);
    this->tex_id = texture;
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char *image = stbi_load(("images/" + name_image).c_str(), &width, &height, 0, STBI_rgb);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}
