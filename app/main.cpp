#include <cstdio>
#include <GLFW/glfw3.h>   // também inclui o cabeçalho do OpenGL
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

static void onGlfwError(int code, const char* description) {
    std::fprintf(stderr, "GLFW erro %d: %s\n", code, description);
}

int main() {
    glfwSetErrorCallback(onGlfwError);
    if (!glfwInit()) return 1;

    // Pedimos um contexto OpenGL 3.3 de núcleo, que é o que o ImGui e os shaders vão usar
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Bancada", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // sincroniza com a taxa de atualização da tela

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)) {
        // 1. Processa eventos do sistema (teclado, mouse, redimensionamento)
        glfwPollEvents();

        // 2. Prepara um novo quadro do ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 3. Descreve a interface deste quadro (modo imediato)
        ImGui::Begin("Bancada");
        ImGui::Text("Simulador em desenvolvimento");
        ImGui::End();

        // 4. Gera a lista de comandos de desenho e limpa a tela
        ImGui::Render();
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.08f, 0.09f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 5. Envia os comandos para a GPU e mostra o quadro pronto
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}