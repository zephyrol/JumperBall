#include "Types.h"

JBTypes::vec3f JBTypesMethods::directionAsVector (JBTypes::Dir dir) {

    JBTypes::vec3f dirVec3 { 0.f, 0.f, 0.f };
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

JBTypes::Dir JBTypesMethods::integerAsDirection (unsigned int number) {
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


JBTypes::timePointMs JBTypesMethods::getTimePointMSNow() noexcept{
    return std::chrono::time_point_cast <std::chrono::milliseconds>
               (std::chrono::system_clock::now());
}

float JBTypesMethods::getTimeSecondsSinceTimePoint (
    const JBTypes::timePointMs& timePoint) noexcept{

    const JBTypes::timePointMs timeNowMs
        = JBTypesMethods::
          getTimePointMSNow();
    const JBTypes::durationMs timeNowSinceEpoch
        = timeNowMs.time_since_epoch();

    const JBTypes::timePointMs timeActionMs
        = timePoint;
    const JBTypes::durationMs timeActionSinceEpoch
        = timeActionMs.time_since_epoch();

    const JBTypes::durationMs difference
        = timeNowSinceEpoch -
          timeActionSinceEpoch;
    const std::chrono::duration <float> durationFloatDifference
        = difference;
    const float fDifference = durationFloatDifference.count();

    return fDifference;
}

float JBTypesMethods::getFloatFromDurationMS (
    const JBTypes::durationMs& dms) {
    const std::chrono::duration <float> durationFloatDifference = dms;
    const float fDifference = durationFloatDifference.count();
    return fDifference;
}

JBTypes::timePointMs JBTypesMethods::getTimePointMsFromTimePoint
    (const std::chrono::time_point <std::chrono::system_clock>& timePoint) noexcept{

    return std::chrono::time_point_cast <std::chrono::milliseconds>(timePoint);
}

unsigned int JBTypesMethods::directionAsInteger (JBTypes::Dir dir) {

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

JBTypes::vec3f JBTypesMethods::cross (const JBTypes::vec3f& a, const JBTypes::vec3f& b) {
    JBTypes::vec3f crossProduct;
    crossProduct.x = a.y * b.z - a.z * b.y;
    crossProduct.y = a.z * b.x - a.x * b.z;
    crossProduct.z = a.x * b.y - a.y * b.x;
    return crossProduct;
}

float JBTypesMethods::dot (const JBTypes::vec3f& a, const JBTypes::vec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float JBTypesMethods::distance (const JBTypes::vec3f& a,
                                const JBTypes::vec3f& b) {
    return sqrtf(powf(b.x - a.x, 2.f) +
                 powf(b.y - a.y, 2.f) +
                 powf(b.z - a.z, 2.f)
                 );
}

JBTypes::Dir JBTypesMethods::vectorAsDirection (const JBTypes::vec3f& vec) {
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
#ifdef _MSC_VER
    std::cerr << "Did you forget to install the files ?" << std::endl
              << "On Visual Studio, build the INSTALL solution" << std::endl;
#else
    std::cerr << "Did you forget to install the project ?" << std::endl <<
        "Run \"cmake --build . --target install\" " <<
        "or \"make install\"" << std::endl;
#endif
}

JBTypes::vec3f JBTypesMethods::add (const JBTypes::vec3f& a,
                                    const JBTypes::vec3f& b) {
    return JBTypes::vec3f { a.x + b.x, a.y + b.y, a.z + b.z };
}

JBTypes::vec3f JBTypesMethods::scalarApplication (float scalar,
                                                  const JBTypes::vec3f& vec) {
    return JBTypes::vec3f { scalar*vec.x, scalar*vec.y, scalar*vec.z };
}

float JBTypesMethods::length (const JBTypes::vec3f& vec) {
    return sqrtf(powf(vec.x, 2.f) + powf(vec.y, 2.f) + powf(vec.z, 2.f));
}

JBTypes::vec3f JBTypesMethods::normalize (const JBTypes::vec3f& vec) {
    const float vecLength = JBTypesMethods::length(vec);
    return { vec.x / vecLength, vec.y / vecLength, vec.z / vecLength };
}

std::string JBTypesMethods::colorToString (const JBTypes::Color color) {
    switch (color) {
    case JBTypes::Color::Blue: return "Blue";
    case JBTypes::Color::Red: return "Red";
    case JBTypes::Color::Yellow: return "Yellow";
    case JBTypes::Color::Green: return "Green";
    case JBTypes::Color::None: return "None";
    default: return "None";
    }
    return "None";
}

JBTypes::Quaternion JBTypesMethods::multiply (const JBTypes::Quaternion& q1, const JBTypes::Quaternion& q2) {
    const JBTypes::vec3f v1xv2 = JBTypesMethods::cross(q1.v, q2.v);
    const float v1dotv2 = JBTypesMethods::dot(q1.v, q2.v);
    const JBTypes::vec3f s1v2 = JBTypesMethods::scalarApplication(q1.w, q2.v);
    const JBTypes::vec3f s2v1 = JBTypesMethods::scalarApplication(q2.w, q1.v);
    const float s1s2 = q1.w * q2.w;

    return JBTypesMethods::createQuaternion(
        JBTypesMethods::add(JBTypesMethods::add(v1xv2, s1v2), s2v1),
        s1s2 - v1dotv2
        );
}

JBTypes::Quaternion JBTypesMethods::q2q1 (
    const JBTypes::Quaternion& q1q2,
    const JBTypes::Quaternion& q1,
    const JBTypes::Quaternion& q2
    ) {
    const JBTypes::vec3f v1xv2 = JBTypesMethods::cross(q1.v, q2.v);
    return JBTypesMethods::createQuaternion(
        JBTypesMethods::add(q1q2.v, JBTypesMethods::scalarApplication(-2.f, v1xv2)),
        q1q2.w
        );
}

JBTypes::Quaternion JBTypesMethods::inverse (const JBTypes::Quaternion& q) {
    const JBTypes::vec3f& v = q.v;
    const float v2PlusS2 = v.x * v.x + v.y * v.y + v.z * v.z + q.w * q.w;

    return JBTypesMethods::createQuaternion(
        { -v.x / v2PlusS2, -v.y / v2PlusS2, -v.z / v2PlusS2 },
        q.w / v2PlusS2
        );
}

JBTypes::Quaternion JBTypesMethods::createQuaternion (const JBTypes::vec3f& v, float w) {
    JBTypes::Quaternion q;
    q.v = v;
    q.w = w;
    return q;
}

JBTypes::Quaternion JBTypesMethods::createRotationQuaternion (const JBTypes::vec3f& axis, float angle) {
    return JBTypesMethods::createQuaternion(
        JBTypesMethods::scalarApplication(sinf(angle / 2.f), axis),
        cosf(angle / 2.f)
        );
}

JBTypes::vec3f JBTypesMethods::rotateVector (const JBTypes::vec3f& v, const JBTypes::Quaternion& q) {
    const auto& qv = q.v;
    const float twoVDotQv = 2.f * JBTypesMethods::dot(v, qv);
    const float qvDotQv = JBTypesMethods::dot(qv, qv);
    const JBTypes::vec3f qvCrossV = JBTypesMethods::cross(qv, v);
    const float qwqwMinusBtwo = q.w * q.w - qvDotQv;

    const JBTypes::vec3f vPart = JBTypesMethods::scalarApplication(qwqwMinusBtwo, v);
    const JBTypes::vec3f qvPart = JBTypesMethods::scalarApplication(twoVDotQv, qv);
    const JBTypes::vec3f qvCrosVPart = JBTypesMethods::scalarApplication(2.f * q.w, qvCrossV);

    return JBTypesMethods::add(JBTypesMethods::add(vPart, qvPart), qvCrosVPart);
}

template<typename K, typename V>  std::map <K, V>
JBTypesMethods::createMap (std::vector <K> keys, std::vector <V> values) {
    std::map <K, V> mapKV;
    for (size_t i = 0; i < keys.size(); ++i) {
        mapKV[keys.at(i)] = values.at(i);
    }
    return mapKV;
}
