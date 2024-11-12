// WARNING: remove this header when compiling
#include "headers/NBCCommon.h"

/*task*/ int main() {
  OnFwd(OUT_A, 100);
  OnFwd(OUT_C, 100);
  Wait(5000);
  Off(OUT_A);
  Off(OUT_C);
}
