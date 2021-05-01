#include "Particle.h"
#include "ParticleSystem.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h""
#include "Hair.h"
#include <iostream>

const int N_HAIRS_X = 1;
const int N_HAIRS_Y = 1;

const int N_PARTICLES = 10;
const int HEIGHT_HAIR = 1;
ParticleSystem::ParticleSystemType SYSTEM_TYPE = ParticleSystem::ParticleSystemType::Waterfall;
const int NUM_PARTICLES = 10;
Particle::UpdateMethod SOLVER = Particle::UpdateMethod::Verlet;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

#if defined HAIR_ENABLED
//Particle fixedParticle(0.0f, 0.7f, 0.0f);
//Hair hair(10, 0, 0.8, 0);
Hair hairs[N_HAIRS_X][N_HAIRS_Y];

#endif
//std::cout << "Lifetime =" << p.getLifetime() << std::endl;
//p.setBouncing(1.0f);
//p.addForce(0, -9.8f, 0);
// One Plane
int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	// build and compile our shader zprogram
	// ------------------------------------
	Shader ourShader("../resources/shaders/6.3.coordinate_systems.vs", "../resources/shaders/6.3.coordinate_systems.fs");

    Plane planes[5] = {
    Plane(0),
    Plane(1),
    Plane(2),
    Plane(5),
    Plane(4),
    };
    
    glm::vec3 pt1(3.5, 0, -3.5);
    glm::vec3 pt2(1.5, -2.5, -3.5);
    glm::vec3 pt3(4.5, -2.5, 1.5);
    Triangle triangle(pt1,pt2,pt3);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
    Sphere sphere;
    sphere.init(0, 2.0);
	sphere.setNewPos(0, -2.5, 0);

#if defined HAIR_ENABLED
    /*fixedParticle.Init();
    fixedParticle.setFixed(true);
    fixedParticle.setBouncing(1.0f);

    Particle p2(0.0f, 0.5f, 0.0f);
    p2.Init();
    p2.setFixed(false);
    p2.setBouncing(1.0f);
    p2.setP1(&fixedParticle);

    Particle p3(0.0f, 0.45f, 0.0f);
    p3.Init();
    p3.setFixed(false);
    p3.setBouncing(1.0f);
    p3.setP1(&p2);

    p2.setP2(&p3);*/
    for (int i = 0; i < N_HAIRS_X; ++i)
    {
        for (int j = 0; j < N_HAIRS_X; ++j)
        {
            hairs[i][j].init(N_PARTICLES, i * SPACING - N_HAIRS_X * SPACING / 2-0.3, HEIGHT_HAIR, j * SPACING - N_HAIRS_Y * SPACING / 2-0.3,true, SPACING);
        }
    }
    //hair.init();
#endif
    //PARTICLES
#if defined PARTICLE_SYS_ENABLED
	ParticleSystem ps;
	ps.setParticleSystem(NUM_PARTICLES);
	ps.iniParticleSystem(SYSTEM_TYPE);
#endif
	//Particle p(0.0f, 0.4f, 0.0f);
	//p.setLifetime(7.0f);
	//	p.setFixed(true);
	//std::cout << "Lifetime =" << p.getLifetime() << std::endl;
	//p.setBouncing(1.0f);
	//p.addForce(0, -9.8f, 0);
	// One Plane





	bool first = true;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
		float currentFrame = glfwGetTime();
		if (first)
		{
			deltaTime = 0.02;
		}
		else
		{
			deltaTime = std::min(currentFrame - lastFrame, 0.2f);
		}
		first = false;
        // per-frame time logic
        // --------------------
        
        
        lastFrame = currentFrame;
		//deltaTime = 0.02;

        // input
        // -----
        processInput(window);
#if defined PARTICLE_SYS_ENABLED
		ps.updateParticleSystem(deltaTime, SOLVER);
#endif
#if defined HAIR_ENABLED
        /*fixedParticle.updateParticle(deltaTime, SOLVER);
        p2.updateParticle(deltaTime, SOLVER);
        p3.updateParticle(deltaTime, SOLVER);
        */
        //hair.update(deltaTime, SOLVER);

        for (int i = 0; i < N_HAIRS_X; ++i)
        {
            for (int j = 0; j < N_HAIRS_X; ++j)
            {
                hairs[i][j].update(deltaTime, SOLVER);
            }
        }
#endif
		// update
				// call solver types: EulerOrig, EulerSemi and Verlet(to be implemented)
		//p.updateParticle(deltaTime, Particle::UpdateMethod::Verlet);
		//std::cout << "position = " << p.getCurrentPosition().x << "  " << p.getCurrentPosition().y
		//	<< "  " << p.getCurrentPosition().z << "  time = " << deltaTime << " en: " << p.getCurrentPosition().y*9.8+ 0.5*(p.m_velocity.x* p.m_velocity.x)+ 0.5 * (p.m_velocity.y * p.m_velocity.y) + 0.5 * (p.m_velocity.z * p.m_velocity.z) << std::endl;
		//Check Cube collisions
#if defined PARTICLE_SYS_ENABLED
		for (int indexPlane = 0; indexPlane < 5; ++indexPlane)
		{
			ps.checkColsPlane(planes[indexPlane]);
		}
		ps.checkColsTriangle(triangle);
		ps.checkColsSphere(sphere);
#endif
#if defined HAIR_ENABLED

        for (int i = 0; i < N_HAIRS_X; ++i)
        {
            for (int j = 0; j < N_HAIRS_X; ++j)
            {
                for (int indexPlane = 0; indexPlane < 5; ++indexPlane)
                {
                    hairs[i][j].checkColsPlane(planes[indexPlane]);
                }
                hairs[i][j].checkColsSphere(sphere);
                hairs[i][j].checkColsTriangle(triangle);
            }
        }
        /*for (int indexPlane = 0; indexPlane < 5; ++indexPlane)
        {
            hair.checkColsPlane(planes[indexPlane]);
        }
        hair.checkColsSphere(sphere);
        hair.checkColsTriangle(triangle);
        */
        /*bool col;
        for (int indexPlane = 0; indexPlane < 5; ++indexPlane)
        {
            col = p2.collisionParticlePlane(planes[indexPlane]);
            if (col)
                p2.correctCollisionParticlePlain(planes[indexPlane]);
        }
        col = p2.collisionParticleTriangle(triangle);
        if (col)
            p2.correctCollisionParticleTriangle(triangle);
        col = p2.collisionParticleSphere(sphere);
        if (col)
            p2.correctCollisionParticleSphere(sphere);

        for (int indexPlane = 0; indexPlane < 5; ++indexPlane)
        {
            col = p3.collisionParticlePlane(planes[indexPlane]);
            if (col)
                p3.correctCollisionParticlePlain(planes[indexPlane]);
        }
        col = p3.collisionParticleTriangle(triangle);
        if (col)
            p3.correctCollisionParticleTriangle(triangle);
        col = p3.collisionParticleSphere(sphere);
        if (col)
            p3.correctCollisionParticleSphere(sphere);
            */
#endif
		/*if (p.collisionParticleTriangle(triangle))
		{
			p.correctCollisionParticleTriangle(triangle);
			//system("PAUSE");
		}
		if (p.collisionParticleSphere(sphere))
		{
			
			p.correctCollisionParticleSphere(sphere);
			//system("PAUSE");
		}*/
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        ourShader.use();

        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("lightPos", lightPos);
        ourShader.setVec3("viewPos", camera.Position);

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

        // render models
        
        for (unsigned int i = 0; i < 5; i++)
        {
            planes[i].draw(&ourShader);
        }
        triangle.draw(&ourShader);
        //glDisable(GL_CULL_FACE);
        sphere.draw(&ourShader);
#if defined PARTICLE_SYS_ENABLED
		ps.drawParticles(&ourShader);
#endif
#if defined HAIR_ENABLED
        /*
        fixedParticle.draw(&ourShader);
        p2.draw(&ourShader);
        p3.draw(&ourShader);
        */
        //hair.draw(&ourShader);
        for (int i = 0; i < N_HAIRS_X; ++i)
        {
            for (int j = 0; j < N_HAIRS_X; ++j)
            {
                hairs[i][j].draw(&ourShader);
            }
        }
#endif
		//p.draw(&ourShader);
        //glEnable(GL_CULL_FACE);
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
#if defined HAIR_ENABLED
    /*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        fixedParticle.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        fixedParticle.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        fixedParticle.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        fixedParticle.ProcessKeyboard(RIGHT, deltaTime);
        */
#endif
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}