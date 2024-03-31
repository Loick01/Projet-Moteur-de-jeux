#include <Skybox.hpp>

Skybox::Skybox(){
    this->programID_skybox = LoadShaders("../shaders/skybox_vertex.vert", "../shaders/skybox_fragment.frag");

    // Sommet de la skybox
    float verts[24] = {
        -1.0f,-1.0f,1.0f,
        1.0f,-1.0f,1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,
        1.0f,1.0f,-1.0f,
        -1.0f,1.0f,-1.0f
    };
    std::copy(std::begin(verts), std::end(verts), std::begin(this->skyboxVertices));

    // Indices des triangles
    unsigned int is[36] = {
        // Right
        1,2,6,
        1,6,5,
        // Front
        0,1,5,
        0,5,4,
        // Left
        3,0,4,
        3,4,7,
        // Back
        2,3,7,
        2,7,6,
        // Bottom
        1,0,3,
        1,3,2,
        // Top
        4,6,7,
        4,5,6
    };
    std::copy(std::begin(is), std::end(is), std::begin(this->indices));

    // Chemin des 6 textures
    std::string textures[6] = {
        "../Textures/Skybox/s1_back.png",
        "../Textures/Skybox/s1_front.png",
        "../Textures/Skybox/s1_bottom.png",
        "../Textures/Skybox/s1_top.png",
        "../Textures/Skybox/s1_left.png",
        "../Textures/Skybox/s1_right.png"
    };
    std::copy(std::begin(textures), std::end(textures), std::begin(this->pathTextures));
}

void Skybox::loadCubemap(){
    glGenTextures(1, &(this->textureID));
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char *data = stbi_load(this->pathTextures[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << this->pathTextures[i].c_str() << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}  

void Skybox::loadSkybox(){
    loadCubemap(); 

    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->skyboxVertices), &this->skyboxVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), &this->indices , GL_STATIC_DRAW);
}

void Skybox::drawSkybox(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    3*sizeof(float),    // stride
                    (void*)0            // array buffer offset
                    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);

    // Draw the triangles !
    glDrawElements(
                    GL_TRIANGLES,      // mode
                    sizeof(this->indices), // count
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);

    glDepthMask(GL_FALSE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

GLuint Skybox::getShaderID(){
    return this->programID_skybox;
}