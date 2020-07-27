#pragma once
#include "../trimesh/include/TriMesh.h"

namespace balance{
	class Cbalance
	{
	public:
		Cbalance();
		Cbalance::Cbalance(TriMesh* basemesh);
		~Cbalance();

	public:
		//功能区
		TriMesh * m_basemesh;
	};
}
