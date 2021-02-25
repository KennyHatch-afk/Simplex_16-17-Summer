#include "MyMesh.h"

void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	float fValue = a_fHeight * 0.5f;

	//Add top point
	vector3 point0(0, fValue, 0);

	//Add bottom middle point
	vector3 point1(0, -fValue, 0);

	//Calculate distance between points
	float dValue = 360 / a_nSubdivisions;

	//Make array for base points
	std::vector<vector3> basePoints;

	//Add base points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, -fValue, zValue);
		basePoints.push_back(tempPoint);
	}

	//Make Cone
	for (int e = 0; e < basePoints.size() - 1; e++)
	{
		//Make Side Tris
		if (e == basePoints.size() - 1)
		//Wrap around if at last input
		{
			AddTri(basePoints[0], basePoints[e], point0);
		}
		else
		{
			AddTri(basePoints[e], basePoints[e + 1], point0);
		}

		//Make Base
		if (e == basePoints.size() - 1)
			//Wrap around if at last input
		{
			AddTri(basePoints[0], basePoints[e], point1);
		}
		else
		{
			AddTri(basePoints[e + 1], basePoints[e], point1);
		}
	}

	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	float fValue = a_fHeight * 0.5f;

	//Add top middle point
	vector3 point0(0, fValue, 0);

	//Add bottom middle point
	vector3 point1(0, -fValue, 0);

	//Calculate distance between points
	float dValue = 360 / a_nSubdivisions;

	//Make array for top points
	std::vector<vector3> topPoints;

	//Make array for base points
	std::vector<vector3> basePoints;

	//Add top points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, fValue, zValue);
		topPoints.push_back(tempPoint);
	}

	//Add base points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, -fValue, zValue);
		basePoints.push_back(tempPoint);
	}

	//Make top
	for (int e = 0; e < topPoints.size() - 1; e++)
	{
		if (e == topPoints.size() - 1)
			//Wrap around if at last input
		{
			AddTri(topPoints[e], topPoints[0], point0);
		}
		else
		{
			AddTri(topPoints[e], topPoints[e + 1], point0);
		}
	}

	//Make base
	for (int e = 0; e < basePoints.size() - 1; e++)
	{
		if (e == basePoints.size() - 1)
			//Wrap around if at last input
		{
			AddTri(basePoints[0], basePoints[e], point1);
		}
		else
		{
			AddTri(basePoints[e + 1], basePoints[e], point1);
		}
	}

	//Make walls
	for (int y = 0; y < topPoints.size() - 1; y++)
	{
		if (y == topPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(basePoints[y], basePoints[0], topPoints[y], topPoints[0]);
		}
		else
		{
			AddQuad(basePoints[y], basePoints[y + 1], topPoints[y], topPoints[y + 1]);
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	float fValue = a_fHeight * 0.5f;

	//Calculate distance between points
	float dValue = 360 / a_nSubdivisions;

	//Make array for top outer points
	std::vector<vector3> topOutPoints;

	//Make array for top inner points
	std::vector<vector3> topInPoints;

	//Make array for base outer points
	std::vector<vector3> baseOutPoints;

	//Make array for base inner points
	std::vector<vector3> baseInPoints;

	//Add top outer points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fOuterRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fOuterRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, fValue, zValue);
		topOutPoints.push_back(tempPoint);
	}

	//Add top inner points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fInnerRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fInnerRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, fValue, zValue);
		topInPoints.push_back(tempPoint);
	}

	//Add base outer points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fOuterRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fOuterRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, -fValue, zValue);
		baseOutPoints.push_back(tempPoint);
	}

	//Add base inner points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fInnerRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fInnerRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, -fValue, zValue);
		baseInPoints.push_back(tempPoint);
	}

	//Make top
	for (int e = 0; e < topOutPoints.size() - 1; e++)
	{
		if (e == topOutPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(topOutPoints[e], topOutPoints[0], topInPoints[e], topInPoints[0]);
		}
		else
		{
			AddQuad(topOutPoints[e], topOutPoints[e + 1], topInPoints[e], topInPoints[e + 1]);
		}
	}

	//Make base
	for (int e = 0; e < baseOutPoints.size() - 1; e++)
	{
		if (e == baseOutPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(baseOutPoints[0], baseOutPoints[e], baseInPoints[0], baseInPoints[e]);
		}
		else
		{
			AddQuad(baseOutPoints[e + 1], baseOutPoints[e], baseInPoints[e + 1], baseInPoints[e]);
		}
	}

	//Make outer walls
	for (int y = 0; y < topOutPoints.size() - 1; y++)
	{
		if (y == topOutPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(baseOutPoints[y], baseOutPoints[0], topOutPoints[y], topOutPoints[0]);
		}
		else
		{
			AddQuad(baseOutPoints[y], baseOutPoints[y + 1], topOutPoints[y], topOutPoints[y + 1]);
		}
	}

	//Make inner walls
	for (int y = 0; y < topInPoints.size() - 1; y++)
	{
		if (y == topInPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(baseInPoints[0], baseInPoints[y], topInPoints[0], topInPoints[y]);
		}
		else
		{
			AddQuad(baseInPoints[y + 1], baseInPoints[y], topInPoints[y + 1], topInPoints[y]);
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code

	//Calculate distance between A points
	float dAValue = 360 / a_nSubdivisionsA;

	//Calculate distance between B points
	float dBValue = 360 / a_nSubdivisionsB;

	//Calculate radius in the ring
	float rValue = (a_fOuterRadius - a_fInnerRadius) / 2;

	//Calculate mid distance of torus ring
	float mValue = rValue + a_fInnerRadius;

	//A collection of rings
	std::vector<std::vector<vector3>> rings;

	for (int u = 0; u < a_nSubdivisionsA; u++)	//Loop through torus ring
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentUDegree = dAValue * u;

		//Convert degrees to radians for input into sin / cos
		currentUDegree = (currentUDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		std::vector<vector3> tempPoints;	//Set up ring

		for (int v = 0; v < a_nSubdivisionsB; v++) //Loop into torus ring
		{
			//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
			float currentVDegree = dBValue * v;

			//Convert degrees to radians for input into sin / cos
			currentVDegree = (currentVDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

			vector3 tempPoint(
				(mValue + rValue * cos(currentVDegree)) * cos(currentUDegree),
				(mValue + rValue * cos(currentVDegree)) * cos(currentUDegree),
				rValue * sin(currentVDegree)
			);

			tempPoints.push_back(tempPoint);	//Store variables in ring
		}
		rings.push_back(tempPoints);
	}

	//Make walls
	for (int y = 0; y < rings.size() - 1; y++)
	{
		for (int f = 0; f < rings[y].size() - 1; f++)
		{
			if (y == rings.size() - 1)
				//Wrap around if at last input
			{
				if (f == rings[y].size() - 1)
				{
					AddQuad(rings[y][f], rings[y][0], rings[0][f], rings[0][0]); //y and f are on last input
				}
				else
				{
					AddQuad(rings[y][f], rings[y][f + 1], rings[0][f], rings[0][f + 1]); //y is but f is not
				}
			}
			else
			{
				if (f == rings[y].size() - 1)
				{
					AddQuad(rings[y][f], rings[y][0], rings[y + 1][f], rings[y + 1][0]); //f is but y is not
				}
				else
				{
					AddQuad(rings[y][f], rings[y][f + 1], rings[y + 1][f], rings[y + 1][f + 1]); //neither are on last input
				}
			}
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 10)
		a_nSubdivisions = 10;

	Release();
	Init();

	// Replace this with your code
	float fValue = a_fRadius;

	//Add top middle point
	vector3 point0(0, fValue, 0);

	//Add bottom middle point
	vector3 point1(0, -fValue, 0);

	//Calculate distance between points
	float dValue = 360 / a_nSubdivisions;

	//Make array for middle points
	std::vector<vector3> midPoints;

	//Make array for top 3/4 points
	std::vector<vector3> topThreePoints;

	//Make array for top half points
	std::vector<vector3> topHalfPoints;

	//Make array for top quarter points
	std::vector<vector3> topQuartPoints;

	//Make array for base 3/4 points
	std::vector<vector3> baseThreePoints;

	//Make array for base half points
	std::vector<vector3> baseHalfPoints;

	//Make array for base quarter points
	std::vector<vector3> baseQuartPoints;

	//Add middle points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree);	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree);

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, 0, zValue);
		midPoints.push_back(tempPoint);
	}

	//Add top 3/4 points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree) * 0.9f;	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree) * 0.9f;

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, fValue * 0.25f, zValue);
		topThreePoints.push_back(tempPoint);
	}

	//Add top half points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree) * 0.7f;	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree) * 0.7f;

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, fValue * 0.6f, zValue);
		topHalfPoints.push_back(tempPoint);
	}

	//Add top quarter points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree) * 0.4f;	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree) * 0.4f;

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, fValue * 0.85f, zValue);
		topQuartPoints.push_back(tempPoint);
	}

	//Add base 3/4 points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree) * 0.9f;	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree) * 0.9f;

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, -fValue * 0.25f, zValue);
		baseThreePoints.push_back(tempPoint);
	}

	//Add base half points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree) * 0.7f;	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree) * 0.7f;

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, -fValue * 0.6f, zValue);
		baseHalfPoints.push_back(tempPoint);
	}

	//Add base quarter points
	for (int s = 0; s < a_nSubdivisions + 1; s++)
	{
		//Calculate the degree of the point by going through subdivisions and multiplying the current num by the degrees between points
		float currentDegree = dValue * s;

		//Convert degrees to radians for input into sin / cos
		currentDegree = (currentDegree * acos(-1.0f)) / 180.0f;		//acos(-1) is PI

		//Calculate angle of point
		float xValue = a_fRadius * sin(currentDegree) * 0.4f;	//Calculate the x/z value of a point by multiplying the radius by sin/cos of the point's angle					
		float zValue = a_fRadius * cos(currentDegree) * 0.4f;

		//Create the point and add it to the vector
		vector3 tempPoint(xValue, -fValue * 0.85f, zValue);
		baseQuartPoints.push_back(tempPoint);
	}


	//Make top
	for (int e = 0; e < topQuartPoints.size() - 1; e++)
	{
		if (e == topQuartPoints.size() - 1)
			//Wrap around if at last input
		{
			AddTri(topQuartPoints[e], topQuartPoints[0], point0);
		}
		else
		{
			AddTri(topQuartPoints[e], topQuartPoints[e + 1], point0);
		}
	}

	//Make 1/4t-1/2t walls
	for (int y = 0; y < topQuartPoints.size() - 1; y++)
	{
		if (y == topQuartPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(topHalfPoints[y], topHalfPoints[0], topQuartPoints[y], topQuartPoints[0]);
		}
		else
		{
			AddQuad(topHalfPoints[y], topHalfPoints[y + 1], topQuartPoints[y], topQuartPoints[y + 1]);
		}
	}

	//Make 1/2t-3/4t walls
	for (int y = 0; y < topHalfPoints.size() - 1; y++)
	{
		if (y == topHalfPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(topThreePoints[y], topThreePoints[0], topHalfPoints[y], topHalfPoints[0]);
		}
		else
		{
			AddQuad(topThreePoints[y], topThreePoints[y + 1], topHalfPoints[y], topHalfPoints[y + 1]);
		}
	}

	//Make 3/4t-mid walls
	for (int y = 0; y < topThreePoints.size() - 1; y++)
	{
		if (y == topThreePoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(midPoints[y], midPoints[0], topThreePoints[y], topThreePoints[0]);
		}
		else
		{
			AddQuad(midPoints[y], midPoints[y + 1], topThreePoints[y], topThreePoints[y + 1]);
		}
	}

	//Make mid-3/4b walls
	for (int y = 0; y < midPoints.size() - 1; y++)
	{
		if (y == midPoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(baseThreePoints[y], baseThreePoints[0], midPoints[y], midPoints[0]);
		}
		else
		{
			AddQuad(baseThreePoints[y], baseThreePoints[y + 1], midPoints[y], midPoints[y + 1]);
		}
	}

	//Make 3/4b-1/2b walls
	for (int y = 0; y < baseThreePoints.size() - 1; y++)
	{
		if (y == baseThreePoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(baseHalfPoints[y], baseHalfPoints[0], baseThreePoints[y], baseThreePoints[0]);
		}
		else
		{
			AddQuad(baseHalfPoints[y], baseHalfPoints[y + 1], baseThreePoints[y], baseThreePoints[y + 1]);
		}
	}

	//Make 1/2b-1/4b walls
	for (int y = 0; y < baseHalfPoints.size() - 1; y++)
	{
		if (y == baseThreePoints.size() - 1)
			//Wrap around if at last input
		{
			AddQuad(baseQuartPoints[y], baseQuartPoints[0], baseHalfPoints[y], baseHalfPoints[0]);
		}
		else
		{
			AddQuad(baseQuartPoints[y], baseQuartPoints[y + 1], baseHalfPoints[y], baseHalfPoints[y + 1]);
		}
	}

	//Make base
	for (int e = 0; e < baseQuartPoints.size() - 1; e++)
	{
		if (e == baseQuartPoints.size() - 1)
			//Wrap around if at last input
		{
			AddTri(baseQuartPoints[0], baseQuartPoints[e], point1);
		}
		else
		{
			AddTri(baseQuartPoints[e + 1], baseQuartPoints[e], point1);
		}
	}
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}