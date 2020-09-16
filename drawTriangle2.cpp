#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 设置顶点数据(and buffer(s)) and configure vertex attributes
float vertices[] = {
    -0.5f, -0.5f, 0.0f,  1.0f, 0, 0,// 0 
     0.5f, -0.5f, 0.0f, 0, 1.0f, 0,// 1 
     0.0f,  0.5f, 0.0f , 0, 0, 1.0f,// 2   
     0.8f, 0.8f, 0.0f , 0.3f, 0.5f, 0.7f//3
};
unsigned int indices[] = {
    2, 1, 0,//first triangle
    1, 2, 3//second triangle
};

const char* vertexShaderSource =
"#version 330 core                                       \n                   "
"layout(location = 6) in vec3 aPos;                     \n            "//vertexShader从vao6号上面挖数据
"layout(location = 7) in vec3 aColor;                     \n            "//vertexShader从vao7号上面挖数据
"out vec4 vertexColor ;                                             \n          "
"void main()                                             \n                       "
"{ \n                                                                                   "
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n      "
"   vertexColor = vec4(aColor.x, aColor.y, aColor.z, 1.0f);\n"
"}    \n";

const char* fragmentShaderSource =
"#version 330 core                             \n             "
"out vec4 FragColor;                           \n            "
"in vec4 vertexColor;                           \n           "
"\n                                                                      "
"void main()                                   \n                 "
"{ \n                                                                   "
"    FragColor = vertexColor ;}\n ";

int main()
{
    // glfw初始化和配置,与opengl关联起来
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // glfw创建窗口
    GLFWwindow* window = glfwCreateWindow(800,  600, "LearnOpenGL",  NULL,  NULL);
    if (window == NULL) {
        std::cout << "未能创建glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//客户端向服务器gpu发出申请
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);//设置回调函数，窗口大小变化时自动调用
   

    // glad用来管理opengl的函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //可以调用opengl函数
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glViewport(0, 0, 800, 600);//视口是用来显示图形的矩形区域，0，0是左下角位置
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

   

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//传输数据到vbo

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//传输数据到ebo

    //位置信息
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(6);//把数据从vbo或者ebo塞入vao中的6号位置
    //颜色信息
    glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(7);//把数据从vbo或者ebo塞入vao中的6号位置

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // 渲染循环
    while (!glfwWindowShouldClose(window))//检查glfw是否被要求退出
    {
        //实现输入控制
        processInput(window);

        // 渲染指令
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);//自定义颜色清空屏幕，否则看到上一次迭代的渲染结果
        glClear(GL_COLOR_BUFFER_BIT);

        // 画出三角形
        glBindVertexArray(VAO); //因为131行已经unbind了要重新bind
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//因为127unbind，要出现bind
        //glDrawArrays(GL_TRIANGLES, 0, 6);绑定VBO时用这个来画图

        float timValue = glfwGetTime();
        float rValue = (sin(timValue) / 2.0f) + 0.5f;
        float gValue = (sin(timValue) / 2.0f) + 0.5f;
        float bValue = (sin(timValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
  
        glUseProgram(shaderProgram);//shader编程
        glUniform4f(vertexColorLocation, rValue, gValue, bValue, 1.0f);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        // glBindVertexArray(0); // no need to unbind it every time 

        glfwSwapBuffers(window);//交换缓冲
        glfwPollEvents();//检查有无出发事件，如键盘输入，鼠标移动
    }

    // 可选: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //释放glfw分配的所有资源
    glfwTerminate();
    return 0;
}

// 处理所有的输入: glfw收到请求，一旦相应的键按住、松开会产生相应的处理
void processInput(GLFWwindow* window)
{
    //按下esc进行相应的设置来结束while循环
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 每当窗口大小变化时触发回调函数，调整视口大小与窗口大小保持一致。
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);//视口是用来显示图形的矩形区域，0，0是左下角位置
}
