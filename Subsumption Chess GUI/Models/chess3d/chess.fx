float4x4 wMat;
float4x4 wvpMat;
float4x4 wInvMat;

//Light1 Point Light Source
float4 vecLight1Pos;
float4 vecLight1Amb;
float4 vecLight1Diff;
float4 vecLight1Spec;
//Light2 Directional Light Source
float4 vecLight2Dir;
float4 vecLight2Amb;
float4 vecLight2Diff;
float4 vecLight2Spec;
//Material properties
float4 matAmb;
float4 matDiff;
float4 matSpec;
float4 matEmiss;
float matPower;
//Additional variables
float4 vecEye;
sampler2D tex0;
sampler2D tex1;

struct VS_OUTPUT
{
    float4 PosWVP : POSITION;
	float3 toEyeT	:	TEXCOORD0;
    float2 TexCoordVS:   TEXCOORD1;
	float3 lightDirT1 : TEXCOORD2;
	float3 lightDirT2 : TEXCOORD3;
};

VS_OUTPUT vs_main(float4 Pos 		: POSITION0, 
				  float3 Tangent 	: TANGENT0,
				  float3 binormal 	: BINORMAL0,
				  float3 Normal 	: NORMAL0, 
				  float2 inTxr		: TEXCOORD0)
{
    VS_OUTPUT outVS = (VS_OUTPUT)0;
    
	float3x3 TBN;
	TBN[0] = Tangent;
	TBN[1] = binormal;
	TBN[2] = Normal;
	
	//Matrix transforms from object to tangent space.
	float3x3 toTangentSpace = transpose(TBN);
	
	//Transform eye position to localspace
	float3 eyePosL = mul(vecEye, wInvMat);
	
	float3 toEyeL = eyePosL - Pos;
	
	outVS.toEyeT = mul(toEyeL, toTangentSpace);
	
	//Light1 (point) vector calc's
	float3 PosW = normalize(mul(Pos, wMat));
	float3 LightDirW1 = normalize(vecLight1Pos - PosW);
	float3 LightDirL1 = mul(float4(LightDirW1,0.0f),wInvMat);
	//Light1 local vector calc
	float3 LightDirL2 = mul(vecLight2Dir,wInvMat);
	//Light to Tangent Space
	
	outVS.lightDirT1 = mul(LightDirL1, toTangentSpace);
	outVS.lightDirT2 = mul(LightDirL2, toTangentSpace);
    
	outVS.PosWVP = mul(Pos, wvpMat); // transform Position
    
    outVS.TexCoordVS = inTxr;

return outVS;
}

float4 ps_main(float3 toEyeT	:	TEXCOORD0,
			   float2 TexCoordVS: TEXCOORD1, 
			   float3 lightDirT1 : TEXCOORD2,
			   float3 lightDirT2 : TEXCOORD3) : COLOR
{
	
	//because sometimes normalisation is lost in interpolation
	toEyeT = normalize(toEyeT);
	lightDirT1 = normalize(lightDirT1);
	lightDirT2 = normalize(lightDirT2);
	
	float3 lightVecT1 = -lightDirT1;
	float3 lightVecT2 = -lightDirT2;
	
	float3 normalT = tex2D(tex1, TexCoordVS);
	
	normalT = 2.0f*normalT - 1.0f;
	
	normalT = normalize(normalT);

	//Reflective
	float3 r1 = reflect(-lightVecT1,normalT);
	float3 r2 = reflect(lightDirT2,normalT);
	//Spec Calc
	float t1 = pow(abs(max(dot(r1, toEyeT), 0.0f)), matPower);
	float t2 = pow(abs(max(dot(r2, toEyeT), 0.0f)), matPower);
	//Diff Calc
	float s1 = max(dot(lightVecT1, normalT), 0.0f);
	float s2 = max(dot(-lightVecT2, normalT), 0.0f);
	
	//Amb
	float3 ambient1 = matAmb * vecLight1Amb;
	float3 ambient2 = matAmb * vecLight2Amb;
	//Diff
	float3 diffuse1 = s1*(matDiff * vecLight1Diff).rgb;
	float3 diffuse2 = s2*(matDiff * vecLight2Diff).rgb;
	//Spec
	float3 specular1 = t1*(matSpec* vecLight1Spec).rgb;
	float3 specular2 = t2*(matSpec* vecLight2Spec).rgb;
	
	float3 light1ADSE = ambient1 + diffuse1 + specular1 + matEmiss;
	float3 light2ADSE = ambient2 + diffuse2 + specular2 + matEmiss;
	return (tex2D(tex0,TexCoordVS) * 
			float4(light1ADSE + light2ADSE, matDiff.a));
}


// Technique to implement a 3D transformation pipeline
technique effect
{
    pass P0    // Effect has only one pass, called 'P0'   
    {
          //set VertexShader state to vertex shader function
          VertexShader = compile vs_2_0 vs_main();
          //set PixelShader state to pixel shader function          
          PixelShader = compile ps_2_0 ps_main();
    }
}