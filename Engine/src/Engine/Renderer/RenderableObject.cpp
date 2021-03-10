#include "egpch.h"
#include "RenderableObject.h"

namespace Engine {

	bool RenderableObject::operator()(const RenderableObject& left, const RenderableObject& right) const
	{
		if (left.Material < right.Material)
			return true;
		if (left.Material > right.Material)
			return false;

		if (left.VertexArray < right.VertexArray)
			return true;
		if (left.VertexArray > right.VertexArray)
			return false;

		auto temp = right.Transform - left.Transform;

		if (temp[3][0] < 0)
			return true;
		if (temp[3][0] > 0)
			return false;

		if (temp[3][1] < 0)
			return true;
		if (temp[3][1] > 0)
			return false;

		if (temp[3][2] < 0)
			return true;
		if (temp[3][2] > 0)
			return false;

		if (temp[3][3] < 0)
			return true;
		if (temp[3][3] > 0)
			return false;

		if (temp[2][0] < 0)
			return true;
		if (temp[2][0] > 0)
			return false;

		if (temp[2][1] < 0)
			return true;
		if (temp[2][1] > 0)
			return false;

		if (temp[2][2] < 0)
			return true;
		if (temp[2][2] > 0)
			return false;

		if (temp[2][3] < 0)
			return true;
		if (temp[2][3] > 0)
			return false;

		if (temp[1][0] < 0)
			return true;
		if (temp[1][0] > 0)
			return false;

		if (temp[1][1] < 0)
			return true;
		if (temp[1][1] > 0)
			return false;

		if (temp[1][2] < 0)
			return true;
		if (temp[1][2] > 0)
			return false;

		if (temp[1][3] < 0)
			return true;
		if (temp[1][3] > 0)
			return false;

		if (temp[0][0] < 0)
			return true;
		if (temp[0][0] > 0)
			return false;

		if (temp[0][1] < 0)
			return true;
		if (temp[0][1] > 0)
			return false;

		if (temp[0][2] < 0)
			return true;
		if (temp[0][2] > 0)
			return false;

		if (temp[0][3] < 0)
			return true;
		if (temp[0][3] > 0)
			return false;

		return false;
	}

}
