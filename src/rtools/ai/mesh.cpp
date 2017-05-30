#include <rtools\ai\mesh.h>
#include <rtools\utils.h>

Material* defaultMaterial;

void Mesh::setDefaultMaterial(Material* material){
	defaultMaterial = material;
}

Mesh::Mesh(const aiScene* scene, const struct aiMesh* mesh) : scene(scene), mesh(mesh){
	numPolygons = mesh->mNumFaces;
	numVertices = (mesh->mNumFaces) * 3;

	//material = defaultMaterial;

	material = new Material(scene->mMaterials[mesh->mMaterialIndex]);
	//

	this->buf = 0;
	this->bCount = 0;
}

void Mesh::genBuffers(GLuint count){
	delete[](this->buf);

	this->buf = new GLuint[count];
	glGenBuffers(count, (this->buf));
	this->bCount = count;
}

void Mesh::init(){
	/*
	for (int t = 0; t < mesh->mNumFaces; ++t) {
		const struct aiFace* face = &mesh->mFaces[t];
		GLenum face_mode;

		switch (face->mNumIndices) {
		case 1: face_mode = GL_POINTS; break;
		case 2: face_mode = GL_LINES; break;
		case 3: face_mode = GL_TRIANGLES; break;
		default: face_mode = GL_POLYGON; break;
		}

		glBegin(face_mode);

		for (int i = 0; i < face->mNumIndices; i++) {
			int index = face->mIndices[i];
			if (mesh->mColors[0] != NULL)
				glColor4fv((GLfloat*)&mesh->mColors[0][index]);
			if (mesh->mNormals != NULL)
				glNormal3fv(&mesh->mNormals[index].x);
			glVertex3fv(&mesh->mVertices[index].x);
		}

		glEnd();
	}
	*/
	boundsMin = GsVec(1e10f, 1e10f, 1e10f);
	boundsMax = GsVec(-1e10f, -1e10f, -1e10f);

	float* p = new float[numVertices * 3];
	float* n = new float[numVertices * 3];
	float* uvs = new float[numVertices * 2];

	float* _p = p;
	float* _n = n;
	float* _uvs = uvs;

	int newNumPolygons = 0;

	printf("Loading %d polygons!\n", numPolygons);

	for (std::size_t t = 0; t < mesh->mNumFaces; ++t) {
		const struct aiFace* face = &(mesh->mFaces[t]);

		if (face->mNumIndices == 3){
			for (std::size_t i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];

				*(p++) = mesh->mVertices[index].x;
				*(p++) = mesh->mVertices[index].y;
				*(p++) = mesh->mVertices[index].z;

				boundsMin.x = min(mesh->mVertices[index].x, boundsMin.x);
				boundsMin.y = min(mesh->mVertices[index].y, boundsMin.y);
				boundsMin.z = min(mesh->mVertices[index].z, boundsMin.z);

				boundsMax.x = max(mesh->mVertices[index].x, boundsMax.x);
				boundsMax.y = max(mesh->mVertices[index].y, boundsMax.y);
				boundsMax.z = max(mesh->mVertices[index].z, boundsMax.z);

				if (mesh->mColors[0] != NULL){
					printf("Vertex color not supported!");
				}
				if (mesh->mNormals == NULL){
					printf("Vertex normals not defined! TODO: use flat shading for this polygon!");
				}else{
					*(n++) = mesh->mNormals[index].x;
					*(n++) = mesh->mNormals[index].y;
					*(n++) = mesh->mNormals[index].z;
				}
				if (mesh->HasTextureCoords(0)){
					*(uvs++) = mesh->mTextureCoords[0][index].x;
					*(uvs++) = 1 - mesh->mTextureCoords[0][index].y;
				}
			}

			newNumPolygons++;
		}else{
			printf("Unexpected polygon with %d vertices!\n", face->mNumIndices);
		}
	}

	numPolygons = newNumPolygons;
	numVertices = newNumPolygons * 3;

	genBuffers(3);
	glGenVertexArrays(1, &vao);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _p, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), _n, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(float), _uvs, GL_STATIC_DRAW);

	delete _p;
	delete _n;
	delete _uvs;

	// Set up VAO:
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // uvs
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Mesh::draw(const GsMat& tr, const GsMat& pr){
	GLProgram *program = material->program;

	program->bind();
	program->setUniformMat4f("vTransf", tr.e);
	program->setUniformMat4f("vProj", pr.e);

	GLTexture *tex = material->tex;

	if (tex == NULL) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}else{
		tex->bind(GL_TEXTURE0);
	}
	program->setUniform1i("Texture0", 0);

	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	glBindVertexArray(0);
}

float Mesh::intersect(const GsVec& start, const GsVec& end){
	if (!RTools::intersectLineSegmentAABB(start, end, boundsMin, boundsMax)){
		return -1;
	}

	float min = -1;
	for (std::size_t t = 0; t < mesh->mNumFaces; ++t) {
		const struct aiFace* face = &(mesh->mFaces[t]);

		if (face->mNumIndices == 3){
			GsVec p0, p1, p2;
			int loc = face->mIndices[0];

			p0.x = mesh->mVertices[loc].x;
			p0.y = mesh->mVertices[loc].y;
			p0.z = mesh->mVertices[loc].z;

			loc = face->mIndices[1];

			p1.x = mesh->mVertices[loc].x;
			p1.y = mesh->mVertices[loc].y;
			p1.z = mesh->mVertices[loc].z;

			loc = face->mIndices[2];

			p2.x = mesh->mVertices[loc].x;
			p2.y = mesh->mVertices[loc].y;
			p2.z = mesh->mVertices[loc].z;

			float val = RTools::intersectRayTriangle(start, end, p0, p1, p2);
			if (val >= 0){
				if (val < min || min < 0){
					min = val;
				}
			}
		}
	}
	return min;
}