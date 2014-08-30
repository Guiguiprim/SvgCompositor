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
  SizeChanged,
  SizeIncrease,
  SizeDecrease,
  // assembly element action
  Remove,
  Lower,
  Raise,
  HCenter,
  VCenter,
  ScaleChanged,
  ScaleIncrease,
  ScaleDecrease,
  SetPos,
  MoveDy,
  MoveUp,
  MoveDown,
  MoveDx,
  MoveRight,
  MoveLeft
};

} // namespace SvgCompositor

#endif // COMPOSITION_ACTION_HPP
