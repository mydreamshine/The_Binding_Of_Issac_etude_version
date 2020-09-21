#pragma once

bool Collide_Rect_AABB(float minX1, float minY1, float maxX1, float maxY1, float minX2, float minY2, float maxX2, float maxY2);

bool Collide_Box_AABB(float minX1, float minY1, float minZ1, float maxX1, float maxY1, float maxZ1, float minX2, float minY2, float minZ2, float maxX2, float maxY2, float maxZ2);

bool Check_BoundaryOut(float Boundary_minX, float Boundary_minY, float Boundary_maxX, float Boundary_maxY, float Obj_minX, float Obj_minY, float Obj_maxX, float Obj_maxY);

bool IntersectRect(fRECT* intersectRect, fRECT& Rect1, fRECT& Rect2);
