#include "DXUT.h" 
#include "Piece.h"
#include "CustomModel.h"


Piece::Piece(IDirect3DDevice9* pd3dDevice) : CustomModel(pd3dDevice)
{
	m_pCModelLocation = "Models\\chess3d\\";
}

// Piece::Piece(const Piece & rhs)
// {
// 
// }

Piece::~Piece()

{

}

void Piece::Effect_Select_Reset()
{
	m_v4_HighLightColour = D3DXReset;
}

void Piece::Effect_Select_Highlight()
{
	m_v4_HighLightColour = D3DXVECTOR4(0.50f, 0.50f, 1.0f, 1.0f);
}

void Piece::Effect_Select_Selected()
{
	m_v4_HighLightColour = D3DXVECTOR4(0.50f, 1.00f, 0.50f, 1.0f);
}

void Piece::Effect_Select_SelectedMoves()
{
	m_v4_HighLightColour = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.5f);
}

void Piece::render(D3DXVECTOR3 v3Position)
{
	D3DXMATRIX world;
	D3DXMATRIX worldViewProjection;
	D3DXMATRIX worldinverse;
	D3DXMATRIX translation;
	
	D3DXMatrixTranslation(&translation, v3Position.x, v3Position.y, v3Position.z);
	D3DMATRIX RotationA = m_mRotationX * m_mRotationY * m_mRotationZ;
	world = m_mScale * RotationA * translation;
	worldViewProjection = world * m_mViewProjection;
	D3DXMatrixInverse(&worldinverse, 0, &world);

	unsigned int numOfPasses = 1;
	unsigned int passIndex;

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	if( m_pEffect != NULL )
	{
		//Technique binds
		D3DXHANDLE hTechnique = m_pEffect->GetTechniqueByName( "effect" );
		m_pEffect->SetTechnique( hTechnique );

		m_pEffect->ApplyParameterBlock( m_hParameters );

		m_pEffect->SetMatrix( "wMat", &world);
		m_pEffect->SetMatrix( "wvpMat", &worldViewProjection);
		m_pEffect->SetMatrix( "wInvMat", &worldinverse);

		//Begin Effect/Shader
		///////////////////////////////////
		m_pEffect->Begin( &numOfPasses, 0 );	// start an active technique
		// loop through all render passes in technique
		for(passIndex = 0; passIndex < numOfPasses; ++passIndex)
		{
			m_pEffect->BeginPass(passIndex);	// begin a pass, within active technique
			for (DWORD i=0; i<m_dNumMaterials; i++)
			{
				// Set the material and texture for this subset
				m_pD3dDevice->SetMaterial(&m_pMeshMaterials[i]);
				m_pD3dDevice->SetTexture(0, m_pMeshTextures[i]);
				m_pD3dDevice->SetTexture(1, m_pNormalmapTex);

				D3DXVECTOR4 matDiff4( m_pMeshMaterials[i].Diffuse.r, m_pMeshMaterials[i].Diffuse.g, m_pMeshMaterials[i].Diffuse.b, m_pMeshMaterials[i].Diffuse.a );
				m_pEffect->SetVector("matDiff",&matDiff4);
				D3DXVECTOR4 matAmb4( m_pMeshMaterials[i].Ambient.r, m_pMeshMaterials[i].Ambient.g, m_pMeshMaterials[i].Ambient.b, m_pMeshMaterials[i].Ambient.a );
				m_pEffect->SetVector("matAmb",&matAmb4);
				D3DXVECTOR4 matSpec4( m_pMeshMaterials[i].Specular.r, m_pMeshMaterials[i].Specular.g, m_pMeshMaterials[i].Specular.b, m_pMeshMaterials[i].Specular.a );
				m_pEffect->SetVector("matSpec",&matSpec4);
				D3DXVECTOR4 matEmiss4( m_pMeshMaterials[i].Emissive.r, m_pMeshMaterials[i].Emissive.g, m_pMeshMaterials[i].Emissive.b, m_pMeshMaterials[i].Emissive.a );
				m_pEffect->SetVector("matEmiss",&matEmiss4);
				float matPower = m_pMeshMaterials[i].Power;
				m_pEffect->SetFloat("matPower",matPower);
				m_pEffect->CommitChanges();    
				// Draw the mesh subset
				m_pMesh->DrawSubset( i );
			}
			m_pEffect->EndPass(); // signal end of rendering current pass 
		}
		m_pEffect->End();  // signal end of rendering current technique
		m_pEffect->DeleteParameterBlock( m_hParameters );
	}

}