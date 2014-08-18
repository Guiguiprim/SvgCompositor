#ifndef COMPOSITION_ACTION_HPP
#define COMPOSITION_ACTION_HPP

namespace SvgCompositor
{

enum Action {
  // assembly action
  SetName = 0,
  SetBackground,
  RemoveBackground,
  AddElement,
  SizeIncrease,
  SizeDecrease,
  // assembly element action
  Remove,
  Lower,
  Raise,
  HCenter,
  VCenter,
  ScaleIncrease,
  ScaleDecrease,
  SetPos,
  MoveUp,
  MoveDown,
  MoveRight,
  MoveLeft
};

} // namespace Composition

#endif // COMPOSITION_ACTION_HPP
