#include <algorithm>
#include <future>
#include <iostream>
#include <string_view>
#include <thread>

#include <SDL.h>
#include <SDL_opengl.h>
#include <array>
#include <font_cache.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <imgui_stdlib.h>
#include <load_image.h>

static std::string username = "";

void show_tab_window_func(bool &show_tab_window) {
  static std::string identify_code = "01";
  static bool init = true;
  static std::array<char, 65> sk = {0};
  static std::string id = u8"郭亚文";

  ImGui::Begin("Another Window", &show_tab_window);
  ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
  if (ImGui::BeginTabBar("sm9", tab_bar_flags)) {
    if (ImGui::BeginTabItem("sign/verify")) {

      // Child 3: manual-resize
      ImGui::SeparatorText("key");
      {
        if (ImGui::BeginChild(
                "key",
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8),
                ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY)) {
          ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.9f);
          ImGui::InputText(u8"主私钥", sk.data(), sk.size(),
                           ImGuiInputTextFlags_CharsHexadecimal);

          ImGui::PopItemWidth(); // Reset item width to default

          ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
          ImGui::InputText(u8"识别码", identify_code.data(),
                           identify_code.size(), ImGuiInputTextFlags_ReadOnly);
          ImGui::PopItemWidth(); // Reset item width to default

          ImGui::SameLine();

          ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.8f);
          ImGui::InputText(u8"ID", &username);
        //   ImGui::InputText(u8"ID", &id);
          ImGui::PopItemWidth(); // Reset item width to default
        }

        ImGui::EndChild();
      }

      ImGui::SeparatorText("operation");
      {
        ImGui::PushStyleColor(ImGuiCol_ChildBg,
                              ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
        if (ImGui::BeginChild(
                "operation",
                ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 8),
                ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY)) {
          if (ImGui::Button(u8"生成主私钥")) {
            if (std::all_of(sk.begin(), sk.end(),
                            [](char c) { return c == 0; })) {
              std::copy_n("001122", 6, sk.data());
            }
            std::cout << id.data() << std::endl;
          }
        }

        ImGui::PopStyleColor();
        ImGui::EndChild();
      }

      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("enc/dec")) {
      ImGui::Text("This is the enc/dec tab!\n");
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("key encap")) {
      ImGui::Text("This is the key encap tab!\n");
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::Separator();
  ImGui::End();
}

void show_image_window_func(bool &show_image_window) {
  int my_image_width = 0;
  int my_image_height = 0;
  GLuint my_image_texture = 0;
  bool ret = LoadTextureFromFile("d:/picture/gray.png", &my_image_texture,
                                 &my_image_width, &my_image_height);
  IM_ASSERT(ret);
  ImGui::Begin("Another Window",
               &show_image_window); // Pass a pointer to our bool variable
                                    // (the window will have a closing button
                                    // that will clear the bool when clicked)
  ImGui::Text("Hello from another window!");
  if (ImGui::Button("Close Me"))
    show_image_window = false;
  ImGuiIO &io = ImGui::GetIO();
  ImGui::TextWrapped("Below we are displaying the font texture (which is the "
                     "only texture we have access to in this demo). "
                     "Use the 'ImTextureID' type as storage to pass pointers "
                     "or identifier to your own texture data. "
                     "Hover the texture for a zoomed view!");

  // Below we are displaying the font texture because it is the only texture we
  // have access to inside the demo! Remember that ImTextureID is just storage
  // for whatever you want it to be. It is essentially a value that will be
  // passed to the rendering backend via the ImDrawCmd structure. If you use one
  // of the default imgui_impl_XXXX.cpp rendering backend, they all have
  // comments at the top of their respective source file to specify what they
  // expect to be stored in ImTextureID, for example:
  // - The imgui_impl_dx11.cpp renderer expect a 'ID3D11ShaderResourceView*'
  // pointer
  // - The imgui_impl_opengl3.cpp renderer expect a GLuint OpenGL texture
  // identifier, etc. More:
  // - If you decided that ImTextureID = MyEngineTexture*, then you can pass
  // your MyEngineTexture* pointers
  //   to ImGui::Image(), and gather width/height through your own functions,
  //   etc.
  // - You can use ShowMetricsWindow() to inspect the draw data that are being
  // passed to your renderer,
  //   it will help you debug issues if you are confused about it.
  // - Consider using the lower-level ImDrawList::AddImage() API, via
  // ImGui::GetWindowDrawList()->AddImage().
  // - Read https://github.com/ocornut/imgui/blob/master/docs/FAQ.md
  // - Read
  // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
  {
    static bool use_text_color_for_tint = false;
    ImGui::Checkbox("Use Text Color for Tint", &use_text_color_for_tint);
    ImGui::Text("%.0dx%.0d", my_image_width, my_image_height);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 uv_min = ImVec2(0.0f, 0.0f); // Top-left
    ImVec2 uv_max = ImVec2(1.0f, 1.0f); // Lower-right
    ImVec4 tint_col = use_text_color_for_tint
                          ? ImGui::GetStyleColorVec4(ImGuiCol_Text)
                          : ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
    ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
    ImGui::Image((void *)(intptr_t)my_image_texture,
                 ImVec2(my_image_width, my_image_height), uv_min, uv_max,
                 tint_col, border_col);
    if (ImGui::BeginItemTooltip()) {
      float region_sz = 32.0f;
      float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
      float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
      float zoom = 4.0f;
      if (region_x < 0.0f) {
        region_x = 0.0f;
      } else if (region_x > my_image_width - region_sz) {
        region_x = my_image_width - region_sz;
      }
      if (region_y < 0.0f) {
        region_y = 0.0f;
      } else if (region_y > my_image_height - region_sz) {
        region_y = my_image_height - region_sz;
      }
      ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
      ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz,
                  region_y + region_sz);
      ImVec2 uv0 =
          ImVec2((region_x) / my_image_width, (region_y) / my_image_height);
      ImVec2 uv1 = ImVec2((region_x + region_sz) / my_image_width,
                          (region_y + region_sz) / my_image_height);
      ImGui::Image((void *)(intptr_t)my_image_texture,
                   ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1,
                   tint_col, border_col);
      ImGui::EndTooltip();
    }
  }

  ImGui::End();
}

int thread_gui() {
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) !=
      0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  // From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
  SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
                        SDL_WINDOW_ALLOW_HIGHDPI);
  SDL_Window *window = SDL_CreateWindow(
      "Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
  if (window == nullptr) {
    printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    return -1;
  }

  SDL_GLContext gl_context = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

//   font_atlas_cache cache = load_font_atlas_cache("./simhei.imfont");
//   restore_font_atlas(cache);

  // Setup Dear ImGui style
  //   ImGui::StyleColorsClassic();
  ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Oor state
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  // Main loop
  bool done = false;
  bool show_image_window = false;
  bool show_tab_window = false;

  while (!done) {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
    // tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
    // your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
    // data to your main application, or clear/overwrite your copy of the
    // keyboard data. Generally you may always pass all inputs to dear imgui,
    // and hide them from your application based on those two flags.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      ImGui_ImplSDL2_ProcessEvent(&event);
      if (event.type == SDL_QUIT)
        done = true;
      if (event.type == SDL_WINDOWEVENT &&
          event.window.event == SDL_WINDOWEVENT_CLOSE &&
          event.window.windowID == SDL_GetWindowID(window))
        done = true;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair
    // to create a named window.
    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                     // and append into it.

      ImGui::Text("This is some useful text."); // Display some text (you can
                                                // use a format strings too)
      ImGui::Checkbox("Another Window", &show_image_window);

      ImGui::Checkbox("tab Window", &show_tab_window);

      ImGui::SliderFloat("float", &f, 0.0f,
                         1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3(
          "clear color",
          (float *)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                   // widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    // 3. Show another simple window.
    if (show_image_window) {
      show_image_window_func(show_image_window);
    }

    if (show_tab_window) {
      show_tab_window_func(show_tab_window);
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}