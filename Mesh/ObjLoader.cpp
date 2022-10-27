#include "ObjLoader.h"

ObjLoader::ObjLoader(Shader& shader, const std::string objfiledir, std::string texfiledir)
	: VisualObject(shader)
{
	ReadFile(objfiledir);

	if (texfiledir != "")
	{
		mTexture = new Texture(texfiledir);
		bUseTex = true;
	}

	mMatrix = glm::mat4(1.0f);
	
}

ObjLoader::~ObjLoader()
{

}

void ObjLoader::init()
{
	VisualObject::init();
}

void ObjLoader::draw()
{
	if (bUseTex)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mTexture->id());
	}

	VisualObject::draw();
}

void ObjLoader::ReadFile(std::string filedir)
{

	std::vector<glm::fvec3> VertexPos;
	std::vector<glm::fvec2> VertexUV;
	std::vector<glm::fvec3> VertexNormal;

	std::vector<GLint> VertexPosIndicies;
	std::vector<GLint> VertexUVIndicies;
	std::vector<GLint> VertexNormalIndicies;


	std::stringstream ss;
	std::ifstream in_file(filedir);
	std::string line = "";
	std::string prefix = "";
	glm::vec3 temp_vec3;
	glm::vec2 temp_vec2;
	GLint temp_glint = 0;

	bool bNormal{ false };
	bool bUV{ false };

	//Check if file is found
	if (!in_file.is_open())
	{
		std::cout << "OBJLOADER COULD NOT OPEN FILE" << std::endl;
		return;
	}

	//Read one line at a time
	while (std::getline(in_file, line)) 
	{
		ss.clear();
		ss.str(line);
		ss >> prefix;


		if (prefix == "o")
		{
			ss >> mName;
		}
		else if (prefix == "v") //Vertex pos
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			VertexPos.push_back(temp_vec3);
		}
		else if (prefix == "vt")
		{
			ss >> temp_vec2.x >> temp_vec2.y;
			VertexUV.push_back(temp_vec2);

			if (!bUV)
				bUV = true;
		}
		else if (prefix == "vn")
		{
			ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
			VertexNormal.push_back(temp_vec3);

			if (!bNormal)
				bNormal = true;
		}
		else if (prefix == "f")
		{
			int i = 0;
			while (ss >> temp_glint) 
			{
				if (i == 0)
					VertexPosIndicies.push_back(temp_glint);
				else if (i == 1)
					VertexUVIndicies.push_back(temp_glint);
				else if (i == 2)
					VertexNormalIndicies.push_back(temp_glint);

				if (ss.peek() == '/')
				{
					if (bUV && bNormal)
						++i;

					else if (bNormal && !bUV)
					{
						if (i == 0)
							i = 2;
						else if (i == 2)
							i = 0;
					}

					
					ss.ignore(1, '/');
					
					if (ss.peek() == '/')
					{
						ss.ignore(1, '/');
					}
				}
				else if (ss.peek() == ' ')
				{

					if (bUV && bNormal)
					{
						++i;
					}
					else if (bNormal && !bUV)
					{
						if (i == 0)
							i = 2;
						else if (i == 2)
							i = 0;
					}

					ss.ignore(1, ' ');
				}

				if (i > 2)
					i = 0;
			}
		}

		mVertices.resize(VertexPosIndicies.size(), Vertex());

		for (size_t i = 0; i < mVertices.size(); i++)
		{
			mVertices[i].m_xyz[0] = VertexPos[VertexPosIndicies[i]-1].x; 
			mVertices[i].m_xyz[1] = VertexPos[VertexPosIndicies[i]-1].y; 
			mVertices[i].m_xyz[2] = VertexPos[VertexPosIndicies[i]-1].z;
			
			if (VertexUV.empty())
			{
				mVertices[i].m_uv[0] = 0.f; 
				mVertices[i].m_uv[1] = 1.f;
			}
			else
			{
				mVertices[i].m_uv[0] = VertexUV[VertexUVIndicies[i] - 1].x;
				mVertices[i].m_uv[1] = VertexUV[VertexUVIndicies[i] - 1].y;
			}

			if (VertexNormal.empty())
			{
				mVertices[i].m_normal[0] = 0.5; mVertices[i].m_normal[1] = 0.5; mVertices[i].m_normal[2] = 0.5;
			}
			else
			{
				mVertices[i].m_normal[0] = VertexNormal[VertexNormalIndicies[i] - 1].x;
				mVertices[i].m_normal[1] = VertexNormal[VertexNormalIndicies[i] - 1].y;
				mVertices[i].m_normal[2] = VertexNormal[VertexNormalIndicies[i] - 1].z;
			}
		}

		mIndices = VertexNormalIndicies;

	//Does it work?
	//std::cout << "Nr of verts: " << mVertices.size() << "\n";

	}

	std::cout << "Obj LOADED!" << "\n";

}
