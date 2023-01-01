/*
 * File: Window.cpp
 * Author: Morgenthaler S
 *
 * Created on 2 novembre 2019, 11:17
 */

#include <istream>
#include "Window.h"


Window::Window(
    GLFWwindow *glfwWindow,
    int frameBufferWidth,
    int frameBufferHeight,
    int windowWidth,
    int windowHeight
) :
    _window(glfwWindow),
    _frameBufferWidth(frameBufferWidth),
    _frameBufferHeight(frameBufferHeight),
    _windowHeight(windowHeight),
    _windowWidth(windowWidth),
    _binaryFont(createBinaryFont()),
    _controller(
        _frameBufferWidth,
        _frameBufferHeight,
        createFilesContent(),
        _binaryFont.data(),
        _binaryFont.size(),
        false
    ) {
}

JBTypes::FileContent Window::createFilesContent() {

    std::map<std::string, std::string> fileDistPath;

    for (std::string shader: {
        "ballVs.vs",
        "basicFboVs.vs",
        "basicFboFs.fs",
        "blocksVs.vs",
        "bloomFs.fs",
        "brightPassFilter.fs",
        "depthFs.fs",
        "enemiesVs.vs",
        "horizontalBlurFs.fs",
        "inGamePageVs.vs",
        "itemsMapVs.vs",
        "labelFs.fs",
        "levelFs.fs",
        "levelsPageVs.vs",
        "specialsVs.vs",
        "starFs.fs",
        "starVs.vs",
        "titlePageVs.vs",
        "verticalBlurFs.fs"
    }) {
        fileDistPath[shader] = "shaders/" + shader;
    }

    constexpr size_t numberOfLevel = 2;
    for (size_t i = 1; i <= numberOfLevel; ++i) {
        const auto mapName = "map" + std::to_string(i) + ".txt";
        fileDistPath[mapName] = "maps/" + mapName;
    }

    const auto searchingDirs = {"./", "bin/"};
    JBTypes::FileContent filesContent;

    for (const auto &item: fileDistPath) {
        const auto &fileName = item.first;
        const auto &path = item.second;
        bool foundFile = false;
        std::ifstream inFile;
        for (const auto &searchingDir: searchingDirs) {
            if (!foundFile) {
                inFile.open(searchingDir + path);
                if (inFile) {
                    foundFile = true;
                }
            }
        }
        if (!foundFile) {
            std::cerr << "ERROR: Opening " << fileName << " impossible .." << std::endl;
            JBTypesMethods::displayInstallError();
            exit(EXIT_FAILURE);
        }
        std::stringstream strStream;
        strStream << inFile.rdbuf();
        filesContent[fileName] = strStream.str();
    }
    return filesContent;
}

std::vector<unsigned char> Window::createBinaryFont() {
    const auto searchingDirs = {"./", "bin/"};

    const std::string &fileName = "Cousine-Regular.ttf";
    const auto &path = "fonts/" + fileName;
    bool foundFile = false;
    std::ifstream fontFile;
    for (const auto &searchingDir: searchingDirs) {
        if (!foundFile) {
            fontFile.open(searchingDir + path, std::ios::binary);
            if (fontFile) {
                foundFile = true;
            }
        }
    }
    if (!foundFile) {
        std::cerr << "ERROR: Opening " << fileName << " impossible .." << std::endl;
        JBTypesMethods::displayInstallError();
        exit(EXIT_FAILURE);
    }
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(fontFile), {});
    return buffer;

}

bool Window::inputManagement() {

    int frameBufferWidth;
    int frameBufferHeight;
    glfwGetFramebufferSize(_window, &frameBufferWidth, &frameBufferHeight);
    if (frameBufferWidth == 0) {
        frameBufferWidth = 1;
    }
    if (frameBufferHeight == 0) {
        frameBufferHeight = 1;
    }
    if (_frameBufferWidth != frameBufferWidth || _frameBufferHeight != frameBufferHeight) {

        // Frame buffer resizing
        _frameBufferWidth = frameBufferWidth;
        _frameBufferHeight = frameBufferHeight;
        _controller.resize(_frameBufferWidth, _frameBufferHeight);

        // Window sizes updating
        int windowWidth;
        int windowHeight;
        glfwGetWindowSize(_window, &windowWidth, &windowHeight);
        if (windowWidth == 0) {
            windowWidth = 1;
        }
        if (windowHeight == 0) {
            windowHeight = 1;
        }

        _windowWidth = windowWidth;
        _windowHeight = windowHeight;
    }

    const std::function<KeyboardKey::Status(const std::vector<unsigned int> &&)>
        getButtonStatus = [this](const std::vector<unsigned int> &&keys) -> KeyboardKey::Status {
        for (unsigned int key: keys) {
            if (glfwGetKey(_window, static_cast<int>(key)) == GLFW_PRESS) {
                return KeyboardKey::Status::Pressed;
            }
        }
        return KeyboardKey::Status::Released;
    };

    // ESCAPE
    const KeyboardKey::Status escapeStatus = getButtonStatus({GLFW_KEY_ESCAPE});
    _controller.interactionButtons(KeyboardKey::Button::Escape, escapeStatus);

    // VALIDATE
    const KeyboardKey::Status validateStatus = getButtonStatus({GLFW_KEY_ENTER, GLFW_KEY_SPACE});
    _controller.interactionButtons(KeyboardKey::Button::Validate, validateStatus);

    // RIGHT
    const KeyboardKey::Status rightStatus = getButtonStatus({GLFW_KEY_RIGHT, GLFW_KEY_L});
    _controller.interactionButtons(KeyboardKey::Button::Right, rightStatus);

    // LEFT
    const KeyboardKey::Status leftStatus = getButtonStatus({GLFW_KEY_LEFT, GLFW_KEY_H});
    _controller.interactionButtons(KeyboardKey::Button::Left, leftStatus);

    // UP
    const KeyboardKey::Status upStatus = getButtonStatus({GLFW_KEY_UP, GLFW_KEY_K});
    _controller.interactionButtons(KeyboardKey::Button::Up, upStatus);

    // Down
    const KeyboardKey::Status downStatus = getButtonStatus({GLFW_KEY_DOWN, GLFW_KEY_J});
    _controller.interactionButtons(KeyboardKey::Button::Down, downStatus);


    /* For debugging only
    const KeyboardKey::Status stopStatus= getButtonStatus({GLFW_KEY_S});
    if(stopStatus == KeyboardKey::Status::Pressed) {
        _controller.stop();
    }

    const KeyboardKey::Status resumeStatus = getButtonStatus({GLFW_KEY_R});
    if(resumeStatus == KeyboardKey::Status::Pressed) {
        _controller.resume();
    }
     */

    // MOUSE BUTTON 1;
    const bool isMousePressed = glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

    if(isMousePressed) {
        const auto getPosXY = [this]() -> std::pair<float, float> {
            double posX, posY;
            glfwGetCursorPos(_window, &posX, &posY);
            return {static_cast <float>(posX), static_cast <float>(posY)};
        };
        const auto posXY = getPosXY();

        const auto posX = posXY.first / static_cast <float>(_windowWidth);

        // GLFW defines y=0 as the top
        const auto posY = 1.f - posXY.second / static_cast <float>(_windowHeight);
        _controller.pressMouse(posX, posY);
    } else {
        _controller.releaseMouse();
    }

    return glfwWindowShouldClose(_window) != 0 || _controller.requestToLeave();
}


void Window::run() {
    glfwSwapInterval(1);
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

    auto before = Chronometer::getTimePointMSNow();
    unsigned int counter = 0;
    while (!inputManagement()) {

        _controller.update();
        _controller.render();
        glfwSwapBuffers(_window);
        glfwPollEvents();

        ++counter;
        const auto after = Chronometer::getTimePointMSNow();
        auto diff = after - before;
        const std::chrono::duration<float> durationFloatDifference = diff;
        const float diffF = durationFloatDifference.count();
        if (diffF > 1.f) {
            before = after;
            std::cout << counter << " FPS" << std::endl;
            counter = 0;
        }

    }

}

