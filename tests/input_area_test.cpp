#include "render/scene/input_area.h"
#include "render/scene/node.h"

#include <cstdio>
#include <linux/input-event-codes.h>
#include <memory>
#include <wayland-client-protocol.h>

namespace {

  bool expect(bool condition, const char* message) {
    if (!condition) {
      std::fprintf(stderr, "input_area_test: %s\n", message);
      return false;
    }
    return true;
  }

} // namespace

int main() {
  bool ok = true;

  {
    InputArea area;
    area.setSize(20.0f, 20.0f);

    int clicks = 0;
    area.setOnClick([&clicks](const InputArea::PointerData& data) {
      if (data.button == BTN_LEFT) {
        ++clicks;
      }
    });

    area.dispatchPress(10.0f, 10.0f, BTN_LEFT, true);
    area.dispatchPress(12.0f, 12.0f, BTN_LEFT, false);

    ok = expect(clicks == 1, "release inside fires click") && ok;
    ok = expect(!area.pressed(), "inside release clears pressed state") && ok;
  }

  {
    InputArea area;
    area.setSize(20.0f, 20.0f);

    int clicks = 0;
    int releases = 0;
    area.setOnClick([&clicks](const InputArea::PointerData&) { ++clicks; });
    area.setOnPress([&releases](const InputArea::PointerData& data) {
      if (!data.pressed) {
        ++releases;
      }
    });

    area.dispatchPress(10.0f, 10.0f, BTN_LEFT, true);
    ok = expect(area.pressed(), "press sets pressed state") && ok;

    area.dispatchPress(30.0f, 10.0f, BTN_LEFT, false);

    ok = expect(clicks == 0, "release outside cancels click") && ok;
    ok = expect(releases == 1, "outside release still dispatches press release") && ok;
    ok = expect(!area.pressed(), "outside release clears pressed state") && ok;
  }

  {
    InputArea area;
    area.setSize(20.0f, 20.0f);
    area.setHitTestOutset({.right = 10.0f});

    int clicks = 0;
    area.setOnClick([&clicks](const InputArea::PointerData&) { ++clicks; });

    area.dispatchPress(10.0f, 10.0f, BTN_LEFT, true);
    area.dispatchPress(25.0f, 10.0f, BTN_LEFT, false);

    ok = expect(clicks == 1, "release inside hit-test outset fires click") && ok;
  }

  {
    InputArea area;
    area.setSize(20.0f, 20.0f);

    int cancellations = 0;
    int clicks = 0;
    area.setOnCancel([&cancellations]() { ++cancellations; });
    area.setOnClick([&clicks](const InputArea::PointerData&) { ++clicks; });

    area.dispatchPress(10.0f, 10.0f, BTN_LEFT, true);
    ok = expect(area.pressed(), "press before cancellation sets pressed state") && ok;

    area.dispatchCancel();

    ok = expect(cancellations == 1, "cancellation callback fires exactly once") && ok;
    ok = expect(!area.pressed(), "cancellation clears pressed state") && ok;

    area.dispatchPress(10.0f, 10.0f, BTN_LEFT, false);
    ok = expect(clicks == 0, "release after cancellation does not click") && ok;
  }

  {
    Node root;
    root.setSize(20.0f, 20.0f);
    auto overflow = std::make_unique<InputArea>();
    auto* overflowPtr = overflow.get();
    overflow->setPosition(25.0f, 0.0f);
    overflow->setSize(10.0f, 10.0f);
    root.addChild(std::move(overflow));

    ok = expect(Node::hitTest(&root, 27.0f, 5.0f) == overflowPtr, "default hit test preserves child overflow") && ok;
    ok = expect(Node::hitTestStrict(&root, 27.0f, 5.0f) == nullptr, "strict hit test clips at ancestor bounds") && ok;
  }

  {
    // The axis counterpart of the button mask: a direction the area has given up is reported
    // unconsumed, so the dispatcher can carry it to an ancestor that claims it.
    InputArea area;
    area.setSize(20.0f, 20.0f);
    int verticalEvents = 0;
    area.setOnAxisHandler([&verticalEvents](const InputArea::PointerData&) {
      ++verticalEvents;
      return true;
    });

    // Wayland reports up as a negative delta.
    area.setAcceptedScrollDirections(
        InputArea::allScrollDirections() & ~InputArea::scrollDirectionMask(InputArea::ScrollDirection::Up)
    );

    const bool upConsumed = area.dispatchAxis(
        1.0f, 1.0f, WL_POINTER_AXIS_VERTICAL_SCROLL, WL_POINTER_AXIS_SOURCE_WHEEL, -10.0, 0, 0, -1.0f
    );
    ok = expect(!upConsumed, "an unaccepted scroll direction is reported unconsumed") && ok;
    ok = expect(verticalEvents == 0, "an unaccepted scroll direction never reaches the handler") && ok;

    const bool downConsumed =
        area.dispatchAxis(1.0f, 1.0f, WL_POINTER_AXIS_VERTICAL_SCROLL, WL_POINTER_AXIS_SOURCE_WHEEL, 10.0, 0, 0, 1.0f);
    ok = expect(downConsumed, "an accepted scroll direction is still consumed") && ok;
    ok = expect(verticalEvents == 1, "an accepted scroll direction reaches the handler") && ok;

    // Rejection happens before the accumulator, so the given-up direction banks nothing here.
    ok = expect(
             area.dispatchAxis(
                 1.0f, 1.0f, WL_POINTER_AXIS_HORIZONTAL_SCROLL, WL_POINTER_AXIS_SOURCE_WHEEL, -10.0, 0, 0, -1.0f
             ),
             "an untouched axis keeps its directions"
         )
        && ok;
  }

  return ok ? 0 : 1;
}
