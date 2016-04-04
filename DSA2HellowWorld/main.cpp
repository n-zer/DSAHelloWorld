#include "main.h"

/*
	NOTE:

	All the code in this project is heavily based on the code from
	the tutorial at http://www.learnopengl.com/. I haven't looked at
	their finished source code, but I followed the tutorial
	pretty closely, so I'm sure it's very similar.
*/

//vertex/element buffers
GLuint vbo;
GLuint vao;
GLuint ebo;

bool firstMouse = true;

//mouse variables
double lastX;
double lastY;

//keyboard variables
bool keys[1024];

//camera variables
CameraSingleton& camera = CameraSingleton::getInstance();

//camera and movement coefficients
const GLfloat cameraSpeed = .5f;
const GLfloat moveSpeed = 0.2f;

int main()
{
	//initialize and set versions
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//use core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//not resizable
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//create window, check for failure
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MainWindow", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//set context
	glfwMakeContextCurrent(window);

	//initialize glew and check for failure
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	//viewport and key/mouse callback
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);

	glEnable(GL_DEPTH_TEST);

	//create obects	
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	//bind to VAO
	glBindVertexArray(vao);	
	//copy vertex array into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//copy index array into element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindVertexArray(0);	

	//shaders
	GLuint shaderProgram = GenerateShaderProgram("vertexShader.shader", "fragmentShader.shader");
	glUseProgram(shaderProgram);

	//some values
	GLfloat timeValue = glfwGetTime();
	GLfloat greenValue = 0.0f;

	camera.SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	camera.SetTarget(glm::vec3(0.0f, 0.0f, -1.0f));
	camera.SetUp(glm::vec3(0.0f, 1.0f, 0.0f));

	//get uniform matrix locations
	GLint rotateMatLocation = glGetUniformLocation(shaderProgram, "rotMatrix");
	GLint modelLocation = glGetUniformLocation(shaderProgram, "model");
	GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
	GLint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	//game loop
	while (!glfwWindowShouldClose(window))
	{
		//poll the keyboard
		glfwPollEvents();
		//clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//move the camera
		doMovement();

		//update view and projection matrices in the shaders
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera.GetView()));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera.GetProjection(false)));

		//bind our vertex array object
		glBindVertexArray(vao);
		//we're drawing ten objects
		for (GLuint i = 0; i < 10; i++)
		{
			//create model matrix
			glm::mat4 model;
			//translate according to our objPosition array
			model = glm::translate(model, objPositions[i]);
			//the models will rotate over time, just for funsies
			model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime()*50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			//set the model matrix in the shader
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
			//draw the object
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		//unbind VAO
		glBindVertexArray(0);

		//swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scandcode, int action, int mode)
{
	//esc closes the app
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//when a key is press, enable it in the keys array
	else if (action == GLFW_PRESS)
		keys[key] = true;
	//when it's released, disable it
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//this prevents jumps on the first mouse movement
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	//get the deltas
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	camera.ChangePitch(yoffset/10);
	camera.ChangeYaw(xoffset/10);
	lastX = xpos;
	lastY = ypos;
}

void doMovement()
{
	//check whether any of the movement keys are pressed, and adjust the camera position if they are
	if (keys[GLFW_KEY_W])
		camera.MoveForward(.1f);
	if (keys[GLFW_KEY_S])
		camera.MoveForward(-.1f);
	if (keys[GLFW_KEY_A])
		camera.MoveSideways(-.1f);
	if (keys[GLFW_KEY_D])
		camera.MoveSideways(.1f);
	if (keys[GLFW_KEY_Z])
		camera.ChangeRoll(-1.0f);
	if (keys[GLFW_KEY_C])
		camera.ChangeRoll(1.0f);
	if (keys[GLFW_KEY_Q])
		camera.MoveVertical(.1f);
	if (keys[GLFW_KEY_E])
		camera.MoveVertical(-.1f);
}

//similar to:
//http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
GLuint GenerateShaderProgram(const char* vertPath, const char* fragPath)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertShaderSourceStd = ReadFile(vertPath);
	std::string fragShaderSourceStd = ReadFile(fragPath);
	const char* vertShaderSource = vertShaderSourceStd.c_str();
	const char* fragShaderSource = fragShaderSourceStd.c_str();

	GLint result = GL_FALSE;
	int logLength=1;

	//compile vertex shader
	glShaderSource(vertShader, 1, &vertShaderSource, nullptr);
	glCompileShader(vertShader);

	//check vertex shader
	CheckShader(vertShader);

	//compile fragment shader
	glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
	glCompileShader(fragShader);

	//check fragment shader
	CheckShader(fragShader);

	//link program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	//check program
	CheckShaderProgram(program);

	//delete shaders
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

GLint CheckShader(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength;

	//get the compile status of the shader
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) //if it wasn't successful...
	{
		//get the length of the info log and create a vector of that length
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		//std::vector<char> shaderError(logLength);
		//populate the vector with the info log's contents
		//glGetShaderInfoLog(shader, logLength, nullptr, &shaderError[0]);
		//print the vector
		//std::cout << &shaderError[0] << std::endl;
	}

	return result;
}

//same as the above, but for the shader program itself
GLint CheckShaderProgram(GLuint program)
{
	GLint result = GL_FALSE;
	int logLength;

	glGetProgramiv(program, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		//std::vector<char> programError(logLength);
		//glGetProgramInfoLog(program, logLength, nullptr, &programError[0]);
		//std::cout << &programError[0] << std::endl;
	}

	return result;
}

//similar to:
//http://www.nexcius.net/2012/11/20/how-to-load-a-glsl-shader-in-opengl-using-c/
std::string ReadFile(const char* filePath)
{
	//create return string and file stream fot the given path
	std::string text;
	std::ifstream fileStream(filePath, std::ios::in);

	//if the stream couldn't be opened
	if (!fileStream.is_open())
	{
		std::cerr << "Error reading file " << filePath << std::endl;
	}

	std::string line = "";
	//go to the end of the stream
	while (!fileStream.eof())
	{
		//get line, append line to return string
		std::getline(fileStream, line);
		text.append(line + "\n");
	}
	fileStream.close();
	return text;
}