﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include "CMesh_D3D11.hpp"
# include <Siv3D/Error.hpp>
# include <Siv3D/Common/Siv3DEngine.hpp>

namespace s3d
{
	CMesh_D3D11::CMesh_D3D11()
	{
		// do nothing
	}

	CMesh_D3D11::~CMesh_D3D11()
	{
		LOG_SCOPED_TRACE(U"CMesh_D3D11::~CMesh_D3D11()");

		m_meshes.destroy();
	}

	void CMesh_D3D11::init()
	{
		LOG_SCOPED_TRACE(U"CMesh_D3D11::init()");

		pRenderer	= dynamic_cast<CRenderer_D3D11*>(SIV3D_ENGINE(Renderer));
		m_device	= pRenderer->getDevice();
		m_context	= pRenderer->getContext();

		// null Mesh を管理に登録
		{
			const Array<Vertex3D> vertices =
			{
				Vertex3D{ .pos = { -8.0f, 0.0f,  8.0f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex = { 0.0f, 0.0f } },
				Vertex3D{ .pos = {  8.0f, 0.0f,  8.0f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex = { 1.0f, 0.0f } },
				Vertex3D{ .pos = { -8.0f, 0.0f, -8.0f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex = { 0.0f, 1.0f } },
				Vertex3D{ .pos = {  8.0f, 0.0f, -8.0f }, .normal = { 0.0f, 1.0f, 0.0f }, .tex = { 1.0f, 1.0f } },
			};
			const Array<Vertex3D::IndexType> indices =
			{
				0, 1, 2, 2, 1, 3
			};

			// null Mesh を作成
			auto nullMesh = std::make_unique<D3D11Mesh>(m_device, vertices, indices);

			if (not nullMesh->isInitialized()) // もし作成に失敗していたら
			{
				throw EngineError{ U"Null Mesh initialization failed" };
			}

			// 管理に登録
			m_meshes.setNullData(std::move(nullMesh));
		}
	}

	Mesh::IDType CMesh_D3D11::create(const Array<Vertex3D>& vertices, const Array<Vertex3D::IndexType>& indices)
	{
		if ((not vertices) || (not indices))
		{
			return Mesh::IDType::NullAsset();
		}

		auto mesh = std::make_unique<D3D11Mesh>(m_device, vertices, indices);

		if (not mesh->isInitialized())
		{
			return Mesh::IDType::NullAsset();
		}

		const String info = U"(type: Default, vertex count:{0}, index count: {1})"_fmt(vertices.size(), indices.size());
		return m_meshes.add(std::move(mesh), info);
	}

	void CMesh_D3D11::release(const Mesh::IDType handleID)
	{
		m_meshes.erase(handleID);
	}

	size_t CMesh_D3D11::getIndexCount(const Mesh::IDType handleID)
	{
		return m_meshes[handleID]->getIndexCount();
	}

	void CMesh_D3D11::bindMeshToContext(const Mesh::IDType handleID)
	{
		m_meshes[handleID]->bindToContext(m_context);
	}
}
