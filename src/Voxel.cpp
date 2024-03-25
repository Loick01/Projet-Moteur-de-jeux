#include <Voxel.hpp>

Voxel::Voxel(glm::vec3 position){
    this->backBottomLeftCorner = position;
    this->buildVoxel();
}

void Voxel::buildVoxel(){
    //std::cout << "Construction des faces du voxel\n";

    float size = 1.0f; // Voxel de taille 1
    int resolution = 1; // Nombre de ligne et de colonne sur la face d'un voxel
    float step = size / resolution;

    for (int i = 0 ; i < 6 ; i++){
        Face *f = new Face;

        f->faceId = i;

        for (int h = 0; h < resolution + 1; h++) {
            for (int w = 0; w < resolution + 1; w++) {
                float x, y, z;
                
                int n = (i%2 == 0 ? 1 : -1);

                if (i < 2){ // Faces bottom et top
                    x = this->backBottomLeftCorner[0] + (float)w * step;
                    y = this->backBottomLeftCorner[1] + i * size;
                    z = this->backBottomLeftCorner[2] + i*step + ((float)h * step)*n; 
                }else if (i == 2){ // Faces back
                    x = this->backBottomLeftCorner[0] + step * (1-w);
                    y = this->backBottomLeftCorner[1] + (float)h * step; 
                    z = this->backBottomLeftCorner[2];
                }else if (i == 3){ // Faces front
                    x = this->backBottomLeftCorner[0] + (float)w * step;
                    y = this->backBottomLeftCorner[1] + (float)h * step; 
                    z = this->backBottomLeftCorner[2] + size;
                }else if (i == 4){ // Face left
                    x = this->backBottomLeftCorner[0] + (i-4) * size;
                    y = this->backBottomLeftCorner[1] + (float)h * step;
                    z = this->backBottomLeftCorner[2] + (float)w * step; 
                }else if (i == 5){ // Face right
                    x = this->backBottomLeftCorner[0] + (i-4) * size;
                    y = this->backBottomLeftCorner[1] + (float)h * step;
                    z = this->backBottomLeftCorner[2] + step * (1-w); 
                }
                f->uv.push_back(glm::vec2(w,1-h));
                f->vertices.push_back(glm::vec3(x,y,z));
            }
        }

        for (int h = 0; h < resolution ; h++) {
            for (int w = 0; w < resolution ; w++) {
                unsigned short index = h * (resolution + 1) + w;
                f->indicesTriangles.push_back(index + resolution + 1);
                f->indicesTriangles.push_back(index);
                f->indicesTriangles.push_back(index + resolution + 2);
                f->indicesTriangles.push_back(index + resolution + 2);
                f->indicesTriangles.push_back(index);
                f->indicesTriangles.push_back(index + 1);
                /*
                if (i == 1 || i == 2 || i == 4){ // Face impaire
                    f->indicesTriangles.push_back(index);
                    f->indicesTriangles.push_back(index + resolution + 1);
                    f->indicesTriangles.push_back(index + 1);
                    f->indicesTriangles.push_back(index + 1);
                    f->indicesTriangles.push_back(index + resolution + 1);
                    f->indicesTriangles.push_back(index + resolution + 2);
                }else if (i == 0 || i == 3 ||i == 5){ // Face paire
                    f->indicesTriangles.push_back(index + resolution + 1);
                    f->indicesTriangles.push_back(index);
                    f->indicesTriangles.push_back(index + resolution + 2);
                    f->indicesTriangles.push_back(index + resolution + 2);
                    f->indicesTriangles.push_back(index);
                    f->indicesTriangles.push_back(index + 1);
                }
                */
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
    }
}

void Voxel::drawVoxel(GLuint programID){
    for (int i = 0; i < 6 ; i++){

		glUniform1i(glGetUniformLocation(programID, "objectID"),this->facesVoxel[i]->faceId);
		
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
    }
}