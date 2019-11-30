#pragma once
#include "Definitions.h"

class Interpolator
{
public:
	Interpolator(Point p0, Point p1, Point p2);
	void computeWeights();
	bool isContained();
	void interpolate();
	void setPointOfInterest(Point& point);
private:
	Point m_p0;
	Point m_p1;
	Point m_p2;

	v3f m_weights = v3f(-1);

	Point* m_point_of_interest = NULL;

	float m_precomputed_denominator;
	v2f m_precomputed_weight_0;
	v2f m_precomputed_weight_1;



	glm::mat3 m_rotation_matrix;
	v3f m_ws_p0;
	v3f m_ws_p1;
	v3f m_ws_p2;

	v3f m_ws_weights = v3f(-1);

	v3f m_ws_point_of_interest;
	
	float m_ws_precomputed_denominator;
	v2f m_ws_precomputed_weight_0;
	v2f m_ws_precomputed_weight_1;
};

