#include "AppClass.h"
std::vector<MyMesh*> squares;

void Application::InitVariables(void)
{
	//init the mesh

	for (int a = 0; a < 46; a++)
	{
		m_pMesh = new MyMesh();
		m_pMesh->GenerateCube(1.0f, C_BLACK);
		squares.push_back(m_pMesh);
	}

	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	static float value = 0.0f;

	std::vector<matrix4> translates;

	//
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value + -1.0f, 0.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -2.0f, 0.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 0.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 0.0f, 0.0f));
	translates.push_back(m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -5.0f, 1.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -3.0f, 1.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 1.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 5.0f, 1.0f, 0.0f));
	translates.push_back(m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -5.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -3.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -2.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -1.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 0.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 5.0f, 2.0f, 0.0f));
	translates.push_back(m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -5.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -4.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -3.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -2.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -1.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 0.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 4.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 5.0f, 3.0f, 0.0f));
	translates.push_back(m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -4.0f, 4.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -3.0f, 4.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -1.0f, 4.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 0.0f, 4.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 4.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 4.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 4.0f, 4.0f, 0.0f));
	translates.push_back(m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -3.0f, 5.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -2.0f, 5.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -1.0f, 5.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 0.0f, 5.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 1.0f, 5.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 5.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 5.0f, 0.0f));
	translates.push_back(m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -2.0f, 6.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 2.0f, 6.0f, 0.0f));
	translates.push_back(m4Translate);

	m4Translate = glm::translate(IDENTITY_M4, vector3(value + -3.0f, 7.0f, 0.0f));
	translates.push_back(m4Translate);
	m4Translate = glm::translate(IDENTITY_M4, vector3(value + 3.0f, 7.0f, 0.0f));
	translates.push_back(m4Translate);
	//
	value += 0.01f;
	//

	for (int b = 0; b < translates.size(); b++)
	{
		matrix4 m4Model = m4Scale * translates[b];

		squares[b]->Render(m4Projection, m4View, m4Model);
	}
	//

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}