#include <Voxel.hpp>

Voxel::Voxel(glm::vec3 position){
    this->backBottomLeftCorner = position;
    this->buildVoxel();
}

void Voxel::buildVoxel(){
    std::cout << "Construction des faces du voxel\n";

    float size = 1.0f; // Voxel de taille 1
    int resolution = 16; // Nombre de ligne et de colonne sur la face d'un voxel
    float step = size / resolution;

    for (int i = 0 ; i < 6 ; i++){
        Face *f = new Face;

        f->faceId = i;

        for (int h = 0; h < resolution + 1; h++) {
            for (int w = 0; w < resolution + 1; w++) {
                float x, y, z;

                if (i < 2){ // Faces nx et px
                    x = this->backBottomLeftCorner[0] + (float)w * step;
                    y = this->backBottomLeftCorner[1] + i * size;
                    z = this->backBottomLeftCorner[2] + (float)h * step; 
                }else if (i < 4){ // Faces ny et py
                    x = this->backBottomLeftCorner[0] + (float)w * step;
                    y = this->backBottomLeftCorner[1] + (float)h * step; 
                    z = this->backBottomLeftCorner[2] + (i-2) * size;
                }else if (i < 6){ // Face nz et pz
                    x = this->backBottomLeftCorner[0] + (i-4) * size;
                    y = this->backBottomLeftCorner[1] + (float)w * step;
                    z = this->backBottomLeftCorner[2] + (float)h * step; 
                }
                f->uv.push_back(glm::vec2((float)(h/((float)(resolution+1))),(float)(w/((float)(resolution+1)))));
                f->vertices.push_back(glm::vec3(x,y,z));
            }
        }

        for (int h = 0; h < resolution ; h++) {
            for (int w = 0; w < resolution ; w++) {
                unsigned short index = h * (resolution + 1) + w;
                f->indicesTriangles.push_back(index);
                f->indicesTriangles.push_back(index + 1);
                f->indicesTriangles.push_back(index + resolution + 1);
                f->indicesTriangles.push_back(index + 1);
                f->indicesTriangles.push_back(index + resolution + 2);
                f->indicesTriangles.push_back(index + resolution + 1);
            }
        }

        this->facesVoxel.push_back(f);
    }
}

void Voxel::loadVoxel(){
    for (int i = 0; i < 6 ; i++){
        glGenBuffers(1, &(this->facesVoxel[i]->vertexbuffer));
        glBindBuffer(GL_ARRAY_BUFFER, this->facesVoxel[i]->vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, (this->facesVoxel[i])->vertices.size() * sizeof(glm::vec3), &(this->facesVoxel[i]->vertices[0]), GL_STATIC_DRAW);
        
        glGenBuffers(1, &(this->facesVoxel[i]->elementbuffer));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->facesVoxel[i]->elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->facesVoxel[i])->indicesTriangles.size()* sizeof(unsigned short), &(this->facesVoxel[i]->indicesTriangles[0]) , GL_STATIC_DRAW);

        glGenBuffers(1, &(this->facesVoxel[i]->uvbuffer));
        glBindBuffer(GL_ARRAY_BUFFER, this->facesVoxel[i]->uvbuffer);
        glBufferData(GL_ARRAY_BUFFER, this->facesVoxel[i]->uv.size() * sizeof(glm::vec2), &(this->facesVoxel[i]->uv[0]) , GL_STATIC_DRAW);
    }
}

void Voxel::drawVoxel(GLuint programID){
    for (int i = 0; i < 6 ; i++){

		glUniform1i(glGetUniformLocation(programID, "objectID"),i);
		
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, this->facesVoxel[i]->vertexbuffer);
        glVertexAttribPointer(
                        0,                  // attribute
                        3,                  // size
                        GL_FLOAT,           // type
                        GL_FALSE,           // normalized?
                        0,                  // stride
                        (void*)0            // array buffer offset
                        );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->facesVoxel[i]->elementbuffer);

        // Draw the triangles !
        glDrawElements(
                        GL_TRIANGLES,      // mode
                        this->facesVoxel[i]->indicesTriangles.size(), // count
                        GL_UNSIGNED_SHORT,   // type
                        (void*)0           // element array buffer offset
                        );

        glDisableVertexAttribArray(0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, this->facesVoxel[i]->uvbuffer);
        glVertexAttribPointer(
                    1,                  // attribute
                    2,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    2*sizeof(float),    // stride
                    (void*)0            // array buffer offset
                    );
    }
}