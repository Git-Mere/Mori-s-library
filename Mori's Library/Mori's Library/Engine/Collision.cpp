#include "Collision.h"

void Collision::Set_color(const vec3& c)
{
	color = c;
}

bool Collision::Point_Point(vec2 point1, vec2 point2)
{
	int check_intx1 = static_cast<int>(point1.x);
	int check_inty1 = static_cast<int>(point1.y);
	int check_intx2 = static_cast<int>(point2.x);
	int check_inty2 = static_cast<int>(point2.y);
	if (check_intx1 == check_intx2 && check_inty1 == check_inty2) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Point_Line(vec2 point1, Math::Line line)
{
	float d1 = Math::dist(point1, line.point_1);
	float d2 = Math::dist(point1, line.point_2);

	float lineLen = Math::dist(line.point_1, line.point_2);

	//부동소수점 계산 오차를 방지하기 위한 버퍼
	float buffer = 0.1f; 

	if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Point_Circle(vec2 point1, vec2 circle_point, float radius)
{
	float distX = point1.x - circle_point.x;
	float distY = point1.y - circle_point.y;
	float distance = (distX * distX) + (distY * distY);

	float r = radius * radius;

	if (distance <= r) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Point_Rect(vec2 point1, Math::rect rect1)
{
	if (point1.x >= rect1.Left() &&
		point1.x <= rect1.Right() &&
		point1.y >= rect1.Bottom() &&
		point1.y <= rect1.Top()) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Point_Poly(vec2 point1, std::vector<vec2> vertices)
{
	bool collision = false;

	float px = point1.x;
	float py = point1.y;
	// go through each of the vertices, plus
	// the next vertex in the list
	int next = 0;
	for (int current = 0; current < vertices.size(); current++) {

		// get next vertex in list
		// if we've hit the end, wrap around to 0
		next = current + 1;
		if (next == vertices.size()) next = 0;

		// get the PVectors at our current position
		// this makes our if statement a little cleaner
		vec2 vc = vertices[current];    // c for "current"
		vec2 vn = vertices[next];       // n for "next"

		// compare position, flip 'collision' variable
		// back and forth
		if (((vc.y >= py && vn.y < py) || (vc.y < py && vn.y >= py)) &&
			(px < (vn.x - vc.x) * (py - vc.y) / (vn.y - vc.y) + vc.x)) {
			collision = !collision;
		}
	}
	if (collision == true) {
		Set_color(yes_color);
	}
	else {
		Set_color(no_color);
	}
	return collision;
}


bool Collision::Line_Line(Math::Line line1, Math::Line line2)
{
	///////line 1
	float x1 = line1.point_1.x;
	float y1 = line1.point_1.y;

	float x2 = line1.point_2.x;
	float y2 = line1.point_2.y;

	///////line 2
	float x3 = line2.point_1.x;
	float y3 = line2.point_1.y;

	float x4 = line2.point_2.x;
	float y4 = line2.point_2.y;

	float denominator = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

	if (denominator != 0) {
		float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator;
		float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;

		if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
			Set_color(yes_color);
			return true;  
		}
	}
	Set_color(no_color);
	return false;
}

bool Collision::Line_Circle(Math::Line line1, vec2 circle_point, float radius)
{

	float cx = circle_point.x;
	float cy = circle_point.y;
	float x1 = line1.point_1.x;
	float y1 = line1.point_1.y;
	float x2 = line1.point_2.x;
	float y2 = line1.point_2.y;

	bool inside1 = Point_Circle(line1.point_1, circle_point, radius);
	bool inside2 = Point_Circle(line1.point_2, circle_point, radius);
	if (inside1 || inside2) { Set_color(yes_color); return true; };

	float len = Math::dist(line1.point_1, line1.point_2);

	// get dot product of the line and circle
	float dot = (((cx - x1) * (x2 - x1)) + ((cy - y1) * (y2 - y1))) / len;

	// find the closest point on the line
	float closestX = x1 + (dot * (x2 - x1));
	float closestY = y1 + (dot * (y2 - y1));
	vec2 closest(closestX, closestY);

	bool onSegment = Point_Line(closest, line1);
	if (!onSegment) { Set_color(no_color); return false; }

	// get distance to closest point

	float distance = Math::dist(closest, circle_point);
	float r = radius * radius;

	if (distance <= r) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Line_Rect(Math::Line line1, Math::rect rect1)
{
	Math::Line left(vec2(rect1.Left(), rect1.Bottom()), vec2(rect1.Left(), rect1.Top()));
	Math::Line right(vec2(rect1.Right(), rect1.Bottom()), vec2(rect1.Right(), rect1.Top()));
	Math::Line top(vec2(rect1.Left(), rect1.Top()), vec2(rect1.Right(), rect1.Top()));
	Math::Line bottom(vec2(rect1.Left(), rect1.Bottom()), vec2(rect1.Right(), rect1.Bottom()));


	bool left_side = Line_Line(line1, left);
	bool right_side = Line_Line(line1, right);
	bool top_side = Line_Line(line1, top);
	bool bottom_side = Line_Line(line1, bottom);


	if (left_side || right_side || top_side || bottom_side) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Line_Poly(Math::Line line1, std::vector<vec2> vertices)
{
	// go through each of the vertices, plus the next
// vertex in the list
	int next = 0;
	for (int current = 0; current < vertices.size(); current++) {

		// get next vertex in list
		// if we've hit the end, wrap around to 0
		next = current + 1;
		if (next == vertices.size()) next = 0;

		// get the PVectors at our current position
		// extract X/Y coordinates from each
		Math::Line line2(vertices[current], vertices[next]);

		// do a Line/Line comparison
		// if true, return 'true' immediately and
		// stop testing (faster)
		bool hit = Line_Line(line1, line2);
		if (hit) {
			Set_color(yes_color);
			return true;
		}
	}

	// never got a hit
	Set_color(no_color);
	return false;
}

bool Collision::Circle_Circle(vec2 circle_point1, float radius1, vec2 circle_point2, float radius2)
{
	float distance = Math::dist(circle_point1 , circle_point2);

	float r = radius1 + radius2;
	 r *= r;

	if (distance <= r) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Circle_Rect(vec2 circle_point1, float radius1, Math::rect rect1)
{
	// temporary variables to set edges for testing
	float cx = circle_point1.x;
	float cy = circle_point1.y;

	float testX = cx;
	float testY = cy;

	// which edge is closest?
	if (cx < rect1.Left()) { testX = rect1.Left(); }     // test left edge
	else if (cx > rect1.Right()) { testX = rect1.Right(); }   // right edge
	if (cy < rect1.Bottom()) { testY = rect1.Bottom(); }      // top edge
	else if (cy > rect1.Top()) { testY = rect1.Top(); }  // bottom edge


	vec2 test(testX, testY);
	float distance = Math::dist(circle_point1 , test);

	float radius = radius1 * radius1;
	// if the distance is less than the radius, collision!
	if (distance <= radius) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Circle_Poly(vec2 circle_point1, float radius1, std::vector<vec2> vertices)
{
	// go through each of the vertices, plus
 // the next vertex in the list
	int next = 0;
	for (int current = 0; current < vertices.size(); current++) {

		// get next vertex in list
		// if we've hit the end, wrap around to 0
		next = current + 1;
		if (next == vertices.size()) next = 0;

		// get the PVectors at our current position
		// this makes our if statement a little cleaner

		Math::Line line2(vertices[current], vertices[next]);
		// check for collision between the circle and
		// a line formed between the two vertices
		bool collision = Line_Circle(line2, circle_point1, radius1);
		if (collision) {
			Set_color(yes_color);
			return true;
		};
	}

	 bool centerInside = Point_Poly(circle_point1, vertices);
	 if (centerInside) {
		 Set_color(yes_color);
		 return true;
	 };

	// otherwise, after all that, return false
	 Set_color(no_color);
	return false;
}

bool Collision::Rect_Rect(Math::rect rect1, Math::rect rect2)
{
	if (rect1.Left() < rect2.Right() && rect1.Right() > rect2.Left()
		&& rect1.Top() > rect2.Bottom() && rect1.Bottom() < rect2.Top()) {
		Set_color(yes_color);
		return true;
	}
	Set_color(no_color);
	return false;
}

bool Collision::Rect_Poly(Math::rect rect1, std::vector<vec2> vertices)
{
	// go through each of the vertices, plus the next
 // vertex in the list
	int next = 0;
	for (int current = 0; current < vertices.size(); current++) {

		// get next vertex in list
		// if we've hit the end, wrap around to 0
		next = current + 1;
		if (next == vertices.size()) next = 0;

		// get the PVectors at our current position
		// this makes our if statement a little cleaner

		Math::Line line2(vertices[current], vertices[next]);
		// check against all four sides of the rectangle
		bool collision = Line_Rect(line2, rect1);
		if (collision) {
			Set_color(yes_color);
			return true;
		};

		// optional: test if the rectangle is INSIDE the polygon
		// note that this iterates all sides of the polygon
		// again, so only use this if you need to
		vec2 center(rect1.Left() + (rect1.Size().x / 2), rect1.Bottom() + (rect1.Size().y / 2));
		bool inside = Point_Poly(center, vertices);
		if (inside) {
			Set_color(yes_color);
			return true;
		};
	}
	Set_color(no_color);
	return false;
}

bool Collision::Poly_Poly(std::vector<vec2> vertices, std::vector<vec2> vertices2)
{
	// go through each of the vertices, plus the next
 // vertex in the list
	int next = 0;
	for (int current = 0; current < vertices.size(); current++) {

		// get next vertex in list
		// if we've hit the end, wrap around to 0
		next = current + 1;
		if (next == vertices.size()) next = 0;

		// get the PVectors at our current position
		// this makes our if statement a little cleaner

		Math::Line line2(vertices[current], vertices[next]);
		// now we can use these two points (a line) to compare
		// to the other polygon's vertices using polyLine()
		bool collision = Line_Poly(line2, vertices2);
		if (collision) {
			Set_color(yes_color);
			return true;
		};

		// optional: check if the 2nd polygon is INSIDE the first
		collision = Point_Poly(vertices2[current], vertices);
		if (collision) {
			Set_color(yes_color);
			return true;
		};
	}
	Set_color(no_color);
	return false;
}
