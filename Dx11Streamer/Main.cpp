#include "Main.h"

using namespace std;

int main()
{
  printf("%s %i.%i.%i\n", NAME, MAJOR, MINOR, RELEASE);

  //Get session information
  DWORD pSessionId;
  if (ProcessIdToSessionId(GetCurrentProcessId(), &pSessionId)) {
    if (pSessionId == 0) {
      printf("sorry, %s needs interactive priviliges\n", NAME);
      return 1;
    }
  }

  //Initialize the factory pointer
  IDXGIFactory2* pFactory;
  
  //Actually create it
  HRESULT hCreateFactory = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&pFactory));
  if (hCreateFactory != S_OK) {
    printf("ERROR: 0x%X\n", hCreateFactory);

    return 1;
  }

  try {
    IDXGIOutput1 * pAttachedOutput = findAttachedOutput(pFactory);

    void * pOutputParent;
    if (pAttachedOutput->GetParent(__uuidof(IDXGIAdapter1), &pOutputParent) != S_OK) {
      throw 3;
    }

    DXGI_OUTPUT_DESC oDesc;
    pAttachedOutput->GetDesc(&oDesc);

    IDXGIAdapter1 * pAttachedOutputAdapter = (IDXGIAdapter1 *)pOutputParent;

    DXGI_ADAPTER_DESC aDesc;
    pAttachedOutputAdapter->GetDesc(&aDesc);

    printf("duplicating %S attached to %S\n", oDesc.DeviceName, aDesc.Description);

    return 0;
  } catch (int e) {
    if (e == 1) {
      printf("error enumerating outputs\n");
      return 1;
    }

    if (e == 2) {
      printf("couldn't find an attached desktop\n");
      return 1;
    }

    if (e == 3) {
      printf("couldn't get the parent of the attached adapter\n");
      return 1;
    }

    if (e == 4) {
      printf("couldn't initialize the Direct3D device\n");
      return 1
    }
  }
}

ID3D11Device * createDirect3D11Device(IDXGIAdapter1 * pOutputAdapter) {
  // Feature levels supported
  D3D_FEATURE_LEVEL aFeatureLevels[] =
    {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_1
    };

  return 0;

  // ID3D11Device * pD3DDevice;
  // if (D3D11CreateDevice(pOutputAdapter, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, aFeatureLevels, 5, ) != S_OK) {
  //   throw 4;
  // }
}

IDXGIOutput1 * findAttachedOutput(IDXGIFactory1 * pFactory) {
  IDXGIAdapter1* pAdapter;

  UINT adapterIndex = 0;
  while (pFactory -> EnumAdapters1(adapterIndex, &pAdapter) != DXGI_ERROR_NOT_FOUND) {
    IDXGIAdapter2* pAdapter2 = (IDXGIAdapter2*) pAdapter;
        
    UINT outputIndex = 0;
    IDXGIOutput * pOutput;

    HRESULT hEnumOutputs = pAdapter -> EnumOutputs(outputIndex, &pOutput);
    while(hEnumOutputs != DXGI_ERROR_NOT_FOUND) {
      if (hEnumOutputs != S_OK) {
	printf("output enumeration error: 0x%X\n", hEnumOutputs);

	throw 1;
      }

      DXGI_OUTPUT_DESC oDesc;
      pOutput->GetDesc(&oDesc);

      if (oDesc.AttachedToDesktop) {
        return (IDXGIOutput1*)pOutput;
      }

      ++outputIndex;
      hEnumOutputs = pAdapter->EnumOutputs(outputIndex, &pOutput);
    }

    ++adapterIndex;
  }

  throw 2;
}
