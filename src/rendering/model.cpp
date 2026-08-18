#include "rendering/model.h"
#include "rendering/material.h"

Model::Model(const char* filePath){
    //load model call
    loadModel(filePath);
}

void Model::draw(Shader& shader){
    //draw all meshes
    for(int i = 0; i < meshes.size(); i++){
        meshes[i].Draw(shader);
    }
}

void Model::drawOutlined(Shader& objectShader, Shader& outlineShader, glm::vec3 color, float outlineSize, glm::vec3 position){
    //enable tests
    glEnable(GL_DEPTH_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    //pass all fragments, replace with ref, and enable writing
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);
    draw(objectShader);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.02f + outlineSize, 0.02f + outlineSize, 0.02f + outlineSize)); 
    outlineShader.setMatrix4f("model", model);
    outlineShader.set3f("lightColor", color);

    //disable writing (read only)
    glStencilMask(0x00);
    //write if it is outside the original
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    draw(outlineShader);

    //reset stencil test
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
}

void Model::loadModel(std::string path){
    //load scene object
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

    //error checking
    if(!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE){
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString();
        return;
    }

    //set directory folder
    directory = path.substr(0, path.find_last_of('/'));

    //process nodes
    processNode(scene->mRootNode, scene);
    std::cout << "Model loaded: " << path << "\n";
}

void Model::processNode(aiNode* node, const aiScene* scene){
    //process current node meshes
    for(int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    //process sub nodes
    for(int i = 0; i < node->mNumChildren; i++){
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene){
    //create local vectors
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Mesh::Texture> textures;

    //set vertices
    for(int i = 0; i < mesh->mNumVertices; i++){
        Mesh::Vertex vertex;
        //set position
        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        //set normals
        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        //set texture coords
        if(mesh->mTextureCoords[0] != 0){
            //set to texture coord
            vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoord.y = mesh->mTextureCoords[0][i].y;
        } else {
            //set to zero
            vertex.texCoord.x = 0;
            vertex.texCoord.y = 0;
        }
        vertices.push_back(vertex);
    }

    //set indices
    for(int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    //set textures
    if(mesh->mMaterialIndex >= 0){
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Mesh::Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<Mesh::Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    Mesh m = Mesh(vertices, textures, indices);
    return m;
}

std::vector<Mesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName){
    std::vector<Mesh::Texture> textures;
    for(int i = 0; i < mat->GetTextureCount(type); i++){
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for(int j = 0; j < loadedTextures.size(); j++){
            if(std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0){
                textures.push_back(loadedTextures[j]);
                skip = true;
            }
        }
        if(skip == false){
            Mesh::Texture t;
            t.ID = TextureFromFile(str.C_Str(), directory);
            t.type = typeName;
            t.path = str.C_Str();
            loadedTextures.push_back(t);
            textures.push_back(t);
        }
    }

    return textures;
}

unsigned int Model::TextureFromFile(const char* path, const std::string& directory){
    std::string fileName = directory + '/' + path;
    unsigned int texture;

    //create texture
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int tWidth, tHeight, nrChannels;
    std::cout << "Loading texture from path: " << fileName << std::endl;
    unsigned char* tData = stbi_load(fileName.c_str(), &tWidth, &tHeight, &nrChannels, 0);
    
    GLenum format;
    if(nrChannels == 1){
        format = GL_RED;
    } else if(nrChannels == 3){
        format = GL_RGB;
    } else if(nrChannels == 4){
        format = GL_RGBA;
    }
    if(tData != 0){
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, format, tWidth, tHeight, 0, format, GL_UNSIGNED_BYTE, tData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tData);
    } else {
        std::cout << "FAILED TO LOAD TEXTURE" << std::endl;
        stbi_image_free(tData);
    }

    //unbind for later use
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    return texture;
}