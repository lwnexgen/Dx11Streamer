#include <iostream>
#include <dxgi1_2.h>
#include <vector>
#include <windows.h>
#include <d3d11.h>

const char * NAME = "Dx11Streamer";

const int MAJOR = 0;
const int MINOR = 0;
const int RELEASE = 1;

IDXGIOutput1 * findAttachedOutput(IDXGIFactory1 * pFactory);
ID3D11Device * createDirect3D11Device(IDXGIOutput1 * pAttachedOutputAdapter);
