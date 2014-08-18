#include <svg_compose/svg_assembly_size.hpp>

namespace SvgCompose
{

SvgAssemblySize toComposeSize(int size)
{
  if(size < (int)S24)
    return S16;
  if(size < (int)S32)
    return S24;
  if(size < (int)S48)
    return S32;
  if(size < (int)S64)
    return S48;
  if(size < (int)S128)
    return S64;
  if(size < (int)S256)
    return S128;
  return S256;
}

SvgAssemblySize increaseSize(SvgAssemblySize size)
{
  switch (size) {
  case S16: return S24;
  case S24: return S32;
  case S32: return S48;
  case S48: return S64;
  case S64: return S128;
  case S128: return S256;
  default:
    break;
  }
  return S256;
}

SvgAssemblySize decreaseSize(SvgAssemblySize size)
{
  switch (size) {
  case S24: return S16;
  case S32: return S24;
  case S48: return S32;
  case S64: return S48;
  case S128: return S64;
  case S256: return S128;
  default:
    break;
  }
  return S16;
}

bool isMinSize(SvgAssemblySize size)
{
  return size == SMin;
}

bool isMaxSize(SvgAssemblySize size)
{
  return size == SMax;
}

} // namespace Composition
