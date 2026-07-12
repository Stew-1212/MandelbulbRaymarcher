#ifndef ARCBALLCAM_HPP
#define ARCBALLCAM_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*CLASS DEFINITION */
class ArcballCam {
    public:
        //constructors and destructor
        ArcballCam();
        ArcballCam(glm::vec3 pos, glm::vec3 target, glm::mat4 projMatrix);
        ~ArcballCam() = default;

        //required methods we need
        void recomputeOrientation();
        void computeViewMatrix();
        void updateTheta(float dTheta);
        void updatePhi(float dPhi);
        void zoomIn(float zoomFactor);
        void zoomOut(float zoomFactor);
        void initFromPositionTarget(glm::vec3 pos, glm::vec3 target);

        //getters
        float getCamTheta() const;
        float getCamPhi() const;
        float getCamRadius() const;
        glm::vec3 getCamPos() const;
        glm::vec3 getCamTarget() const;
        glm::vec3 getCamUpVector() const;
        glm::mat4 getCamProjectionMatrix() const;
        glm::mat4 getCamViewMatrix() const;

        //setters
        void setCamTheta(float newTheta);
        void setCamPhi(float newPhi);
        void setCamRadius(float newRadius);
        void setCamPos(glm::vec3& newPos);
        void setCamTarget(glm::vec3& newTarget);
        void setCamUpVector(glm::vec3& newUpVector);
        void setCamProjectionMatrix(glm::mat4& newProjectionMatrix);
        void setCamViewMatrix(glm::mat4& newViewMatrix);


    private:
        //camera attributes
        float _camTheta;
        float _camPhi;
        float _camRadius;
        glm::vec3 _camPos;
        glm::vec3 _camTarget; //i.e look at point
        glm::vec3 _camUpVector;
        glm::mat4 _camProjectionMatrix;
        glm::mat4 _camViewMatrix;
};


/*BEGIN ACTUAL IMPLEMENTATION*/
inline ArcballCam::ArcballCam() :
    _camProjectionMatrix(glm::mat4(1.f)),
    _camViewMatrix(glm::mat4(1.f)),
    _camPos(glm::vec3(0.f, 0.f, 0.f)),
    _camTarget(glm::vec3(0.f, 0.f, -1.f)),
    _camUpVector(glm::vec3(0.f, 1.f, 0.f)),
    _camTheta(0.0f),
    _camPhi(glm::pi<float>() / 2.0f),
    _camRadius(1.f) {}

inline ArcballCam::ArcballCam(glm::vec3 pos, glm::vec3 target, glm::mat4 projMatrix) :
    _camProjectionMatrix(projMatrix),
    _camViewMatrix(glm::mat4(1.f)),
    _camTarget(target),
    _camUpVector(glm::vec3(0.f, 1.f, 0.f)) {
        //convert the given position to spherical coordinates
        initFromPositionTarget(pos, target);
    }

inline void ArcballCam::initFromPositionTarget(glm::vec3 pos, glm::vec3 target) {
    //set pos and target
    _camPos = pos;
    _camTarget = target;
    //compute vector from target to position
    glm::vec3 dir = pos - target;
    //set radius to the length of the vector
    _camRadius = glm::length(dir);
    if (_camRadius > 0.0f) {
        //normalize direction
        dir = dir / _camRadius;
        //compute phi and theta
        _camPhi = glm::acos(-dir.y);
        _camTheta = glm::atan(dir.z, dir.x);
    } else {
        //otherwise use default values
        _camPhi = glm::pi<float>() / 2.0f;
        _camTheta = 0.0f;
    }
    recomputeOrientation();
}

inline void ArcballCam::recomputeOrientation() {
    glm::vec3 direction;
    //convert spherical to Cartestian
    direction.x = glm::sin(_camTheta) * glm::sin(_camPhi) * _camRadius;
    direction.y = -glm::cos(_camPhi) * _camRadius;
    direction.z = -glm::cos(_camTheta) * glm::sin(_camPhi) * _camRadius;

    _camPos = _camTarget + direction;
    _camUpVector = glm::vec3(0.f, 1.f, 0.f);
    computeViewMatrix();
}

inline void ArcballCam::computeViewMatrix() {
    _camViewMatrix = glm::lookAt(_camPos, _camTarget, _camUpVector);
}

inline void ArcballCam::updateTheta(float dTheta) {
    _camTheta += dTheta;
    recomputeOrientation();
}

inline void ArcballCam::updatePhi(float dPhi) {
    _camPhi += dPhi;
    //clamp phi to avoid flipping upside down
    const float epsilon = 0.001f;
    _camPhi = glm::clamp(_camPhi, epsilon, glm::pi<float>() - epsilon);
    recomputeOrientation();
}

inline void ArcballCam::zoomIn(float zoomFactor) {
    //reduce radius
    _camRadius -= zoomFactor;
    if (_camRadius < 0.1f) _camRadius = 0.1f; //prevent flipping
    recomputeOrientation();
}

inline void ArcballCam::zoomOut(float zoomFactor) {
    //increase radius
    _camRadius += zoomFactor;
    recomputeOrientation();
}

inline float ArcballCam::getCamTheta() const {
    return _camTheta;
}
        
inline float ArcballCam::getCamPhi() const {
    return _camPhi;
}

inline float ArcballCam::getCamRadius() const {
    return _camRadius;
}

inline glm::vec3 ArcballCam::getCamPos() const {
    return _camPos;
}

inline glm::vec3 ArcballCam::getCamTarget() const {
    return _camTarget;
}

inline glm::vec3 ArcballCam::getCamUpVector() const {
    return _camUpVector;
}

inline glm::mat4 ArcballCam::getCamProjectionMatrix() const {
    return _camProjectionMatrix;
}

inline glm::mat4 ArcballCam::getCamViewMatrix() const {
    return _camViewMatrix;
}

inline void ArcballCam::setCamTheta(float newTheta) {
    _camTheta = newTheta;
}

inline void ArcballCam::setCamPhi(float newPhi) {
    _camPhi = newPhi;
}

inline void ArcballCam::setCamRadius(float newRadius) {
    _camRadius = newRadius;
}
        
inline void ArcballCam::setCamPos(glm::vec3& newPos) {
    _camPos = newPos;
}
        
inline void ArcballCam::setCamTarget(glm::vec3& newTarget) {
    _camTarget = newTarget;
}
        
inline void ArcballCam::setCamUpVector(glm::vec3& newUpVector) {
    _camUpVector = newUpVector;
}
        
inline void ArcballCam::setCamProjectionMatrix(glm::mat4& newProjectionMatrix) {
    _camProjectionMatrix = newProjectionMatrix;
}
        
inline void ArcballCam::setCamViewMatrix(glm::mat4& newViewMatrix) {
    _camViewMatrix = newViewMatrix;
}



#endif