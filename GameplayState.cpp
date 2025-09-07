#include "GameplayState.h"

void GameplayState::init(int* swState, Renderer* r, Camera* c)
{
	switchState = swState;
	renderer = r;
	camera = c;

	texture = loadBMP("res/stallTexture.bmp");
	loadOBJtoBuffers("res/stall.obj", vertexbuffer, uvbuffer, normalbuffer, elementbuffer, objSize);

	texture2 = loadBMP("res/uvmap.bmp");
	loadOBJtoBuffers("res/monkey.obj", vertexbuffer2, uvbuffer2, normalbuffer2, elementbuffer2, objSize2);

	texture3 = loadBMP("res/lightmap.bmp");
	loadOBJtoBuffers("res/room.obj", vertexbuffer3, uvbuffer3, normalbuffer3, elementbuffer3, objSize3);

	swordTexture = loadBMP("res/SwordTexture.bmp");
	loadOBJtoBuffers("res/sword.obj", swordVbuffer, swordUVbuffer, swordNbuffer, swordEbuffer, swordObjSize);

	cTexture = loadBMP("res/characterTexture.bmp");
	loadStaticMeshtoBuffers("res/character.mesh", cVbuffer, cUVbuffer, cNbuffer, cEbuffer, cObjSize);

	//TO RENDER MORE THAN ONE TEXTURE FROM FBO, USE OTHER GL_COLOR_ATTACHEMENTS AND OTHER TEXTURES

	//Screen texture and quad creation
	createEmptyTexture(screenTexture, camera->screenW, camera->screenH);
	renderer->attachTextureToFBO(screenTexture, GL_COLOR_ATTACHMENT0);
	generateScreenQuad(screenVBuffer);

	baseShader.init("shaders/BaseVertShader.txt", "shaders/BaseFragShader.txt");
	baseShader.addUniform("ModelMatrix");
	baseShader.addUniform("ViewMatrix");
	baseShader.addUniform("ProjectionMatrix");
	baseShader.addUniform("TexSampler");

	screenShader.init("shaders/QuadOnScreenVert.txt", "shaders/BaseFragShader.txt");
	screenShader.addUniform("TexSampler");

	weaponShader.init("shaders/weaponVert.txt", "shaders/weaponFrag.txt");
	weaponShader.addUniform("ModelMatrix");
	weaponShader.addUniform("ProjectionMatrix");
	weaponShader.addUniform("TexSampler");
}

void GameplayState::render()
{
	glClearColor(0.0f, 0.2f, 0.0f, 0.0f);

	renderer->renderToFBO(0, 0, camera->screenW, camera->screenH, GL_COLOR_ATTACHMENT0);

	renderer->setShader(&baseShader);
	
	renderer->setTexture(texture);
	renderer->renderMesh(camera, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, objSize, glm::vec3(0, -2, -5), glm::vec3(0, 0, 0));
	
	renderer->setTexture(cTexture);
	renderer->renderMesh(camera, cVbuffer, cUVbuffer, cNbuffer, cEbuffer, cObjSize, glm::vec3(0, 7, 0), glm::vec3(0), glm::vec3(5));

	renderer->setTexture(texture2);
	renderer->renderMesh(camera, vertexbuffer2, uvbuffer2, normalbuffer2, elementbuffer2, objSize2, glm::vec3(0, 1.5, -6), glm::vec3(0, 0, 0));

	renderer->setTexture(texture3);
	renderer->renderMesh(camera, vertexbuffer3, uvbuffer3, normalbuffer3, elementbuffer3, objSize3, glm::vec3(0, 0, -15), glm::vec3(0, 0, 0));

	//renderer->setShader(&weaponShader);
	//renderer->setTexture(swordTexture);
	//renderer->renderMesh(camera, swordVbuffer, swordUVbuffer, swordNbuffer, swordEbuffer, swordObjSize, glm::vec3(0.3, -0.5, -1), glm::vec3(-90*abs(sinf(glm::radians(angle))), 0, 0), glm::vec3(0.125f));

	angle += 0.25f;

	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);
	
	//Rendering screen texture to screen
	renderer->renderToScreen(0, 0, camera->screenW, camera->screenH);
	//WHEN MOVING CAMERA, MATRICES HAVE TO BE RECALULATED AFTER EACH MOVEMENT
	//TODO: ADD PUSH AND POP FOR CAMERA POSITIONS
	//camera->camSetPos(glm::vec3(0, 0, 10));
	camera->calculateMatrices();

	/*renderer->setTexture(texture);
	renderer->renderMesh(camera, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, objSize, glm::vec3(0, -2, -5), glm::vec3(0, angle, 0));

	renderer->setTexture(texture2);
	renderer->renderMesh(camera, vertexbuffer2, uvbuffer2, normalbuffer2, elementbuffer2, objSize2, glm::vec3(0, 1.5, -6), glm::vec3(0, angle, 0));

	renderer->setTexture(texture3);
	renderer->renderMesh(camera, vertexbuffer3, uvbuffer3, normalbuffer3, elementbuffer3, objSize3, glm::vec3(0, 0, -15), glm::vec3(0, angle, 0));*/
	
	renderer->setShader(&screenShader);
	renderer->setTexture(screenTexture);
	renderer->renderMesh(camera, screenVBuffer, 6, glm::vec3(0, 0, -10));
}

void GameplayState::update()
{
	if (glfwGetKey(camera->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->camMove(camera->getRightVector()*glm::vec3(-0.01));
	}

	if (glfwGetKey(camera->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->camMove(camera->getRightVector()*glm::vec3(0.01));
	}

	if (glfwGetKey(camera->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->camMove(camera->getViewDirVector()*glm::vec3(-0.01));
	}
	
	if (glfwGetKey(camera->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->camMove(camera->getViewDirVector()*glm::vec3(0.01));
	}

	//CAM CONTROL
	if (glfwGetKey(camera->getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera->camRotate(glm::vec3(-0.25, 0, 0));
	}

	if (glfwGetKey(camera->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera->camRotate(glm::vec3(0.25, 0, 0));
	}

	if (glfwGetKey(camera->getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera->camRotate(glm::vec3(0, -0.25, 0));
	}

	if (glfwGetKey(camera->getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera->camRotate(glm::vec3(0, 0.25, 0));
	}

	if (glfwGetKey(camera->getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera->camRotate(glm::vec3(0, 0, 0.25));
	}

	if (glfwGetKey(camera->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
	{
		camera->camRotate(glm::vec3(0, 0, -0.25));
	}
}

void GameplayState::close()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	renderer->close();
	camera->close();
}

void GameplayState::resetState(bool fullReset)
{
}