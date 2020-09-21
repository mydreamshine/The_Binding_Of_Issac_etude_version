#include "stdafx.h"
#include "CollisionFunc.h"

bool Collide_Rect_AABB(float minX1, float minY1, float maxX1, float maxY1, float minX2, float minY2, float maxX2, float maxY2)
{
	if (maxX1 < minX2) return false;
	if (minX1 > maxX2) return false;
	if (minY1 > maxY2) return false;
	if (maxY1 < minY2) return false;
	return true;
}

bool Collide_Box_AABB(float minX1, float minY1, float minZ1, float maxX1, float maxY1, float maxZ1, float minX2, float minY2, float minZ2, float maxX2, float maxY2, float maxZ2)
{
	if (maxX1 < minX2) return false;
	if (minX1 > maxX2) return false;
	if (minY1 > maxY2) return false;
	if (maxY1 < minY2) return false;
	if (maxZ1 < minZ2) return false;
	if (minZ1 > maxZ2) return false;
	return true;
}

bool Check_BoundaryOut(float Boundary_minX, float Boundary_minY, float Boundary_maxX, float Boundary_maxY, float Obj_minX, float Obj_minY, float Obj_maxX, float Obj_maxY)
{
	if (Boundary_maxX < Obj_minX || Boundary_minX > Obj_maxX || Boundary_minY > Obj_maxY || Boundary_maxY < Obj_minY)
		return true;
	return false;
}

bool IntersectRect(fRECT* intersectRect, fRECT& Rect1, fRECT& Rect2)
{
	bool VCollision = false;
	bool HCollision = false;

	// 수평 충돌
	if (Rect1.Left < Rect2.Right && Rect1.Right > Rect2.Left)
	{
		HCollision = true;
		intersectRect->Left = (Rect1.Left > Rect2.Left) ? Rect1.Left : Rect2.Left;
		intersectRect->Right = (Rect1.Right < Rect2.Right) ? Rect1.Right : Rect2.Right;
	}
	// 수직 충돌
	if (Rect1.Top > Rect2.Bottom && Rect1.Bottom < Rect2.Top)
	{
		VCollision = true;
		intersectRect->Top = (Rect1.Top < Rect2.Top) ? Rect1.Top : Rect2.Top;
		intersectRect->Bottom = (Rect1.Bottom > Rect2.Bottom) ? Rect1.Bottom : Rect2.Bottom;
	}
	return VCollision && HCollision;
}