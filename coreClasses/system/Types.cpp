#include "Types.h"

glm::vec3 JBTypesMethods::directionAsVector(JBTypes::Dir dir) {

    glm::vec3 dirVec3{0.f, 0.f, 0.f};
    switch (dir) {
        case JBTypes::Dir::North:
            dirVec3.z = -1.f;
            break;
        case JBTypes::Dir::South:
            dirVec3.z = 1.f;
            break;
        case JBTypes::Dir::East:
            dirVec3.x = 1.f;
            break;
        case JBTypes::Dir::West:
            dirVec3.x = -1.f;
            break;
        case JBTypes::Dir::Up:
            dirVec3.y = 1.f;
            break;
        case JBTypes::Dir::Down:
            dirVec3.y = -1.f;
            break;
        default:
            break;
    }

    return dirVec3;
}

glm::i32vec3 JBTypesMethods::directionAsVectorInt(JBTypes::Dir dir) {

    glm::i32vec3 dirVec3{0, 0, 0};
    switch (dir) {
        case JBTypes::Dir::North:
            dirVec3.z = -1;
            break;
        case JBTypes::Dir::South:
            dirVec3.y = 1;
            break;
        case JBTypes::Dir::East:
            dirVec3.x = 1;
            break;
        case JBTypes::Dir::West:
            dirVec3.x = -1;
            break;
        case JBTypes::Dir::Up:
            dirVec3.y = 1;
            break;
        case JBTypes::Dir::Down:
            dirVec3.y = -1;
            break;
        default:
            break;
    }

    return dirVec3;
}


glm::vec3 JBTypesMethods::rotationVectorUpToDir(const JBTypes::Dir &dir) {

    switch (dir) {
        case JBTypes::Dir::North:
            return {-JBTypes::pi2, 0.f, 0.f};
        case JBTypes::Dir::South:
            return {JBTypes::pi2, 0.f, 0.f};
        case JBTypes::Dir::East:
            return {0.f, 0.f, -JBTypes::pi2};
        case JBTypes::Dir::West:
            return {0.f, 0.f, JBTypes::pi2};
        case JBTypes::Dir::Up:
            return {0.0f, 0.0f, 0.0f};
        case JBTypes::Dir::Down:
            return {JBTypes::pi, 0.f, 0.f};
        default:
            return {0.0f, 0.0f, 0.0f};
    }
}


JBTypes::Dir JBTypesMethods::integerAsDirection(unsigned int number) {
    JBTypes::Dir dir;
    switch (number) {
        case 0:
            dir = JBTypes::Dir::North;
            break;
        case 1:
            dir = JBTypes::Dir::South;
            break;
        case 2:
            dir = JBTypes::Dir::East;
            break;
        case 3:
            dir = JBTypes::Dir::West;
            break;
        case 4:
            dir = JBTypes::Dir::Up;
            break;
        case 5:
            dir = JBTypes::Dir::Down;
            break;
        default:
            dir = JBTypes::Dir::North;
            break;
    }

    return dir;
}

unsigned int JBTypesMethods::directionAsInteger(JBTypes::Dir dir) {

    unsigned int number;
    switch (dir) {
        case JBTypes::Dir::North:
            number = 0;
            break;
        case JBTypes::Dir::South:
            number = 1;
            break;
        case JBTypes::Dir::East:
            number = 2;
            break;
        case JBTypes::Dir::West:
            number = 3;
            break;
        case JBTypes::Dir::Up:
            number = 4;
            break;
        case JBTypes::Dir::Down:
            number = 5;
            break;
        default:
            number = 0;
            break;
    }
    return number;
}

JBTypes::Dir JBTypesMethods::vectorAsDirection(const glm::vec3& vec) {
    if (
        vec.x - 1.f < EPSILON_F && vec.x - 1.f > -EPSILON_F &&
        vec.y < EPSILON_F && vec.y > -EPSILON_F &&
        vec.z < EPSILON_F && vec.z > -EPSILON_F
        )
        return JBTypes::Dir::East;
    else if (
        vec.x + 1.f < EPSILON_F && vec.x + 1.f > -EPSILON_F &&
        vec.y < EPSILON_F && vec.y > -EPSILON_F &&
        vec.z < EPSILON_F && vec.z > -EPSILON_F
        )
        return JBTypes::Dir::West;
    else if (
        vec.y - 1.f < EPSILON_F && vec.y - 1.f > -EPSILON_F &&
        vec.x < EPSILON_F && vec.x > -EPSILON_F &&
        vec.z < EPSILON_F && vec.z > -EPSILON_F
        )
        return JBTypes::Dir::Up;
    else if (
        vec.y + 1.f < EPSILON_F && vec.y + 1.f > -EPSILON_F &&
        vec.x < EPSILON_F && vec.x > -EPSILON_F &&
        vec.z < EPSILON_F && vec.z > -EPSILON_F
        )
        return JBTypes::Dir::Down;
    else if (
        vec.z - 1.f < EPSILON_F && vec.z - 1.f > -EPSILON_F &&
        vec.x < EPSILON_F && vec.x > -EPSILON_F &&
        vec.y < EPSILON_F && vec.y > -EPSILON_F
        )
        return JBTypes::Dir::South;
    else if (
        vec.z + 1.f < EPSILON_F && vec.z + 1.f > -EPSILON_F &&
        vec.x < EPSILON_F && vec.x > -EPSILON_F &&
        vec.y < EPSILON_F && vec.y > -EPSILON_F
        )
        return JBTypes::Dir::North;
    else {
        std::cerr << "Warning : your vector does not mean any direction"
                  << std::endl << "x: " << vec.x << " y: " << vec.y << " z: " << vec.z
                  << " ... return North ... " << std::endl;
        return JBTypes::Dir::North;
    }
}

void JBTypesMethods::displayInstallError() {
    std::cerr << "Did you forget to install the project ?" << std::endl <<
              "Run \"cmake --build . --target install\" " <<
              "or \"make install\"" << std::endl;
}

std::string JBTypesMethods::colorToString(const JBTypes::Color &color) {
    switch (color) {
        case JBTypes::Color::Blue:
            return "Blue";
        case JBTypes::Color::Red:
            return "Red";
        case JBTypes::Color::Yellow:
            return "Yellow";
        case JBTypes::Color::Green:
            return "Green";
        case JBTypes::Color::Orange:
            return "Orange";
        case JBTypes::Color::White:
            return "White";
        case JBTypes::Color::Black:
            return "Black";
        case JBTypes::Color::None:
            return "None";
        case JBTypes::Color::Purple:
            return "Purple";
        case JBTypes::Color::ShinyRed:
            return "Red";
        case JBTypes::Color::ShinyGreen:
            return "Green";
        case JBTypes::Color::ShinyBlue:
            return "Blue";
        default:
            return "None";
    }
    return "None";
}

JBTypes::Dir JBTypesMethods::charAsDirection(unsigned char dirChar) {

    JBTypes::Dir dir;

    switch (dirChar) {
        case 'N':
            dir = JBTypes::Dir::North;
            break;
        case 'S':
            dir = JBTypes::Dir::South;
            break;
        case 'E':
            dir = JBTypes::Dir::East;
            break;
        case 'W':
            dir = JBTypes::Dir::West;
            break;
        case 'U':
            dir = JBTypes::Dir::Up;
            break;
        case 'D':
            dir = JBTypes::Dir::Down;
            break;
        default:
            std::cerr << "Unknown direction character " << dirChar << std::endl;
            dir = JBTypes::Dir::North;
            break;
    }
    return dir;
}

JBTypes::Color JBTypesMethods::charAsColor(unsigned char charColor) {

    JBTypes::Color color;
    switch (charColor) {
        case 'R':
            color = JBTypes::Color::Red;
            break;
        case 'G':
            color = JBTypes::Color::Green;
            break;
        case 'B':
            color = JBTypes::Color::Blue;
            break;
        case 'Y':
            color = JBTypes::Color::Yellow;
            break;
        default:
            std::cerr << "Unknown color character " << charColor << std::endl;
            color = JBTypes::Color::None;
            break;
    }
    return color;
}

JBTypes::Color JBTypesMethods::colorToShiny(const JBTypes::Color &color) {
    if (color == JBTypes::Color::Red) {
        return JBTypes::Color::ShinyRed;
    }
    if (color == JBTypes::Color::Green) {
        return JBTypes::Color::ShinyGreen;
    }
    if (color == JBTypes::Color::Blue) {
        return JBTypes::Color::ShinyBlue;
    }
    return JBTypes::Color::None;
}

std::array<bool, 6> JBTypesMethods::strDirAsArray(const std::string &directions) {
    std::array<bool, 6> dirsArray = {false, false, false, false, false, false};
    for (unsigned char dir: directions) {
        switch (dir) {
            case 'N' : {
                dirsArray.at(0) = true;
                break;
            }
            case 'S' : {
                dirsArray.at(1) = true;
                break;
            }
            case 'E' : {
                dirsArray.at(2) = true;
                break;
            }
            case 'W' : {
                dirsArray.at(3) = true;
                break;
            }
            case 'U' : {
                dirsArray.at(4) = true;
                break;
            }
            case 'D' : {
                dirsArray.at(5) = true;
                break;
            }
            default:
                break;
        }
    }
    return dirsArray;
}

bool JBTypesMethods::floatsEqual(float a, float b) {
    const float diff = a - b;
    return diff < EPSILON_F && diff > -EPSILON_F;
}
