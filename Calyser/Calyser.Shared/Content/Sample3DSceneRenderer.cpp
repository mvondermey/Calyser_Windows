#include "pch.h"
#include "Sample3DSceneRenderer.h"
#include "..\Common\DirectXHelper.h"

using namespace Calyser;
using namespace std;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(10),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
	//
	//done
	//
	
	//
}

// Initializes view parameters when the window size changes.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 60.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
		);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
		);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const XMVECTORF32 eye = { 0.0f, 0.7f, 1.5f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radiansX = static_cast<float>(fmod(totalRotation, XM_2PI));		
//
		Rotate(radiansX,0.0);
	}
}

// Rotate the 3D cube model a set amount of radians.
void Sample3DSceneRenderer::Rotate(float radiansX, float radiansY)
{
	// Prepare to pass the updated model matrix to the shader
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationZ(radiansX)*XMMatrixRotationX(radiansY)));
	//
}

void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
void Sample3DSceneRenderer::TrackingUpdate(float positionX, float positionY)
{
	if (m_tracking)
	{
		//
		// Reset Vertex
		//
		//for (int i = 0; i<cubeVerticesVector.size(); i++){
		//	cubeVerticesVector.at(i).color = XMFLOAT3(1.0f, 1.0f, 0.0f);
		//	{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) };
		//}
		//
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = &cubeVerticesVector[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(cubeVerticesVector.size()*sizeof(VertexPositionColor), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_vertexBuffer
			)
		);
		//
		//
		D3D11_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
		XMVECTOR cursor1 = XMVectorSet(positionX, positionY, viewport.MinDepth, 0.0);
		XMVECTOR cursor2 = XMVectorSet(positionX, positionY, viewport.MaxDepth, 0.0);
		//
		XMVECTOR minPointSource = XMVector3Unproject(cursor1, viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, XMLoadFloat4x4(&m_constantBufferData.projection), XMLoadFloat4x4(&m_constantBufferData.view), XMLoadFloat4x4(&m_constantBufferData.model));
		XMVECTOR maxPointSource = XMVector3Unproject(cursor2, viewport.TopLeftX, viewport.TopLeftY, viewport.Width, viewport.Height, viewport.MinDepth, viewport.MaxDepth, XMLoadFloat4x4(&m_constantBufferData.projection), XMLoadFloat4x4(&m_constantBufferData.view), XMLoadFloat4x4(&m_constantBufferData.model));
		//
		XMStoreFloat3(&(cubeVerticesVector.at(0).pos),minPointSource);
		XMStoreFloat3(&(cubeVerticesVector.at(1).pos),maxPointSource);
		OutputDebugStringA(to_string(cubeVerticesVector.at(0).pos.x).c_str());
		OutputDebugStringA(" = ");
		OutputDebugStringA(to_string(cubeVerticesVector.at(0).pos.y).c_str());
		OutputDebugStringA(" = ");
		OutputDebugStringA(to_string(cubeVerticesVector.at(0).pos.z).c_str());
		OutputDebugStringA(" \n ");
		//
		XMVECTOR RAY = XMVector3Normalize(maxPointSource - minPointSource);
		//
		//td::string output = "TEST\n" ;
		//OutputDebugStringA(output.c_str());
		//
		float radiansX = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		float radiansY = XM_2PI * 2.0f * positionY / m_deviceResources->GetOutputSize().Width;
		//
		//Rotate(radiansX, radiansY);
		//

	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
void Sample3DSceneRenderer::Render()
{
	//
	//OutputDebugStringA("Render...\n");
	//
	// Loading is asynchronous. Only draw geometry after it's loaded.
	if (!m_loadingComplete)
	{
		return;
	}
	//

	auto context = m_deviceResources->GetD3DDeviceContext();

	

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource(
		m_constantBuffer.Get(),
		0,
		NULL,
		&m_constantBufferData,
		0,
		0
		);

	// Each vertex is one instance of the VertexPositionColor struct.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_vertexBuffer.GetAddressOf(),
		&stride,
		&offset
		);

	context->IASetIndexBuffer(
		m_indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // Each index is one 16-bit unsigned integer (short).
		0
		);

	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	context->VSSetShader(
		m_vertexShader.Get(),
		nullptr,
		0
		);

	// Send the constant buffer to the graphics device.
	context->VSSetConstantBuffers(
		0,
		1,
		m_constantBuffer.GetAddressOf()
		);

	// Attach our pixel shader.
	context->PSSetShader(
		m_pixelShader.Get(),
		nullptr,
		0
		);

	// Draw the objects.
	context->DrawIndexed(
		m_indexCount,
		0,
		0
		);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateVertexShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_vertexShader
				)
			);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateInputLayout(
				vertexDesc,
				ARRAYSIZE(vertexDesc),
				&fileData[0],
				fileData.size(),
				&m_inputLayout
				)
			);
	});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreatePixelShader(
				&fileData[0],
				fileData.size(),
				nullptr,
				&m_pixelShader
				)
			);

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
				)
			);
	});

	// Once both shaders are loaded, create the mesh.
	auto createCubeTask = (createPSTask && createVSTask).then([this] () {

		// Load mesh vertices. Each vertex has a position and a color.
		static const VertexPositionColor cubeVertices[] = 
		{
			//Plane 1
			{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-0.2f, -0.5f, 0.2f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3( 0.2f, -0.5f, 0.2f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-0.1f, -0.5f, 0.2f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-0.1f, -0.5f, 0.2f), XMFLOAT3(1.0f, 1.0f, 1.0f) }


			/*
			{ XMFLOAT3(-0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f)},
			{ XMFLOAT3(0.5f, 0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) },
			{ XMFLOAT3(-0.5f, 0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) }
			*/
		};


		cubeVerticesVector.push_back({ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) });
		cubeVerticesVector.push_back({ XMFLOAT3( 0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) });
		cubeVerticesVector.push_back({ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f) });
		cubeVerticesVector.push_back({ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f) });

		int m_div = 4;
		cubeVerticesVector.clear();
		for (int i=0; i < m_div; i++) {
			for (int j=0; j < m_div; j++){
				float xmin = -0.5f;
				float xmax = 0.5f;
				float x = (xmax - xmin)*i / m_div + xmin;
				float y = (xmax - xmin)*j / m_div + xmin;
				cubeVerticesVector.push_back({ XMFLOAT3(x, -0.5f, y), XMFLOAT3(0.0f, 0.0f, .0f) });
				OutputDebugStringA(to_string(x).c_str());
				OutputDebugStringA(" = ");
				OutputDebugStringA(to_string(y).c_str());
				OutputDebugStringA("\n");

				//
			}
		}


		//std::copy(cubeVerticesVector.begin(),cubeVerticesVector.end(),cubeVertices);

		D3D11_SUBRESOURCE_DATA vertexBufferData = {0};
		vertexBufferData.pSysMem = &cubeVerticesVector[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(cubeVerticesVector.size()*sizeof(VertexPositionColor), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
				)
			);


		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes
		// 0, 2 and 1 from the vertex buffer compose the 
		// first triangle of this mesh.

		int numberVertices = (cubeVerticesVector.size());
		OutputDebugStringA("Number of Vertices \n");
		OutputDebugStringA(to_string(numberVertices).c_str());
		OutputDebugStringA("\n");

		static vector<short> cubeIndicesVector;
		cubeIndicesVector.push_back(0);
		cubeIndicesVector.push_back(1);
		for (int i = 0; i < numberVertices; i++){
			for (int j = 0; j < numberVertices; j++){
				if (i != j) {
					cubeIndicesVector.push_back(i);
					cubeIndicesVector.push_back(j);
				}
				//OutputDebugStringA(to_string(i).c_str());
				//OutputDebugStringA(" = ");
				//OutputDebugStringA(to_string(j).c_str());
				//OutputDebugStringA(" \n ");
//
			}
		}

		OutputDebugStringA("After Loop \n");

		//unsigned short cubeIndices[1000000];
		//std::copy(cubeVerticesVector.begin(), cubeVerticesVector.end(); cubeIndices);
		/* short cubeIndices[] = {
			0, 1,
			1,2,
			2,3,
			3,0,
			0,2,
			//
			0,4,
			1,4,
			2,4,
			3,4
		};
		//
		*/
		//short mytest []= cubeIndicesVector;
		//
		//std::copy(cubeIndicesVector.begin(), cubeIndicesVector.end(), cubeIndices);
		//
		//m_indexCount = ARRAYSIZE(cubeIndices);
		m_indexCount = cubeIndicesVector.size();
		/*

		*/
		//
		D3D11_SUBRESOURCE_DATA indexBufferData = {0};
		indexBufferData.pSysMem = &cubeIndicesVector[0];
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(cubeIndicesVector.size()*sizeof(short), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
				)
			);
	});
	//
	// Once the cube is loaded, the object is ready to be rendered.
	createCubeTask.then([this] () {
		m_loadingComplete = true;
	});
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}