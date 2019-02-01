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
	, hello_("hello", "Hello", 0, 0, 5)
{
    // Add ports
    addPort(meshIn_);
    addPort(meshOut_);
    
    // Add properties
    addProperty(radius_);
	addProperty(hello_);
}


void CubeAnimator::process()
{
    // Clone the input mesh
    if (!meshIn_.getData()) return;
    auto mesh = meshIn_.getData()->clone();

    // Get the matrix that defines where the mesh is currently
    auto matrix = mesh->getWorldMatrix();
    // Transform the mesh (TODO)

	float pi = 3.14159265;
	float df = radius_.get();
	
	printf("%g\n", hello_.get());

	matrix = 
		//Circle Motion Around Tower & Swinging Inwards And Outwards Wave Motion
		glm::translate(vec3( (4 - sin(8 * df * pi + pi/4)) * cos(df * pi) , (4 - cos(8 * df * pi - pi/4)) * sin(df * pi), 1))
		//Swinging Sine Motion
		//Die Rotation Around Z-Axis
		* glm::rotate(glm::mat4(), df * pi, vec3(0, 0, 1)) 
		* matrix;
	
	// Update
    mesh->setWorldMatrix(matrix);
	
    // Set output
    meshOut_.setData(mesh);
}

} // namespace

