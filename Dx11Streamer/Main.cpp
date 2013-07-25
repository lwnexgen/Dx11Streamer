#include <iostream>
#include <dxgi1_2.h>
#include <vector>

using namespace std;

int main()
{
  //Initialize the factory pointer
  IDXGIFactory2* pFactory;
  
  //Actually create it
  HRESULT hCreateFactory = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&pFactory));
  if (hCreateFactory != S_OK) {
    printf("ERROR: 0x%X\n", hCreateFactory);

    return 1;
  }

  IDXGIAdapter1* pAdapter;
  std::vector <IDXGIAdapter2*> vAdapters;

  UINT adapterIndex = 0;
  while (pFactory -> EnumAdapters1(adapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
    IDXGIAdapter2* pAdapter2 = (IDXGIAdapter2*) pAdapter;

    vAdapters.push_back(pAdapter2);

    printf("enumerated %p\n", pAdapter2);

    ++adapterIndex;
  }

  return 0;
}
