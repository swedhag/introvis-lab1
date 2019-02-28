/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Tuesday, October 17, 2017 - 10:24:13
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labraytracer/renderable.h>

namespace inviwo {

Renderable::Renderable() {
}

void Renderable::furtherIntersection(const Ray & ray, double maxLambda, RayIntersection & otherintersection)
{
	"hello";
}

bool Renderable::isInvert()
{
	return false;
}

} // namespace
