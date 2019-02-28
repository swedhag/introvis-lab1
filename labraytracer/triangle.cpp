/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/triangle.h>
#include <labraytracer/util.h>
#include <memory>

namespace inviwo {

Triangle::Triangle() {
}

Triangle::Triangle(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& uvw0,
                   const vec3& uvw1, const vec3& uvw2) {
    mVertices[0] = v0;
    mVertices[1] = v1;
    mVertices[2] = v2;
    mUVW[0] = uvw0;
    mUVW[1] = uvw1;
    mUVW[2] = uvw2;
}

bool Triangle::closestIntersection(const Ray& ray, double maxLambda,
                                   RayIntersection& intersection) const {
    //Programming TASK 1: Implement this method
    //Your code should compute the intersection between a ray and a triangle.
    //
    //If you detect an intersection, the return type should look similar to this:
    //if(rayIntersectsTriangle)
    //{
    //  intersection = RayIntersection(ray,shared_from_this(),lambda,ray.normal,uvw);
    //  return true;
    //} 
    //
    // Hints :
    // Ray origin p_r : ray.getOrigin()
    // Ray direction t_r : ray.getDirection()
    // Compute the intersection point using ray.pointOnRay(lambda)


	const vec3& t1 = (mVertices[1] - mVertices[0]);
	const vec3& t2 = (mVertices[2] - mVertices[0]);
	const vec3& normal = glm::cross(t1, t2);
	const vec3& pr = ray.getOrigin();
	const vec3& tr = ray.getDirection();
	const double denominator = glm::dot(tr, normal);
	if (fabs(denominator) == 0) {
		return false;
	}
	const double numerator = glm::dot((mVertices[0] - pr), normal);
	const double lambda = numerator / denominator;
	if (lambda > maxLambda || lambda < 0) {
		return false;
	}

	const vec3& rayPoint = pr + Util::scalarMult(lambda, tr);
	const vec3& tp = rayPoint - mVertices[0];
	const double dot00 = glm::dot(t2, t2);
	const double dot01 = glm::dot(t2, t1);
	const double dot02 = glm::dot(t2, tp);
	const double dot11 = glm::dot(t1, t1);
	const double dot12 = glm::dot(t1, tp);
	const double invertedDenominator = 1 / (dot00 * dot11 - dot01 * dot01);
	const double s = (dot11*dot02 - dot01*dot12) * invertedDenominator;
	const double t = (dot00*dot12 - dot01*dot02) * invertedDenominator;
	if (!((s >= 0) && (t >= 0) && (s + t <= 1))) {
		return false;
	}
	const vec3  uvw(0, 0, 0);
	intersection = RayIntersection(ray, shared_from_this(), lambda, normal, uvw);
	return true;
}

bool Triangle::anyIntersection(const Ray& ray, double maxLambda) const {
    RayIntersection temp;
    return closestIntersection(ray, maxLambda, temp);
}

void Triangle::drawGeometry(std::shared_ptr<BasicMesh> mesh,
                            std::vector<BasicMesh::Vertex>& vertices) const {
    auto indexBuffer = mesh->addIndexBuffer(DrawType::Lines, ConnectivityType::None);

    Util::drawLineSegment(mVertices[0], mVertices[1], vec4(0.2, 0.2, 0.2, 1), indexBuffer,
                          vertices);
    Util::drawLineSegment(mVertices[1], mVertices[2], vec4(0.2, 0.2, 0.2, 1), indexBuffer,
                          vertices);
    Util::drawLineSegment(mVertices[2], mVertices[0], vec4(0.2, 0.2, 0.2, 1), indexBuffer,
                          vertices);
}

} // namespace
