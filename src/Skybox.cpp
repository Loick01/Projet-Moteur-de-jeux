#include <Skybox.hpp>

Skybox::Skybox(){
    this->programID_skybox = LoadShaders("../shaders/skybox_vertex.vert", "../shaders/skybox_fragment.frag");

    // Sommet de la skybox
    float verts[108] = {
        // positions        
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    std::copy(std::begin(verts), std::end(verts), std::begin(this->skyboxVertices));

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

    unsigned int skyboxVBO;
    glGenVertexArrays(1, &(this->skyboxVAO));
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(this->skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->skyboxVertices), &(this->skyboxVertices), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    this->loadCubemap();
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
            // std::cout << "Text " << i << " : width = " << width << ", height = " << height << ", nr = " << nrChannels << "\n";
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
    //glUniform1i(glGetUniformLocation(this->programID_skybox, "skybox"), unit);
}

void Skybox::useShader(){
    glUseProgram(this->programID_skybox);
}

GLuint Skybox::getProgramID(){
    return this->programID_skybox;
}

unsigned int Skybox::getVAO(){
    return this->skyboxVAO;
}