/*********************************************************************
 *  Author  : Tino Weinkauf
 *  Init    : Thursday, February 01, 2018 - 20:35:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <modules/dh2320lab2/chaikin.h>

namespace inviwo
{
namespace kth
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo Chaikin::processorInfo_
{
    "org.inviwo.Chaikin",      // Class identifier
    "Chaikin",                // Display name
    "DH2320",              // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo Chaikin::getProcessorInfo() const
{
    return processorInfo_;
}


Chaikin::Chaikin()
    :Processor()
    ,portInLines("InLines")
    ,portOutLines("OutLines")
    ,propMinNumDesiredPoints("MinNumDesiredPoints", "Num Points", 100, 1, 200, 1)
{
    addPort(portInLines);
    addPort(portOutLines);
    addProperty(propMinNumDesiredPoints);
}

/*  Applies Chaikin's Corner Cutting algorithm.

    ControlPolygon is an array of points defining the line segments to be subdivided.
    MinNumDesiredPoints is the minimum number of points the resulting curve should contain.
    Curve is the array of points of the resulting curve after applying corner cutting.
*/
void Chaikin::CornerCutting(const std::vector<vec3>& ControlPolygon,
                            const size_t MinNumDesiredPoints,
                            std::vector<vec3>& Curve)
{
    //TODO: Extend and edit this code
	for (int z(0); z < ControlPolygon.size(); z++) {
		//std::cout << newCurve[k];
		std::cout << 'h' << ControlPolygon[z] << '\n';
	}
    Curve.clear();
	std::vector<vec3> newCurve;
    const size_t NumPointsPerPolygonLeg = 1 + MinNumDesiredPoints / ControlPolygon.size();
    Curve.reserve(NumPointsPerPolygonLeg * ControlPolygon.size());
	newCurve.reserve(NumPointsPerPolygonLeg * ControlPolygon.size());
	//Curve.clear();
	//Curve.push_back(ControlPolygon[0]);
	for (int i(0); i < ControlPolygon.size(); i++)
	{	
		//if (!(floor(100*((ControlPolygon[i + 1][1] - ControlPolygon[i - 1][1]) / (ControlPolygon[i + 1][0] - ControlPolygon[i - 1][0])))) == floor(100 * ((ControlPolygon[i + 1][1] - ControlPolygon[i - 1][1]) / (ControlPolygon[i + 1][0] - ControlPolygon[i - 1][0]))))
		//{
			/*(!((floor(100*((ControlPolygon[i + 1][1] * ControlPolygon[i - 1][1]) / (ControlPolygon[i + 1][0] * ControlPolygon[i - 1][0])))/100) ==
			(floor(100*((ControlPolygon[i + 1][1] * ControlPolygon[i][1]) / (ControlPolygon[i + 1][0] * ControlPolygon[i][0])))/100))) {
			Curve.push_back(ControlPolygon[i]);*/
			Curve.push_back(ControlPolygon[i]);
		//}
		//else {
		//	std::cout << i << " hej";
		//}
		//std::cout << "long" << ((ControlPolygon[i + 1][1]  - ControlPolygon[i - 1][1]) / (ControlPolygon[i + 1][0] - ControlPolygon[i - 1][0])) << '\n';
		//std::cout << "short" << ((ControlPolygon[i + 1][1] - ControlPolygon[i][1]) / (ControlPolygon[i + 1][0] - ControlPolygon[i][0])) << '\n';
	}
    for(size_t i(0);i<MinNumDesiredPoints -1 ;i++)
    {
		newCurve.clear();


        //Linearly interpolate between left and right point in the t-interval [0,1)
		const vec3& LeftPoint = Curve[0];
		const vec3& RightPoint = Curve[(0 + 1) % Curve.size()];
		const float t = 0.25;
		newCurve.push_back((1 - t) * LeftPoint + t * RightPoint);
		newCurve.push_back(t*LeftPoint + (1 - t) * RightPoint);
		for (size_t j(1); j < Curve.size(); j++)
		{
			const vec3& LeftPoint = Curve[j];
			const vec3& RightPoint = Curve[(j + 1) % Curve.size()];
			if (!(floor(100*(Curve[(j + 1) % Curve.size()][1] - Curve[j][1]) * (Curve[j][0] - Curve[j - 1][0])) == floor(100 * (Curve[j][1] - Curve[j - 1][1]) * (Curve[(j + 1) % Curve.size()][0] - Curve[j][0])))) {

				const float t = 0.25;
				newCurve.push_back((1 - t) * LeftPoint + t * RightPoint);
				newCurve.push_back(t*LeftPoint + (1 - t) * RightPoint);

			}
			else {
				newCurve.push_back(RightPoint);
			}
        }
		Curve.clear();
		Curve = newCurve;
    }
}

void Chaikin::process()
{
    //Get the input data
    auto MultiInLines = portInLines.getVectorData();

    //Prepare output data
    auto OutLines = std::make_shared<Mesh>(DrawType::Lines, ConnectivityType::Strip);
    auto OutVertexBuffer = std::make_shared<Buffer<vec3> >();
    auto OutVertices = OutVertexBuffer->getEditableRAMRepresentation();
    OutLines->addBuffer(BufferType::PositionAttrib, OutVertexBuffer);

    for(auto InLines : MultiInLines)
    {
        //Vertex data
        auto pit = util::find_if(InLines->getBuffers(), [](const auto& buf)
        {
            return buf.first.type == BufferType::PositionAttrib;
        });
        if (pit == InLines->getBuffers().end()) return; //could not find a position buffer
        // - in RAM
        const auto posRam = pit->second->getRepresentation<BufferRAM>();
        if (!posRam) return; //could not find a position buffer ram
        // - 3D
        if (posRam->getDataFormat()->getComponents() != 3) return; //Only 3 dimensional meshes are supported
        // - save into a reasonable format with transformed vertices
        std::vector<glm::vec3> AllVertices;
        Matrix<4, float> Trafo = InLines->getWorldMatrix();
        const size_t NumInVertices = posRam->getSize();
        AllVertices.reserve(NumInVertices);
        for(size_t i(0);i<NumInVertices;i++)
        {
            dvec3 Position = posRam->getAsDVec3(i);
            glm::vec4 HomogeneousPos(Position.x, Position.y, Position.z, 1.0f);
            glm::vec4 TransformedHomogeneousPos = Trafo * HomogeneousPos;

            AllVertices.push_back( vec3(TransformedHomogeneousPos.x / TransformedHomogeneousPos.w,
                                        TransformedHomogeneousPos.y / TransformedHomogeneousPos.w,
                                        TransformedHomogeneousPos.z / TransformedHomogeneousPos.w
                                        ) );
        }

        //For each line buffer
        const auto& AllIndexBuffers = InLines->getIndexBuffers();
        for(const auto& IdxBuffer : AllIndexBuffers)
        {
            //Well, do we actually have lines? If not, next buffer!
            if (IdxBuffer.first.dt != DrawType::Lines) continue;

            //Get the indices of the lines
            const auto& Indices = IdxBuffer.second->getRAMRepresentation()->getDataContainer();

            //Create a simple vector of line vertices for the corner cutting
            std::vector<glm::vec3> LineVertices;
            LineVertices.reserve(Indices.size());
            for(const auto& idx : Indices)
            {
                if (LineVertices.empty() || LineVertices.back() != AllVertices[idx])
                {
                    LineVertices.push_back(AllVertices[idx]);
                }
            }

            //Remove a possibly duplicated first/last point (closed loop case),
            // since we assume a closed loop anyway.
            if (LineVertices.size() > 1 && LineVertices[0] == LineVertices[LineVertices.size() - 1])
            {
                LineVertices.pop_back();
            }

            //Cut the corners!
            std::vector<vec3> ChaikinVertices;
            CornerCutting(LineVertices, propMinNumDesiredPoints.get(), ChaikinVertices);
            const size_t NumNewVertices = ChaikinVertices.size();

            //Write out
            auto OutIndexBuffer = std::make_shared<IndexBuffer>();
            auto OutIndices = OutIndexBuffer->getEditableRAMRepresentation();
            auto OutIndexBufferPoints = std::make_shared<IndexBuffer>();
            auto OutIndicesPoints = OutIndexBufferPoints->getEditableRAMRepresentation();
            OutLines->addIndicies(Mesh::MeshInfo(DrawType::Lines, ConnectivityType::Strip), OutIndexBuffer);
            OutLines->addIndicies(Mesh::MeshInfo(DrawType::Points, ConnectivityType::None), OutIndexBufferPoints);
            const size_t PreviousNumVertices = OutVertices->getSize();
            OutVertices->reserve(NumNewVertices + PreviousNumVertices);
            for(size_t i(0);i<NumNewVertices;i++)
            {
                OutVertices->add(ChaikinVertices[i]);
                OutIndices->add((uint32_t)(PreviousNumVertices + i));
                OutIndicesPoints->add((uint32_t)(PreviousNumVertices + i));
            }
            OutIndices->add((uint32_t)(PreviousNumVertices)); //Close loop.
        }
    }

    //Push it out!
    portOutLines.setData(OutLines);
}

} // namespace
} // namespace

