#include <UnitTest.h>

int packageTestsEnabled(const char * packageName, const char * packageDisabler)
{
  if (isSpecified(packageDisabler))
    return 0;

  if (!isSpecified("auto-eval") && !isSpecified("silent-tests"))
  {
    printf("  Testing %s (disable with %s)", packageName, packageDisabler);
    if (isSpecified("verbose-unittests"))
      printf("\n");
    else
      fflush(stdout);
  }
  return 1;
}

void runtest(const char * packageName, const char * functionName, const char * disablerName, void (*function)(void))
{
  if (!isSpecified(disablerName))
  {
    if (isSpecified("auto-eval"))
    {
      function();
      printf("##AUISWLOSP %s %s PSOLWSIUA##\n", packageName, functionName);
    }
    else
      if (!isSpecified("silent-tests"))
      {
        if (isSpecified("verbose-unittests"))
          printf("    -> %s (disable with %s)\n", functionName, disablerName);
        else
        {
          printf(".");
          (void) fflush(stdout);
        }
      }
    function();
  }
}

void endtest(void)
{
  if (!isSpecified("auto-eval"))
    if (!isSpecified("verbose-unittests"))
      printf("\n");
}

