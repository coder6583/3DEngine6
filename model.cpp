#include "model.hpp"

Model::Model(Shape3DType Type3D, float size, glm::vec3 pos, float mass, float color[3])
{
			this -> Type3D = Type3D;
				this -> loadCube(size, pos, mass, color);
				this -> myColor.x = color[0];
				this -> myColor.y = color[1];
				this -> myColor.z = color[2];
 				this -> name = "Cube";
				this -> original = "Cube";
				this -> SetOriginalTransform();
				this -> path = "/Cube";
	this -> isSelected = false;
}

Model::Model(Shape3DType Type3D, glm::vec3 size)
{
	this -> Type3D = Type3D;
	this -> loadGround(size);

	int imageWidth, imageHeight;
	unsigned char *image = SOIL_load_image("grid.png", &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
// 	std::cout << imageWidth << " " << imageHeight << std::endl;
	glGenTextures(1, &this -> groundTex);

	glBindTexture(GL_TEXTURE_2D, this -> groundTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	this -> name = "Ground";
	this -> original = "Ground";
	this -> path = "/Ground";
	this -> isSelected = false;
}

void Model::Run()
{
	for(const auto &com : this -> interpreter.GetCom())
	{
		if(com.first == COMMAND::MOVE)
		{
// 			this -> Move(glm::vec3(com.second.x, com.second.z, com.second.y));
		}
		else if(com.first == COMMAND::MOVETO)
		{
// 			this -> MoveTo(glm::vec3(com.second.x, com.second.y, com.second.z));
		}
		else if(com.first == COMMAND::ROTATE)
		{
// 			this -> Rotate(com.second.x, glm::vec3(com.second.y, com.second.z, com.second.w));
		}
		else if(com.first == COMMAND::SCALE)
		{
// 			this -> Scale(glm::vec3(com.second.x, com.second.y, com.second.z));
		}
	}

}

void Model::Draw(Shader shader)
{
	if(this -> Type3D == Shape3DType::MODEL)
	{
		for(GLuint i = 0; i < this -> meshes.size(); i++)
			this -> meshes[i].Draw(shader);
	}
	else if(this -> Type3D == Shape3DType::CUBE)
	{
		glBindVertexArray(this -> VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	else if(this -> Type3D == Shape3DType::GROUND)
	{
		glBindTexture(GL_TEXTURE_2D, this -> groundTex);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader.Program, "grid"), 0);
		glBindVertexArray(this -> VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
}

void Model::loadModel(std::string path)
{
	std::cout << "a" << std::endl;
   b3BulletDefaultFileIO fileIO;
   GLInstanceGraphicsShape *gfxShape = LoadMeshFromSTL(path.c_str(), &fileIO);
   const GLInstanceVertex &v = gfxShape -> m_vertices -> at(0);
   btConvexHullShape *shape = new btConvexHullShape((const btScalar*)(&v.xyzw[0]), gfxShape -> m_numvertices, sizeof(GLInstanceVertex));
	std::cout << gfxShape-> m_numvertices << std::endl;
	colShape = shape;
   // Read file via ASSIMP
   Assimp::Importer importer;
   const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
   // Check for errors
   if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
   {
       std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
       return;
   }
   // Retrieve the directory path of the filepath
   this->directory = path.substr(0, path.find_last_of('/'));
   std::stringstream ss(path);
   while(std::getline(ss, this -> name, '/'));
   this -> original = this -> name;

   // Process ASSIMP's root node recursively
   this->processNode(scene->mRootNode, scene);
}

void Model::loadCube(float size, glm::vec3 pos, float mass, float color[3])
{
	this -> size = size;
	GLfloat vertices[] =
	{
		-size, -size, -size,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     size, -size, -size,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     size,  size, -size,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     size,  size, -size,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -size,  size, -size,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -size, -size, -size,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -size, -size,  size,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     size, -size,  size,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     size,  size,  size,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     size,  size,  size,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -size,  size,  size,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -size, -size,  size,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -size,  size,  size, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -size,  size, -size, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -size, -size, -size, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -size, -size, -size, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
    -size, -size,  size, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -size,  size,  size, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

     size,  size,  size,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     size,  size, -size,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     size, -size, -size,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     size, -size, -size,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     size, -size,  size,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     size,  size,  size,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

    -size, -size, -size,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     size, -size, -size,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     size, -size,  size,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     size, -size,  size,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -size, -size,  size,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -size, -size, -size,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

    -size,  size, -size,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
     size,  size, -size,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     size,  size,  size,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     size,  size,  size,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    -size,  size,  size,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -size,  size, -size,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

	this -> colShape = new btBoxShape(btVector3(btScalar(size), btScalar(size), btScalar(size)));
	this -> trans.setIdentity();

	this -> mass = btScalar(mass);
	this -> isDynamic = (mass != 0.f);

	this -> localInertia = btVector3(0, 0, 0);
	if(this -> isDynamic)
		colShape -> calculateLocalInertia(mass, localInertia);
	this -> trans.setOrigin(btVector3(pos.x, pos.y, pos.z));

	this -> myMotionState = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
	this -> originalInfo = new btRigidBody::btRigidBodyConstructionInfo(rbInfo);
	this -> myMotionState -> getWorldTransform(this -> originalTrans);
	this -> body = new btRigidBody(rbInfo);

	glGenVertexArrays(1, &this -> VAO);
	glGenBuffers(1, &this -> VBO);

	glBindVertexArray(this -> VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this -> VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid *)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

// 	glBindBuffer(GL_ARRAY_BUFFER, this -> VBO);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(barycentricCoords), barycentricCoords, GL_STATIC_DRAW);

// 	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid *)0);
// 	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}
void Model::loadGround(glm::vec3 size)
{
	GLfloat vertices[] =
	{
		-size.x, -0.5f, -size.z,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     size.x, -0.5f, -size.z,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     size.x,  0.0f, -size.z,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     size.x,  0.0f, -size.z,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -size.x,  0.0f, -size.z,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -size.x, -0.5f, -size.z,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -size.x, -0.5f,  size.z,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     size.x, -0.5f,  size.z,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     size.x,  0.0f,  size.z,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     size.x,  0.0f,  size.z,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -size.x,  0.0f,  size.z,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -size.x, -0.5f,  size.z,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -size.x,  0.0f,  size.z, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -size.x,  0.0f, -size.z, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -size.x, -0.5f, -size.z, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -size.x, -0.5f, -size.z, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -size.x, -0.5f,  size.z, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -size.x,  0.0f,  size.z, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     size.x,  0.0f,  size.z,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     size.x,  0.0f, -size.z,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     size.x, -0.5f, -size.z,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     size.x, -0.5f, -size.z,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     size.x, -0.5f,  size.z,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     size.x,  0.0f,  size.z,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -size.x, -0.5f, -size.z,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     size.x, -0.5f, -size.z,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     size.x, -0.5f,  size.z,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     size.x, -0.5f,  size.z,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -size.x, -0.5f,  size.z,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -size.x, -0.5f, -size.z,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -size.x,  0.0f, -size.z,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     size.x,  0.0f, -size.z,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     size.x,  0.0f,  size.z,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     size.x,  0.0f,  size.z,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -size.x,  0.0f,  size.z,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -size.x,  0.0f, -size.z,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	glGenVertexArrays(1, &this -> VAO);
	glGenBuffers(1, &this -> VBO);

	glBindVertexArray(this -> VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this -> VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // Process each mesh located at the current node
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            // The node object only contains indices to index the actual objects in the scene.
            // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(this->processMesh(mesh, scene));
        }
        // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }

    }

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // Data to fill
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
// 		  modelShape = new btConvexHullShape();
        // Walk through each of the mesh's vertices
//         std::cout << mesh -> mNumVertices << std::endl;
        for(GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.position = vector;
//             std::cout << mesh->mVertices[i].x << " " << mesh->mVertices[i].y << " " << mesh->mVertices[i].z << std::endl;
//             modelShape -> addPoint(btVector3(mesh -> mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
            // Texture Coordinates
            if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.texCoords = vec;
            }
            else
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            vertex.barycentric = glm::vec3(0, 0, 0);
            vertices.push_back(vertex);
        }
        // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector
//             std::cout << face.mNumIndices << std::endl;
            for(GLuint j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
                switch(j)
                {
					case 0:
						vertices.at((3 * i) + j).barycentric = glm::vec3(1, 0, 0);
						break;
					case 1:
						vertices.at((3 * i) + j).barycentric = glm::vec3(0, 1, 0);
						break;
					case 2:
						vertices.at((3 * i) + j).barycentric = glm::vec3(0, 0, 1);
						break;
					 }
            }
        }
        // Process materials
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
            // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
            // Same applies to other texture as the following list summarizes:
            // Diffuse: texture_diffuseN
            // Specular: texture_specularN
            // Normal: texture_normalN

            // 1. Diffuse maps
            std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. Specular maps
            std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
// 		  colShape = modelShape;
			this -> trans.setIdentity();

// 			this -> mass = btScalar(1.f);
			this -> isDynamic = (mass != 0.f);

			this -> localInertia = btVector3(0, 0, 0);
			if(this -> isDynamic)
				colShape -> calculateLocalInertia(mass, localInertia);
			this -> trans.setOrigin(btVector3(firstPos.x, firstPos.y, firstPos.z));

			this -> myMotionState = new btDefaultMotionState(trans);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			this -> myMotionState -> getWorldTransform(this -> originalTrans);
			this -> body = new btRigidBody(rbInfo);
			this -> originalInfo = new btRigidBody::btRigidBodyConstructionInfo(rbInfo);
        // Return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
        }
        return textures;
    }

GLint TextureFromFile(const char* path, std::string directory)
{
     //Generate texture ID and load texture data
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

void Model::MVPUniform(Shader shader, Camera camera, int width, int height)
{
	GLuint modelLoc = glGetUniformLocation(shader.Program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this -> model));
	GLuint viewLoc = glGetUniformLocation(shader.Program, "view");
	this -> view = glm::mat4(1.0f);
	this -> view = camera.getViewMatrix();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this -> view));
	GLuint projLoc = glGetUniformLocation(shader.Program, "projection");
	this -> projection = glm::mat4(1.0f);
	this -> projection = glm::perspective(glm::radians(camera.fov), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(this -> projection));
}

void Model::UniformMat4(Shader shader, std::string uniName, glm::mat4 matrix)
{
	GLuint uniformLoc = glGetUniformLocation(shader.Program, uniName.c_str());
	glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Model::Uniform3f(Shader shader, std::string uniName, glm::vec3 vector)
{
	GLuint uniformLoc = glGetUniformLocation(shader.Program, uniName.c_str());
	glUniform3f(uniformLoc, vector.x, vector.y, vector.z);
}

void Model::Uniform1bool(Shader shader, std::string uniName, bool i)
{
	GLuint uniformLoc = glGetUniformLocation(shader.Program, uniName.c_str());
	glUniform1i(uniformLoc, i);
}

void Model::MoveTo(glm::vec3 nextPos)
{
	glm::vec3 offset = nextPos - currentPos;
	currentPos = nextPos;
	this -> model = glm::translate(this -> model, offset);
}

void Model::Move(glm::vec3 offset)
{
	this -> model = glm::translate(this -> model, offset);
}

void Model::Rotate(btVector3 axis, float radians)
{
	btTransform t;
	this -> body -> getMotionState() -> getWorldTransform(t);
	btQuaternion rotation = btQuaternion(axis, radians);
	t.setRotation(rotation);
	this -> body -> getMotionState() -> setWorldTransform(t);
}

void Model::Scale(glm::vec3 scaleRatio)
{
	this -> model = glm::scale(this -> model, scaleRatio);
}
