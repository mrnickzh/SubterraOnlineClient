#include "PhysicsEngine.hpp"

// void calculatevelocity() {
//     for (auto e : Main::entities) {
//         auto cv = e->velocity;
//         auto cp = e->position;
//         cv->prev_velocity = cv->velocity;
//
//         cp->prevPosition = cp->position;
//         cp->position = cp->position + cv->velocity;
//
//         if (cv->velocity.x < 0) { cv->velocity.x += 0.001; }
//         if (cv->velocity.x > 0) { cv->velocity.x -= 0.001; }
//         if (cv->velocity.x < 0.001 && cv->velocity.x > -0.001) { cv->velocity.x = 0; }
//
//
//         if (cv->velocity.y < 0) { cv->velocity.y += 0.001; }
//         if (cv->velocity.y > 0) { cv->velocity.y -= 0.001; }
//         if (cv->velocity.y < 0.001 && cv->velocity.y > -0.001) { cv->velocity.y = 0; }
//     }
// }
//
// bool collision(float px, float py, float pz, Object* object1, Object* object2) {
//     bool xcheck1 = object1->collider1.x + px <= object2->collider2.x + object2->position.x;
//     bool x1check1 = object1->collider2.x + px >= object2->collider1.x + object2->position.x;
//     bool ycheck1 = object1->collider1.y + py <= object2->collider2.y + object2->position.y;
//     bool y1check1 = object1->collider2.y + py >= object2->collider1.y + object2->position.y;
//     if (xcheck1 && ycheck1 && x1check1 && y1check1) {
//         return true;
//     }
//     return false;
// }