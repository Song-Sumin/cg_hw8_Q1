# cg_hw8_Q1 readme

## What you need
You need Visual Studio 2022 and window 11 OS.

And C/C++ should be available in VS2022.

## About
This project is about Immediate Mode.

To view the result image, open Q1_result.png, 

and if you want an explanation of the code, scroll down below.

## How to run

1. Click code and download as zip file.
   
![image](https://github.com/user-attachments/assets/2992e6c8-4764-4594-bee9-fd5972298ab8)

2. Unzip a download file

![image](https://github.com/user-attachments/assets/370022b6-86c9-4dbe-b44d-5297df3646c8)


3. Open cg_hw8_Q1-master. Double click cg_hw8_Q1-master and open OpenglViewer

![image](https://github.com/user-attachments/assets/48a6acdb-f1c6-4e9b-b08c-a7682515a777)

5. click "F5" on your keybord. Then you will get the result.

![Q1_result](https://github.com/user-attachments/assets/4879d0e3-8bbf-4ee3-96ff-be01c4b40c00)


## Code explanation


```
GLuint LoadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path.c_str(), std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory? Don't forget to read the FAQ!\n", vertex_file_path.c_str());
        getchar();
        return 0;
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path.c_str(), std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader: %s\n", vertex_file_path.c_str());
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path.c_str());
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDetachShader(ProgramID, VertexShaderID);
    glDetachShader(ProgramID, FragmentShaderID);

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}
```
The LoadShaders function loads, compiles, and links a vertex and fragment shader from given file paths.

It creates shader objects and returns the final shader program ID.

-------------


```
void Update_mesh(const GLuint& VAO, const std::vector<GLuint>& GLBuffers,
    std::vector<glm::vec3> Positions,
    std::vector<glm::vec3> Normals,
    std::vector<glm::vec3> Colors,
    std::vector<unsigned int>& Indices)
{
    glBindVertexArray(VAO); // Must be bound before binding buffers

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(Positions[0]), &Positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normals[0]), &Normals[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, GLBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Colors[0]), &Colors[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GLBuffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(Indices[0]), &Indices[0], GL_STATIC_DRAW);

    //glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO to avoid accidental modification
    glBindVertexArray(0); // Unbind VAO to avoid accidental modification
}

```
The Update_mesh function uploads data to GPU buffers and sets up vertex attribute pointers within a given VAO. 

It prepares the mesh for rendering with glDrawElements.


-----------
```
// --------------------------------------------------
// Global Variables
// --------------------------------------------------
int Width = 512;
int Height = 512;
// --------------------------------------------------

void resize_callback(GLFWwindow*, int nw, int nh)
{
    Width = nw;
    Height = nh;
    // Tell the viewport to use all of our screen estate
    glViewport(0, 0, nw, nh);
}
// Sphere mesh 생성 함수 추가
void CreateSphereMesh(float radius, int width, int height, std::vector<vec3>& Positions, std::vector<vec3>& Normals, std::vector<unsigned int>& Indices) {
    Positions.clear();
    Normals.clear();
    Indices.clear();

    // 내부 영역 (극점 제외)
    for (int j = 1; j < height - 1; ++j) {
        float theta = float(j) / (height - 1) * pi<float>();
        for (int i = 0; i < width; ++i) {
            float phi = float(i) / (width - 1) * pi<float>() * 2.0f;
            float x = sin(theta) * cos(phi);
            float y = cos(theta);
            float z = -sin(theta) * sin(phi);
            vec3 p = radius * vec3(x, y, z);
            Positions.push_back(p);
            Normals.push_back(normalize(p));
        }
    }

    // 북극점
    Positions.push_back(vec3(0, radius, 0));
    Normals.push_back(vec3(0, 1, 0));

    // 남극점
    Positions.push_back(vec3(0, -radius, 0));
    Normals.push_back(vec3(0, -1, 0));

    int topIndex = Positions.size() - 2;
    int bottomIndex = Positions.size() - 1;

    // 내부 삼각형
    for (int j = 0; j < height - 3; ++j) {
        for (int i = 0; i < width - 1; ++i) {
            int idx0 = j * width + i;
            int idx1 = (j + 1) * width + (i + 1);
            int idx2 = j * width + (i + 1);
            int idx3 = (j + 1) * width + i;

            Indices.push_back(idx0);
            Indices.push_back(idx1);
            Indices.push_back(idx2);

            Indices.push_back(idx0);
            Indices.push_back(idx3);
            Indices.push_back(idx1);
        }
    }

    // 북극 삼각형
    for (int i = 0; i < width - 1; ++i) {
        Indices.push_back(topIndex);
        Indices.push_back(i);
        Indices.push_back(i + 1);
    }

    // 남극 삼각형
    for (int i = 0; i < width - 1; ++i) {
        int base = (height - 3) * width;
        Indices.push_back(bottomIndex);
        Indices.push_back(base + i + 1);
        Indices.push_back(base + i);
    }
}
```
resize_callback: Updates window size

CreateSphereMesh: Make sphere using sphere_scene.cpp.

--------------

```
int main(int argc, char* argv[]) {
    GLFWwindow* window;
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int Width = 512, Height = 512;
    window = glfwCreateWindow(Width, Height, "OpenGL Viewer", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) return -1;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint shaderProgram = LoadShaders("Phong.vert", "Phong.frag");

    std::vector<vec3> Positions, Normals, Colors;
    std::vector<unsigned int> Indices;
    CreateSphereMesh(1.0f, 32, 16, Positions, Normals, Indices);
    for (size_t i = 0; i < Positions.size(); ++i) Colors.push_back(vec3(0.0f));

    mat4 model = scale(translate(mat4(1.0f), vec3(0, 0, -7)), vec3(2.0f));
    mat4 view = lookAt(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
    mat4 projection = frustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    std::vector<GLuint> GLBuffers(4);
    glGenBuffers(4, &GLBuffers[0]);
    Update_mesh(VAO, GLBuffers, Positions, Normals, Colors, Indices);

    vec3 lightPos(-4.0f, 4.0f, -3.0f);
    vec3 viewPos(0.0f);
    vec3 lightColor(1.0f);

    vec3 ka(0.0f, 1.0f, 0.0f);
    vec3 kd(0.0f, 0.5f, 0.0f);
    vec3 ks(0.5f, 0.5f, 0.5f);
    float shininess = 32.0f;

    glUseProgram(shaderProgram);
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, &lightPos[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, &viewPos[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, &lightColor[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "ka"), 1, &ka[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "kd"), 1, &kd[0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "ks"), 1, &ks[0]);
    glUniform1f(glGetUniformLocation(shaderProgram, "shininess"), shininess);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(4, &GLBuffers[0]);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
```
main function get shader from "Phong.vert", "Phong.frag", creates a sphere mesh, sets up shaders and transformation matrices, and renders the sphere with Phong shading . 


--------------

```
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
```
phong.vert

phong.vert file transforms 3D positions on screen and normals lighting.


--------------

```
#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 outColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float shininess;

void main() {
    // Ambient
    vec3 ambient = 0.2 * ka;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = kd * diff;

    // Specular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specular = ks * spec * diff; 

    vec3 result = (ambient + diffuse + specular) * lightColor;
    result = pow(result, vec3(1.0 / 2.2));
    outColor = vec4(result, 1.0);
}
```
phong.frag

phong.frag file uses Blinn-Phong lighting with gamma correction to compute realistic lighting effects per pixel.

--------------
