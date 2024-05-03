#include <Skybox.hpp>

Skybox::Skybox() {
    this->createSkybox();
    this->programID_Skybox = LoadShaders("../shaders/skybox_vertex.vert", "../shaders/skybox_fragment.frag");
    this->loadCubemap();
}

Skybox::~Skybox(){
    std::cout << "Destruction de Skybox\n";
    glDeleteBuffers(1, &(this->vertexbuffer));
    glDeleteBuffers(1, &(this->elementbuffer));
}

void Skybox::createSkybox() {  
    this->vertices.push_back(-1.0f );     this->vertices.push_back( -1.0f );     this->vertices.push_back( 1.0f );
    this->vertices.push_back( 1.0f );     this->vertices.push_back( -1.0f );     this->vertices.push_back( 1.0f );
    this->vertices.push_back( 1.0f );     this->vertices.push_back( -1.0f );     this->vertices.push_back( -1.0f );
    this->vertices.push_back( -1.0f );     this->vertices.push_back( -1.0f );     this->vertices.push_back( -1.0f );
    this->vertices.push_back( -1.0f );     this->vertices.push_back( 1.0f );     this->vertices.push_back( 1.0f );
    this->vertices.push_back( 1.0f );     this->vertices.push_back( 1.0f );     this->vertices.push_back( 1.0f );
    this->vertices.push_back( 1.0f );     this->vertices.push_back( 1.0f );     this->vertices.push_back( -1.0f );
    this->vertices.push_back( -1.0f );     this->vertices.push_back( 1.0f );     this->vertices.push_back( -1.0f );


    this->indices.push_back(1);     this->indices.push_back(6);     this->indices.push_back(2);
    this->indices.push_back(1);     this->indices.push_back(5);     this->indices.push_back(6);
    this->indices.push_back(0);     this->indices.push_back(7);     this->indices.push_back(4);
    this->indices.push_back(0);     this->indices.push_back(3);     this->indices.push_back(7);
    this->indices.push_back(4);     this->indices.push_back(6);     this->indices.push_back(5);
    this->indices.push_back(4);     this->indices.push_back(7);     this->indices.push_back(6);
    this->indices.push_back(0);     this->indices.push_back(2);     this->indices.push_back(3);
    this->indices.push_back(0);     this->indices.push_back(1);     this->indices.push_back(2);
    this->indices.push_back(0);     this->indices.push_back(5);     this->indices.push_back(1);
    this->indices.push_back(0);     this->indices.push_back(4);     this->indices.push_back(5);
    this->indices.push_back(3);     this->indices.push_back(6);     this->indices.push_back(7);
    this->indices.push_back(3);     this->indices.push_back(2);     this->indices.push_back(6);


    glGenBuffers(1, &(this->vertexbuffer));
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), &(this->vertices[0]), GL_STATIC_DRAW);
    
    glGenBuffers(1, &(this->elementbuffer));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()* sizeof(unsigned int), &(this->indices[0]) , GL_STATIC_DRAW);

    std::string textures[6] = {
        "../Textures/Skybox/px.png",
        "../Textures/Skybox/nx.png",
        "../Textures/Skybox/py.png",
        "../Textures/Skybox/ny.png",
        "../Textures/Skybox/pz.png",
        "../Textures/Skybox/nz.png"
    };
    std::copy(std::begin(textures), std::end(textures), std::begin(this->pathTextures));
}


void Skybox::drawSkybox(glm::mat4 Model, glm::mat4 Projection, glm::mat4 View) {
    glUseProgram(this->programID_Skybox);

    glUniformMatrix4fv(glGetUniformLocation(this->programID_Skybox,"model"),1,GL_FALSE,&Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(this->programID_Skybox,"projection"),1,GL_FALSE,&Projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(this->programID_Skybox,"view"),1,GL_FALSE,&View[0][0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
    glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);

    // Draw the triangles !
    glDrawElements(
                    GL_TRIANGLES,      // mode
                    this->indices.size(), // count
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
                    );

    glDisableVertexAttribArray(0);
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
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << this->pathTextures[i].c_str() << std::endl;
            stbi_image_free(data);
            return;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Axe x
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Axe y
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // Axe z
}

void Skybox::bindCubemap(GLenum TextureUnit, int unit){
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
    glUniform1i(glGetUniformLocation(this->programID_Skybox, "skyboxTexture"), unit);
}