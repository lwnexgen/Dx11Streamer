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

    ID3D11Device * pD3Device = createDirect3D11Device(pAttachedOutputAdapter);

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
      return 1;
    }
  }
}

ID3D11Device * createDirect3D11Device(IDXGIAdapter1 * pOutputAdapter) {
  D3D_FEATURE_LEVEL aFeatureLevels[] =
    {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
      D3D_FEATURE_LEVEL_9_1
    };

  ID3D11Device * pDevice;
  D3D_FEATURE_LEVEL featureLevel;
  ID3D11DeviceContext * pDeviceContext;

  D3D_DRIVER_TYPE DriverTypes[] =
    {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_REFERENCE,
      D3D_DRIVER_TYPE_NULL,
      D3D_DRIVER_TYPE_SOFTWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_UNKNOWN
    };
  
  HRESULT hCreateDevice;

  for (int driverTypeIndex = 0 ; driverTypeIndex < ARRAYSIZE(DriverTypes) ; driverTypeIndex++) {
    D3D_DRIVER_TYPE driverType = DriverTypes[driverTypeIndex];

    hCreateDevice = D3D11CreateDevice(pOutputAdapter,
					      driverType,
					      NULL,
					      0,
					      aFeatureLevels,
					      ARRAYSIZE(aFeatureLevels),
					      D3D11_SDK_VERSION,
					      &pDevice,
					      &featureLevel,
					      &pDeviceContext);
    if (hCreateDevice == S_OK) {
      printf("created D3D11 adapter at %p\n", pD3Device);
      printf("used driver type: %i\n", driverType);

      return pDevice;
    } else {
      printf("error creating D3D11 device using %i\n", driverType);
      printf("error creating D3D11 device: 0x%X\n", hCreateDevice);
    }
  }

  throw 4;
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
