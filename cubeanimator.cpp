/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Thursday, October 12, 2017 - 11:11:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */
//#include <iostream>
//#include <string>
#include <labtransformations/cubeanimator.h>
#include <stdio.h>      
#include <math.h>   

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo CubeAnimator::processorInfo_
{
    "org.inviwo.CubeAnimator",      // Class identifier
    "Cube Animator",                // Display name
    "VisGra",                 // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo CubeAnimator::getProcessorInfo() const
{
    return processorInfo_;
}


CubeAnimator::CubeAnimator()
    :Processor()
    // Ports
    , meshIn_("meshIn")
    , meshOut_("meshOut")
    // Properties 
    // For a FloatProperty 
    // variablename(identifier, display name, init value, minvalue, maxvalue)
    , radius_("radius", "Radius", 1, 0, 2)
{
    // Add ports
    addPort(meshIn_);
    addPort(meshOut_);
    
    // Add properties
    addProperty(radius_);

}


void CubeAnimator::process()
{
    // Clone the input mesh
    if (!meshIn_.getData()) return;
    auto mesh = meshIn_.getData()->clone();

    // Get the matrix that defines where the mesh is currently
    auto matrix = mesh->getWorldMatrix();
    // Transform the mesh (TODO)
    //matrix *= glm::translate(vec3(radius_.get(), 0,0));
	float pi = 3.14159265;
	float df = radius_.get();
	//float df3 = df * 3;
	//printf("%g\n",sin(df*6.28));

	//matrix *= glm::translate(vec3((sin((radius_.get()) * 2)-1), (cos((radius_.get()) * 2) - 1), 1));
	//matrix *= glm::translate(vec3((4*sin(df*2*pi))-((sin(16*pi*df))/5), (4*cos(df*2*pi))- ((cos(16*pi*df)/5) / 1), 1)); //(sin(df * 8 * pi) / 2)
	matrix = glm::translate(vec3( 4 * cos(df * pi) - sin(9 * df * pi - pi/2)/2, 4 * sin(df * pi) - cos(9 * df * pi + pi/2)/2, 1)) * glm::rotate(glm::mat4(), df * pi, vec3(0, 0, 1)) * matrix;
	//matrix = glm::translate(vec3(sin(df * 16 * pi), cos(df * 16 * pi), 0)) * matrix;
	//matrix = glm::rotate(glm::mat4(), df * 2 * pi, vec3(0, 0, 1)) * matrix;
	// Update
    mesh->setWorldMatrix(matrix);
	
    // Set output
    meshOut_.setData(mesh);
}

} // namespace

