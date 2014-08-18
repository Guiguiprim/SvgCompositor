#ifndef SVG_ASSEMBLY_SIZE_HPP
#define SVG_ASSEMBLY_SIZE_HPP

namespace SvgCompose
{

enum SvgAssemblySize {
  S16 = 16,
  S24 = 24,
  S32 = 32,
  S48 = 48,
  S64 = 64,
  S128 = 128,
  S256 = 256,

  SMin = S16,
  SMax = S256
};

SvgAssemblySize toComposeSize(int size);
SvgAssemblySize increaseSize(SvgAssemblySize size);
SvgAssemblySize decreaseSize(SvgAssemblySize size);
bool isMinSize(SvgAssemblySize size);
bool isMaxSize(SvgAssemblySize size);

} // namespace Composition

#endif // SVG_ASSEMBLY_SIZE_HPP
