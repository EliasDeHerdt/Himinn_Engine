#pragma once

namespace Himinn
{
	struct IVector2
	{
		int x, y;

		bool operator==(const IVector2& rhs) const
		{
			return (x == rhs.x && y == rhs.y);
		}
		
		bool operator!=(const IVector2& rhs) const
		{
			return (x != rhs.x || y != rhs.y);
		}
	};

	enum class QBertDirection
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight
	};

	
}