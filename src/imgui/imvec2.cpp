#include "imvec2.h"

ImVec2 operator-(const ImVec2& l, const ImVec2& r) { return{ l.x - r.x, l.y - r.y }; }
ImVec2 operator+(const ImVec2& l, const ImVec2& r) { return{ l.x + r.x, l.y + r.y }; }
ImVec2 operator*(const ImVec2& l, const ImVec2& r) { return{ l.x * r.x, l.y * r.y }; }
ImVec2 operator*(const ImVec2& l, const float& scalar) { return{ l.x * scalar, l.y * scalar }; }
ImVec2 operator/(const ImVec2& l, const float& scalar) { return{ l.x / scalar, l.y / scalar }; }