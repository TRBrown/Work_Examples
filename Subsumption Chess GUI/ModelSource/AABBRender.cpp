#include "DXUT.h" 
#include "AABBRender.h"

extern IDirect3DDevice9* g_pD3DDevice;

AABBRender::AABBRender(AABB boundingbox)
{
	m_pD3dDevice = g_pD3DDevice;
	Loadmodel(boundingbox);

}

AABBRender::~AABBRender()
{
	
	m_pVertexBuffer->Release();
	m_pVertexBuffer = 0;

}


void AABBRender::Loadmodel(AABB boundingbox)
{
	
	int verticalEdgeCount = (4 + 1);
	int vertexCount = verticalEdgeCount * 2;


    // Create vertex buffer.
    m_pD3dDevice->CreateVertexBuffer( vertexCount * sizeof(CUSTOMVERTEX),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL );

    /*
	 Load data into vertex buffer
	 */
	
    CUSTOMVERTEX* pVertexArray;
    
	m_pVertexBuffer->Lock( 0, 0, (void**)&pVertexArray, 0 );

	// Generate array of vertices (and vertex normals) for box (open top and bottom) as 
	// triangle strip (see render() function, where type of draw primitive (expected by 
	// rendering device) set as "D3DPT_TRIANGLESTRIP").

/* Vertices of box (open top and bottom) arranged as triangle strip

(-1,1,1)   (1,1,1)   (1,1,-1)  (-1,1,-1) (-1,1,1)
	*---------*---------*---------*---------*
	|\        |\        |\        |\        |
	|  \      |  \      |  \      |  \      |
	|    \    |    \    |    \    |    \    |
	|      \  |      \  |      \  |      \  |
	|        \|        \|        \|        \|
	*---------*---------*---------*---------*
(-1,-1,1)  (1,-1,1)  (1,-1,-1) (-1,-1,-1) (-1,-1, 1)

(AABBmin) (min,max,min)   (1,1,-1)  (-1,1,-1) (AABBmin)
*---------*---------*---------*---------*
|\        |\        |\        |\        |
|  \      |  \      |  \      |  \      |
|    \    |    \    |    \    |    \    |
|      \  |      \  |      \  |      \  |
|        \|        \|        \|        \|
*---------*---------*---------*---------*
(min,min,max)  (1,-1,1)  (AABBmax) (-1,-1,-1) (min,min,max)

*/
	pVertexArray[0].position = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.minPt.y,  boundingbox.maxPt.z);
	pVertexArray[1].position = boundingbox.minPt;
	pVertexArray[2].position = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.maxPt.y,  boundingbox.maxPt.z);
	pVertexArray[3].position = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.maxPt.y,  boundingbox.minPt.z);
	pVertexArray[4].position = boundingbox.maxPt;
	pVertexArray[5].position = D3DXVECTOR3(boundingbox.maxPt.x, boundingbox.maxPt.y,  boundingbox.minPt.z);
	pVertexArray[6].position = D3DXVECTOR3(boundingbox.maxPt.x, boundingbox.minPt.y,  boundingbox.maxPt.z);
	pVertexArray[7].position = D3DXVECTOR3(boundingbox.maxPt.x, boundingbox.minPt.y,  boundingbox.minPt.z);
	// for closed box, last edge of triangle strip is same as first edge 
	pVertexArray[8].position = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.minPt.y,  boundingbox.maxPt.z);
	pVertexArray[9].position = boundingbox.minPt;
// 	pVertexArray[0].normal = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.minPt.y,  boundingbox.maxPt.z);
// 	pVertexArray[1].normal = boundingbox.minPt;
// 	pVertexArray[2].normal = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.maxPt.y,  boundingbox.maxPt.z);
// 	pVertexArray[3].normal = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.maxPt.y,  boundingbox.minPt.z);
// 	pVertexArray[4].normal = boundingbox.maxPt;
// 	pVertexArray[5].normal = D3DXVECTOR3(boundingbox.maxPt.x, boundingbox.maxPt.y,  boundingbox.minPt.z);
// 	pVertexArray[6].normal = D3DXVECTOR3(boundingbox.maxPt.x, boundingbox.minPt.y,  boundingbox.maxPt.z);
// 	pVertexArray[7].normal = D3DXVECTOR3(boundingbox.maxPt.x, boundingbox.minPt.y,  boundingbox.minPt.z);
// 	// for closed box, last edge of triangle strip is same as first edge 
// 	pVertexArray[8].normal = D3DXVECTOR3(boundingbox.minPt.x, boundingbox.minPt.y,  boundingbox.maxPt.z);
//	pVertexArray[9].normal = boundingbox.minPt;
	pVertexArray[0].colour = D3DCOLOR_XRGB(0, 0, 255);
	pVertexArray[1].colour = D3DCOLOR_XRGB(255, 0, 255);
	pVertexArray[2].colour = D3DCOLOR_XRGB(255, 255, 255);
	pVertexArray[3].colour = D3DCOLOR_XRGB(255, 255, 0);
	pVertexArray[4].colour = D3DCOLOR_XRGB(255, 255, 255);
	pVertexArray[5].colour = D3DCOLOR_XRGB(255, 255, 255);
	pVertexArray[6].colour = D3DCOLOR_XRGB(0, 255, 255);
	pVertexArray[7].colour = D3DCOLOR_XRGB(255, 0, 255);
	pVertexArray[8].colour = D3DCOLOR_XRGB(255, 255, 255);
	pVertexArray[9].colour = D3DCOLOR_XRGB(0, 255, 255);

    m_pVertexBuffer->Unlock();


};

void AABBRender::render(D3DXMATRIX view, D3DXMATRIX projection)
{

	D3DXMATRIX IdentityMat;

	D3DXMatrixIdentity(&IdentityMat);

	m_pD3dDevice->SetTransform(D3DTS_WORLD, &IdentityMat);

	m_pD3dDevice->SetTransform(D3DTS_VIEW, &view);    // set the view transform to matView

	m_pD3dDevice->SetTransform(D3DTS_PROJECTION, &projection);    // set the projection

	
	int triangleCount = (2 * 4);
	
	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );

	// Render contents of vertex buffer into backbuffer 
	m_pD3dDevice->SetStreamSource( 0, m_pVertexBuffer, 0, sizeof(CUSTOMVERTEX) );
	m_pD3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
	m_pD3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, triangleCount);

	m_pD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pD3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
}
