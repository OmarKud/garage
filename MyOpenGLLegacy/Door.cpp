//#include "Door.h"
//#include <GL/gl.h>
//#include <cmath>
//#include "Door.h"
//
//void Door::SetPose(const Vec3& hingePos, float width, float height) {
//    hinge = hingePos;
//    w = width;
//    h = height;
//}
//
//void Door::TryToggle(const Vec3& playerPos) {
//    // إذا اللاعب قريب من المفصل
//    Vec3 d = { playerPos.x - hinge.x, 0.0f, playerPos.z - hinge.z };
//    if (length(d) < 2.0f) {
//        open = !open;
//        targetAngle = open ? 90.0f : 0.0f;
//    }
//}
//
//void Door::Update(float dt) {
//    // حركة ناعمة
//    float speed = 180.0f; // deg/sec
//    if (std::fabs(angle - targetAngle) < 0.5f) { angle = targetAngle; return; }
//    angle += (angle < targetAngle ? 1.0f : -1.0f) * speed * dt;
//    if ((angle < targetAngle && angle > targetAngle) || (angle > targetAngle && angle < targetAngle))
//        angle = targetAngle;
//}
//
//AABB Door::ColliderBox() const {
//    // صندوق بسيط مكان الباب وهو مسكر (سمك بسيط)
//    float t = 0.15f;
//    return {
//        { hinge.x,     0.0f, hinge.z - t },
//        { hinge.x + w, h,    hinge.z + t }
//    };
//}
//
//void Door::Render() const {
//    glPushMatrix();
//    glTranslatef(hinge.x, hinge.y, hinge.z);
//    glRotatef(angle, 0, 1, 0);
//
//    glColor3f(0.65f, 0.45f, 0.25f);
//    glBegin(GL_QUADS);
//    // باب بسيط مستطيل على محور X (من 0 إلى w)
//    glVertex3f(0, 0, 0);
//    glVertex3f(w, 0, 0);
//    glVertex3f(w, h, 0);
//    glVertex3f(0, h, 0);
//    glEnd();
//
//    glPopMatrix();
//}
