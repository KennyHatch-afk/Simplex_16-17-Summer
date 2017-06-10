/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/06
----------------------------------------------*/
#ifndef __PRIMITIVEMANAGERSINGLETON_H_
#define __PRIMITIVEMANAGERSINGLETON_H_

#include "Simplex\Mesh\PrimitiveClass.h"
#include "Simplex\Mesh\PrimitiveWireClass.h"
#include "Simplex\Mesh\MeshDrawerSingleton.h"


namespace Simplex
{

//System Class
class SimplexDLL PrimitiveManagerSingleton
{
	static PrimitiveManagerSingleton* m_pInstance; // Singleton of the class

	MeshClass* m_pAxis = nullptr; //Axis primitive
	MeshClass* m_pGrid = nullptr; //Grid primitive
	MeshClass* m_pLine = nullptr; //Line primitive
	MeshClass* m_pPlane = nullptr; //Plane primitive
	MeshClass* m_pCube = nullptr; //Cube primitive
	MeshClass* m_pCubeWire = nullptr; //Cube Wire primitive
	MeshClass* m_pCone = nullptr; //Cone primitive
	MeshClass* m_pConeWire = nullptr; //Cone Wire primitive
	MeshClass* m_pCylinder = nullptr; //Cylinder primitive
	MeshClass* m_pCylinderWire = nullptr; //Cylinder Wire primitive
	MeshClass* m_pTube = nullptr; //Tube primitive
	MeshClass* m_pTubeWire = nullptr; //Tube Wire primitive
	MeshClass* m_pTorus = nullptr; //Torus primitive
	MeshClass* m_pTorusWire = nullptr; //Torus Wire primitive
	MeshClass* m_pSphere = nullptr;//Sphere primitive
	MeshClass* m_pSphereWire = nullptr;//Sphere Wire primitive
	MeshClass* m_pHexagon = nullptr;//Hexagon primitive
	MeshClass* m_pHexagonWire = nullptr;//Hexagon Wire primitive
	MeshClass* m_pSkybox = nullptr;//Skybox primitive

	MeshDrawerSingleton* m_pMeshDrawer = nullptr;

public:
	/*
	USAGE: Gets/Constructs the singleton pointer
	ARGUMENTS:
	OUTPUT:
	*/
	static PrimitiveManagerSingleton* GetInstance();
	/*
	USAGE: Destroys the singleton
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	static void ReleaseInstance(void);
	/*
	USAGE: Render the axis on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddSkyboxToRenderList(String a_sTextureName);

	/*
	USAGE: Render the axis on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddAxisToRenderList(matrix4 a_m4ToWorld);
	/*
	USAGE: Renders the cube on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddPlaneToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG);
	/*
	USAGE: Renders the cube on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddCubeToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG, int a_RenderOption = BTO_RENDER::RENDER_SOLID | BTO_RENDER::RENDER_WIRE);
	/*
	USAGE: Renders the cone on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddConeToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG, int a_RenderOption = BTO_RENDER::RENDER_SOLID | BTO_RENDER::RENDER_WIRE);
	/*
	USAGE: Renders the cylinder on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddCylinderToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG, int a_RenderOption = BTO_RENDER::RENDER_SOLID | BTO_RENDER::RENDER_WIRE);
	/*
	USAGE: Renders the tube on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddTubeToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG, int a_RenderOption = BTO_RENDER::RENDER_SOLID | BTO_RENDER::RENDER_WIRE);
	/*
	USAGE: Renders the torus on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddTorusToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG, int a_RenderOption = BTO_RENDER::RENDER_SOLID | BTO_RENDER::RENDER_WIRE);
	/*
	USAGE: Renders a hexagon on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddHexagonToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG, int a_RenderOption = BTO_RENDER::RENDER_SOLID | BTO_RENDER::RENDER_WIRE);
	/*
	USAGE: Renders the sphere on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	MeshClass* AddSphereToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG, int a_RenderOption = BTO_RENDER::RENDER_SOLID | BTO_RENDER::RENDER_WIRE);
	/*
	USAGE: Renders the sphere on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	void AddGridToRenderList(float a_fSize = 1.0f, int a_Axis = BTO_AXIS::AX_XY, vector3 a_v3Color = DEFAULT_V3NEG);
	/*
	USAGE: Renders the sphere on the specified position
	ARGUMENTS:
	OUTPUT: returns the pointer to mesh used in the queue
	*/
	void AddLineToRenderList(vector3 a_v3Start, vector3 a_v3End, vector3 a_v3StartColor = DEFAULT_V3NEG, vector3 a_v3EndColor = DEFAULT_V3NEG);
	/*
	USAGE: Renders a camera shape on the specified position
	ARGUMENTS:
	OUTPUT: ---
	*/
	void AddCameraToRenderList(matrix4 a_m4ToWorld, vector3 a_v3Color = DEFAULT_V3NEG);
	
private:
	/*
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT:
	*/
	PrimitiveManagerSingleton(void);
	/*
	USAGE: Copy constructor
	ARGUMENTS:
	OUTPUT:
	*/
	PrimitiveManagerSingleton(PrimitiveManagerSingleton const& other);
	/*
	USAGE: Copy Assignment operator
	ARGUMENTS:
	OUTPUT:
	*/
	PrimitiveManagerSingleton& operator=(PrimitiveManagerSingleton const& other);
	/*
	USAGE: Destructor
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~PrimitiveManagerSingleton(void);
	/*
	USAGE: Releases the objects memory
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	/*
	USAGE: Initializates the objects fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
};

}

#endif //__PRIMITIVEMANAGERSINGLETON_H_