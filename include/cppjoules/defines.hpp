/**
 * Windows requires us to expose the classes in dlls
 */
#ifdef _WIN32
#define EXPOSE_DLL __declspec(dllexport)
#else
#define EXPOSE_DLL
#endif
