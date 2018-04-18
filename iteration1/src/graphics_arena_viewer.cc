/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <vector>
#include <iostream>

#include "src/graphics_arena_viewer.h"
#include "src/arena_params.h"
#include "src/rgb_color.h"
#include "src/robot.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/

GraphicsArenaViewer::GraphicsArenaViewer(
    const struct arena_params *const params,
    Arena * arena, Controller * controller) :
    GraphicsApp(
        params->x_dim + GUI_MENU_WIDTH + GUI_MENU_GAP * 2,
        params->y_dim,
        "Robot Simulation"),
    controller_(controller),
    arena_(arena) {
  auto *gui = new nanogui::FormHelper(screen());
  nanogui::ref<nanogui::Window> window =
      gui->addWindow(
          Eigen::Vector2i(10 + GUI_MENU_GAP, 10),
          "Menu");
  status_button_ = gui->addGroup("Robot Simulator");
  gui->addGroup("Simulation Control");

  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));
  new_game_button_ =
    gui->addButton(
      "New Game",
      std::bind(&GraphicsArenaViewer::NewGameBtnPressed, this));




  screen()->performLayout();
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/

// This is the primary driver for state change in the arena.
// It will be called at each iteration of nanogui::mainloop()
void GraphicsArenaViewer::UpdateSimulation(double dt) {
  // Do not advance time on pause
  if (!paused_) {
  controller_->AdvanceTime(dt);
}
if (arena_->robot()->get_lives() < 1) {
  arena_->set_game_status(LOST);
}




// Update Game Status
OnGameStatus();
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
void GraphicsArenaViewer::OnPlayingBtnPressed() {
  // Not implemented. Sample code provided to show how to implement.
  // multi-function button
  if (new_game_) {
    // reset Environment
    status_button_->setCaption("Robot Simulator");
    // reset game
    arena_->Reset();
    arena_->robot()->set_lives(9);
    controller_->AcceptCommunication(kNewGame);
    new_game_ = false;
    paused_ = true;
    playing_button_->setCaption("Play");
    arena_->set_game_status(PLAYING);

  } else {
    paused_ = !paused_;

    if (!paused_) {
      playing_button_->setCaption("Pause");
      controller_->AcceptCommunication(kPlay);
    } else {
      playing_button_->setCaption("Play");
      controller_->AcceptCommunication(kPause);
    }
  }
  }
// Module for New game button including user communcation and reset behaviors
void GraphicsArenaViewer::NewGameBtnPressed() {
  status_button_->setCaption("Robot Simulator");
  // reset game
  arena_->Reset();
  arena_->robot()->set_lives(9);
  controller_->AcceptCommunication(kNewGame);
  new_game_ = false;
  paused_ = true;
  playing_button_->setCaption("Play");
  arena_->set_game_status(PLAYING);
}
  // Check Game status and communicate to user
void GraphicsArenaViewer::OnGameStatus() {
  if (arena_->robot()->get_lives() < 1 && !new_game_) {
    status_button_->setCaption("All Lives Lost");
    playing_button_->setCaption("Game Over");

    paused_ = true;
    new_game_ = true;
  }
  if (arena_->get_game_status() == WON) {
    status_button_->setCaption("You Win!");
    playing_button_->setCaption("Good Job!");
    paused_ = true;
    new_game_ = true;
  }
}

/** OnSpecialKeyDown is called when the user presses down on one of the
  * special keys (e.g. the arrow keys).
  */
/**
 * @TODO: Check for arrow key presses using GLFW macros, then
 * convert to appropriate enum Communication and relay to controller
 */
void GraphicsArenaViewer::OnSpecialKeyDown(int key,
  __unused int scancode, __unused int modifiers) {
    Communication key_value = kNone;

    switch (key) {
      case GLFW_KEY_LEFT:
        key_value = kKeyLeft;
        break;
      case GLFW_KEY_RIGHT:
        key_value = kKeyRight;
        break;
      case GLFW_KEY_UP:
        key_value = kKeyUp;
        break;
      case GLFW_KEY_DOWN:
        key_value = kKeyDown;
        break;
      default: {}
    }
  // Check pause status before relaying inputs
  if (!paused_) {
  controller_->AcceptCommunication(key_value);
}
}

/*******************************************************************************
 * Drawing of Entities in Arena
 ******************************************************************************/
void GraphicsArenaViewer::DrawRobot(NVGcontext *ctx,
                                     const Robot *const robot) {
  // translate and rotate all graphics calls that follow so that they are
  // centered, at the position and heading of this robot
  nvgSave(ctx);
  nvgTranslate(ctx,
               static_cast<float>(robot->get_pose().x),
               static_cast<float>(robot->get_pose().y));
  nvgRotate(ctx,
            static_cast<float>(robot->get_pose().theta * M_PI / 180.0));

  // robot's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx, 0.0, 0.0, static_cast<float>(robot->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(robot->get_color().r, robot->get_color().g,
                       robot->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // robot id text label
  nvgSave(ctx);
  nvgRotate(ctx, static_cast<float>(M_PI / 2.0));
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx, 0.0, 10.0, (robot->get_name()  +  ' ' +
  (std::to_string(arena_->robot()->get_lives()))).c_str(), nullptr);
  nvgRestore(ctx);
  nvgRestore(ctx);
}
void GraphicsArenaViewer::DrawArena(NVGcontext *ctx) {
  nvgBeginPath(ctx);
  // Creates new rectangle shaped sub-path.
  nvgRect(ctx, 0, 0, arena_->get_x_dim(), arena_->get_y_dim());
  nvgStrokeColor(ctx, nvgRGBA(255, 255, 255, 255));
  nvgStroke(ctx);
}

void GraphicsArenaViewer::DrawEntity(NVGcontext *ctx,
                                       const ArenaEntity *const entity) {
  // obstacle's circle
  nvgBeginPath(ctx);
  nvgCircle(ctx,
            static_cast<float>(entity->get_pose().x),
            static_cast<float>(entity->get_pose().y),
            static_cast<float>(entity->get_radius()));
  nvgFillColor(ctx,
               nvgRGBA(entity->get_color().r, entity->get_color().g,
                       entity->get_color().b, 255));
  nvgFill(ctx);
  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // obstacle id text label
  nvgFillColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgText(ctx,
          static_cast<float>(entity->get_pose().x),
          static_cast<float>(entity->get_pose().y),
          entity->get_name().c_str(), nullptr);
}

void GraphicsArenaViewer::DrawUsingNanoVG(NVGcontext *ctx) {
  // initialize text rendering settings
  nvgFontSize(ctx, 18.0f);
  nvgFontFace(ctx, "sans-bold");
  nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
  DrawArena(ctx);
  std::vector<ArenaEntity *> entities = arena_->get_entities();
  for (auto &entity : entities) {
    DrawEntity(ctx, entity);
  } /* for(i..) */
  DrawRobot(ctx, arena_->robot());
}

NAMESPACE_END(csci3081);
