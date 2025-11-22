//シェーダー

//頂点シェーダーの入力構造体・・これをもとに基礎情報を作成
struct VSInput
{
    float3 position : POSITION;
    float4 coler : COLER;
};

//頂点シェーダーの出力構造体・・シェーダーとして返してるはず
struct VSOutput
{
    float4 position : SV_Position;
    float4 coler : COLER;
};

//ピクセルシェーダーの入力構造体
struct PSInput
{
    float4 position : SV_Position;
    float4 coler : COLOR;
};

//頂点シェーダーを返す
VSOutput vs(VSInput input)
{
    VSOutput output;
    
    output.position = float4(input.position, 1.0f);
    
    output.coler = input.coler;
    
    return output;
}

//ピクセルシェーダー
float4 ps(PSInput input) : SV_TARGET
{
    return input.coler;
}