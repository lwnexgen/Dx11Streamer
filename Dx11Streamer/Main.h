#include <iostream>
#include <dxgi1_2.h>
#include <vector>
#include <windows.h>

const char * NAME = "Dx11Streamer";

const int MAJOR = 0;
const int MINOR = 0;
const int RELEASE = 1;

IDXGIOutput1 * findAttachedOutput(IDXGIFactory1 * pFactory);
