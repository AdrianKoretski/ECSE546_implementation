#include "Interpolator.h"

extern bool use_correct_interpolation;

Interpolator::Interpolator(Point p0, Point p1, Point p2)
{
	m_p0 = p0;
	m_p1 = p1;
	m_p2 = p2;

	m_precomputed_denominator =
		(p1.position.y - p2.position.y)
		* (p0.position.x - p2.position.x)
		+ (p2.position.x - p1.position.x)
		* (p0.position.y - p2.position.y);


	m_precomputed_weight_0 = v2f(
		p1.position.y - p2.position.y,
		p2.position.x - p1.position.x
	);

	m_precomputed_weight_1 = v2f(
		p2.position.y - p0.position.y,
		p0.position.x - p2.position.x
	);
	
	m_ws_p0 = v3f(p0.position.x / p0.position.w, p0.position.y / p0.position.w, 1.f / p0.position.w);
	m_ws_p1 = v3f(p1.position.x / p1.position.w, p1.position.y / p1.position.w, 1.f / p1.position.w);
	m_ws_p2 = v3f(p2.position.x / p2.position.w, p2.position.y / p2.position.w, 1.f / p2.position.w);


	v3f t = glm::normalize(m_ws_p1 - m_ws_p0);
	v3f n = glm::normalize(glm::cross(t, m_ws_p2 - m_ws_p0));
	v3f u = glm::normalize(glm::cross(t, n));

	m_rotation_matrix = glm::mat3{
		t.x, t.y, t.z,
		u.x, u.y, u.z,
		n.x, n.y, n.z
	};
	m_rotation_matrix = glm::inverse(m_rotation_matrix);

	m_ws_p0 = m_rotation_matrix * m_ws_p0;
	m_ws_p1 = m_rotation_matrix * m_ws_p1;
	m_ws_p2 = m_rotation_matrix * m_ws_p2;

	m_ws_precomputed_denominator =
		(m_ws_p1.y - m_ws_p2.y)
		* (m_ws_p0.x - m_ws_p2.x)
		+ (m_ws_p2.x - m_ws_p1.x)
		* (m_ws_p0.y - m_ws_p2.y);

	m_ws_precomputed_weight_0 = v2f(
		m_ws_p1.y - m_ws_p2.y,
		m_ws_p2.x - m_ws_p1.x
	);
	m_ws_precomputed_weight_1 = v2f(
		m_ws_p2.y - m_ws_p0.y,
		m_ws_p0.x - m_ws_p2.x
	);
}

void Interpolator::setPointOfInterest(Point& point)
{
	m_point_of_interest = &point;
}

void Interpolator::computeWeights()
{
	m_weights.x = glm::dot(m_precomputed_weight_0, v2f(m_point_of_interest->position - m_p2.position)) / m_precomputed_denominator;
	m_weights.y = glm::dot(m_precomputed_weight_1, v2f(m_point_of_interest->position - m_p2.position)) / m_precomputed_denominator;
	m_weights.z = 1 - m_weights.x - m_weights.y;

	m_point_of_interest->position.w =
		m_p0.position.w * m_weights.x +
		m_p1.position.w * m_weights.y +
		m_p2.position.w * m_weights.z;
	m_point_of_interest->position.z = 1.f / m_point_of_interest->position.w;

	v3f ws_point_of_interest = m_point_of_interest->position * (1.f / m_point_of_interest->position.w);
	ws_point_of_interest.z = (1.f / m_point_of_interest->position.w);
	ws_point_of_interest = m_rotation_matrix * ws_point_of_interest;

	m_ws_weights.x = glm::dot(m_ws_precomputed_weight_0, v2f(ws_point_of_interest - m_ws_p2)) / m_ws_precomputed_denominator;
	m_ws_weights.y = glm::dot(m_ws_precomputed_weight_1, v2f(ws_point_of_interest - m_ws_p2)) / m_ws_precomputed_denominator;
	m_ws_weights.z = 1 - m_ws_weights.x - m_ws_weights.y;
}

bool Interpolator::isContained()
{
	if (m_weights.x < 0 || m_weights.y < 0 || m_weights.z < 0)
		return false;
	return true;
}

void Interpolator::interpolate()
{
	if (use_correct_interpolation)
	{
		m_point_of_interest->color =
			m_p0.color * m_ws_weights.x +
			m_p1.color * m_ws_weights.y +
			m_p2.color * m_ws_weights.z;
		m_point_of_interest->normal =
			m_p0.normal * m_ws_weights.x +
			m_p1.normal * m_ws_weights.y +
			m_p2.normal * m_ws_weights.z;
		m_point_of_interest->texture_coordinates =
			m_p0.texture_coordinates * m_ws_weights.x +
			m_p1.texture_coordinates * m_ws_weights.y +
			m_p2.texture_coordinates * m_ws_weights.z;
	}
	else
	{
		m_point_of_interest->color =
			m_p0.color * m_weights.x +
			m_p1.color * m_weights.y +
			m_p2.color * m_weights.z;
		m_point_of_interest->normal =
			m_p0.normal * m_weights.x +
			m_p1.normal * m_weights.y +
			m_p2.normal * m_weights.z;
		m_point_of_interest->texture_coordinates =
			m_p0.texture_coordinates * m_weights.x +
			m_p1.texture_coordinates * m_weights.y +
			m_p2.texture_coordinates * m_weights.z;
	}
}
