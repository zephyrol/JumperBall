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
        "inGamePageVs.vs",
        "labelFs.fs",
        "levelsPageVs.vs",
        "mapVs.vs",
        "mapFs.fs",
        "pausePageVs.vs",
        "postEffectsVs.vs",
        "postEffectsFs.fs",
        "starFs.fs",
        "starVs.vs",
        "successPageVs.vs",
        "storePageVs.vs",
        "titlePageVs.vs"
    }) {
        fileDistPath[shader] = "shaders/" + shader;
    }

    for (size_t i = 1; i <= Player::maxLevel ; ++i) {
        const auto mapName = "map" + std::to_string(i) + ".txt";
        fileDistPath[mapName] = "maps/" + mapName;
    }

    constexpr auto saveFileName = "save.txt";
    fileDistPath[saveFileName] = std::string("save/") + saveFileName;

    JBTypes::FileContent filesContent;
    for (const auto &item: fileDistPath) {
        const auto &fileName = item.first;
        const auto &path = item.second;
        filesContent[fileName] = readFile(path);
    }
    return filesContent;
}


std::string Window::readFile(const std::string& fileName) {

    bool foundFile = false;
    const auto searchingDirs = {"./", "bin/", "../"};
    std::ifstream inFile;
    for (const auto &searchingDir: searchingDirs) {
        if (!foundFile) {
            inFile.open(searchingDir + fileName);
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
    inFile.close();
    return strStream.str();
}

std::string Window::extractSpecificPartOfOutput(const std::string &updateOutput, const std::string &key) {
    const auto offset = updateOutput.find(key);
    if(offset == std::string::npos) {
        return "";
    }
    auto partBeginIterator = updateOutput.begin() + static_cast<int>(offset + key.length());
    if(*partBeginIterator != ',') {
        return "";
    }
    ++partBeginIterator;
    const auto partEndIterator = std::find(partBeginIterator, updateOutput.end(), ';');
    std::string fromBeginToEnd( partBeginIterator, partEndIterator);
    return fromBeginToEnd;
}

void Window::writeSaveFile(const std::string& updateOutput) {
    const std::string saveFileContent = extractSpecificPartOfOutput(updateOutput, "save");
    if(saveFileContent.empty()) {
        return;
    }

    bool foundFile = false;
    const auto searchingDirs = {"./", "bin/", "../"};
    std::ofstream outFile;
    const std::string fileName = "save/save.txt";
    for (const auto &searchingDir: searchingDirs) {
        if (!foundFile) {
            outFile.open(searchingDir + fileName, std::ofstream::out | std::ofstream::trunc);
            if (outFile) {
                foundFile = true;
            }
        }
    }
    if (!foundFile) {
        std::cerr << "ERROR: Opening " << fileName << " impossible .." << std::endl;
        JBTypesMethods::displayInstallError();
        exit(EXIT_FAILURE);
    }
    outFile << saveFileContent;
    outFile.close();
}

std::vector<unsigned char> Window::createBinaryFont() {
    const auto searchingDirs = {"./", "bin/", "../"};

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
    fontFile.close();
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

    return glfwWindowShouldClose(_window) != 0;
}


void Window::run() {

    const auto shouldLeave = [](const std::string& output) {
        return output.find("quit") != std::string::npos;
    };

    glfwSwapInterval(1);
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);

    auto before = Chronometer::getTimePointMSNow();
    unsigned int counter = 0;
    bool requestLeaving = false;
    while (!(inputManagement() || requestLeaving)) {

        const auto updateOutput = _controller.update();

        if(shouldLeave(updateOutput)){
            requestLeaving = true;
        }

        _controller.render();

        writeSaveFile(updateOutput);

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

