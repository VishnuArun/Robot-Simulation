/**
 * @file graphics_arena_viewer.cc
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
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
#include "src/params.h"

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

  // Added from lab10
  window->setLayout(new nanogui::GroupLayout());
  // fin

  gui->addGroup("Simulation Control");

  playing_button_ =
    gui->addButton(
      "Play",
      std::bind(&GraphicsArenaViewer::OnPlayingBtnPressed, this));
  new_game_button_ =
    gui->addButton(
      "New Game",
      std::bind(&GraphicsArenaViewer::NewGameBtnPressed, this));
  food_button_ =
    gui->addButton(
      "Food Off",
      std::bind(&GraphicsArenaViewer::FoodBtnPressed, this));

  // added from lab10
  playing_button_->setFixedWidth(100);
  // fin

  gui->addGroup("Arena Configuration");

  // Creating a panel impacts the layout. Widgets, sliders, buttons can be
  // assigned to either the window or the panel.
  nanogui::Widget *panel = new nanogui::Widget(window);

  // *************** SLIDER 1 ************************//
  new nanogui::Label(panel, "Number of Robots", "sans-bold");
  nanogui::Slider *slider = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider->setValue(0.5f);
  slider->setFixedWidth(100);

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *textBox = new nanogui::TextBox(panel);
  textBox->setFixedSize(nanogui::Vector2i(60, 25));
  textBox->setFontSize(20);
  textBox->setValue("5");

  // This is the lambda function called while the user is moving the slider
  slider->setCallback(
    [textBox](float value) {
      textBox->setValue(std::to_string(static_cast<int>(value*10)));
    });
  slider->setFinalCallback(
    [&](float value) {
      robot_count_ = static_cast<int>(value*10);
      arena_->set_robots(robot_count_);
    });

  // *************** SLIDER 2 ************************//
  new nanogui::Label(panel, "Number of Lights", "sans-bold");
  nanogui::Slider *slider2 = new nanogui::Slider(panel);
  slider2->setValue(0.5f);
  slider2->setFixedWidth(100);
  // textBox->setUnits("%");

  nanogui::TextBox *textBox2 = new nanogui::TextBox(panel);
  textBox2->setFixedSize(nanogui::Vector2i(60, 25));
  textBox2->setFontSize(20);
  textBox2->setValue("5");
  // textBox2->setAlignment(nanogui::TextBox::Alignment::Right);

  slider2->setCallback(
    [textBox2](float value) {
      textBox2->setValue(std::to_string(static_cast<int>(value*5)));
    });

  slider2->setFinalCallback(
    [&](float value) {
      int light_count_ = static_cast<int>(value*5);
      arena_->set_lights(light_count_);
    });

  // *************** SLIDER 3 ************************//
  new nanogui::Label(panel, "Light Sensitivity", "sans-bold");
  nanogui::Slider *slider3 = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider3->setValue(1.0f);
  slider3->setFixedWidth(100);

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *textBox3 = new nanogui::TextBox(panel);
  textBox3->setFixedSize(nanogui::Vector2i(60, 25));
  textBox3->setFontSize(20);
  textBox3->setValue("10");

  // This is the lambda function called while the user is moving the slider
  slider3->setCallback(
    [textBox3](float value) {
      textBox3->setValue(std::to_string(static_cast<int>(value*10)));
    });
  /** This is the lambda function called once the user is no longer
  * manipulating the slider.Note robot_count_ is set, which is a
  * graphics_arena_ variable in this version, although
  * you should communicate that value to the controller so that
  * it can configure the Arena.
  */
  slider3->setFinalCallback(
    [&](float value) {
      robot_count_ = static_cast<int>(value*1200);
      arena_->AdjustLightSensitivity(robot_count_);
    });

  // *************** SLIDER 4 ************************//
  new nanogui::Label(panel, "Food Quanity", "sans-bold");
  nanogui::Slider *slider4 = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider4->setValue(0.5f);
  slider4->setFixedWidth(100);

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *textBox4 = new nanogui::TextBox(panel);
  textBox4->setFixedSize(nanogui::Vector2i(60, 25));
  textBox4->setFontSize(20);
  textBox4->setValue("5");

  // This is the lambda function called while the user is moving the slider
  slider4->setCallback(
    [textBox4](float value) {
      textBox4->setValue(std::to_string(static_cast<int>(value*10)));
    });
  /** This is the lambda function called once the user is no longer manipulating
  * the slider. Note robot_count_ is set, which is a graphics_arena_ variable
  * in this version, although you should communicate that value to the controller
  * so that it can configure the Arena.
  */
  slider4->setFinalCallback(
    [&](float value) {
      int food_quantity_ = static_cast<int>(value*10);
      arena_->set_food(food_quantity_);
    });
  panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
    nanogui::Alignment::Middle, 0, 15));
  screen()->performLayout();

  // *************** SLIDER 5 ************************//
  new nanogui::Label(panel, "Fear-Explore Ratio", "sans-bold");
  nanogui::Slider *slider5 = new nanogui::Slider(panel);
  // The starting value (range is from 0 to 1)
  // Note that below the displayed value is 10* slider value.
  slider5->setValue(0.5f);
  slider5->setFixedWidth(100);

  // Display the corresponding value of the slider in this textbox
  nanogui::TextBox *textBox5 = new nanogui::TextBox(panel);
  textBox5->setFixedSize(nanogui::Vector2i(60, 25));
  textBox5->setFontSize(20);
  textBox5->setValue("5");

  // This is the lambda function called while the user is moving the slider
  slider5->setCallback(
    [textBox5](float value) {
      textBox5->setValue(std::to_string(static_cast<int>(value*10)));
    });
  /** This is the lambda function called once the user is no longer manipulating
  * the slider. Note robot_count_ is set, which is a graphics_arena_ variable
  * in this version, although you should communicate that value to the controller
  * so that it can configure the Arena.
  */
  slider5->setFinalCallback(
    [&](float value) {
      int robot_ratio_ = static_cast<int>(value*robot_count_);
      arena_->set_robot_ratio(robot_ratio_);
    });
  panel->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical,
    nanogui::Alignment::Middle, 0, 15));
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

  if (arena_->get_game_status() == 1) {
    new_game_ = true;
    paused_ = true;
    status_button_->setCaption("Robot Starved");
    playing_button_->setCaption("Restart Simulation");
  }
}
}

/*******************************************************************************
 * Handlers for User Keyboard and Mouse Events
 ******************************************************************************/
void GraphicsArenaViewer::OnPlayingBtnPressed() {
  // Not implemented. Sample code provided to show how to implement.
  // multi-function button
  if (parameter_instantiation_flag_) {
    arena_->ParameterReset();
    parameter_instantiation_flag_ = false;
    playing_button_->setCaption("Pause");
    arena_->set_game_status(PLAYING);
    paused_ = !paused_;
    new_game_ = false;
  } else {
  if (new_game_) {
    // reset Environment
    status_button_->setCaption("Robot Simulator");
    // reset game
    arena_->Reset();


    controller_->AcceptCommunication(kNewGame);
    new_game_ = false;
    paused_ = true;
    parameter_instantiation_flag_ = false;

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
}
// Module for New game button including user communcation and reset behaviors
void GraphicsArenaViewer::NewGameBtnPressed() {
  status_button_->setCaption("Robot Simulator");
  // reset game

  parameter_instantiation_flag_ = true;
  arena_->Reset();

  // Need to Call new instance of arena in order to reset params

  controller_->AcceptCommunication(kNewGame);
  new_game_ = false;
  paused_ = true;
  playing_button_->setCaption("Play");
  arena_->set_game_status(PLAYING);
}

void GraphicsArenaViewer::FoodBtnPressed() {
  bool status = !arena_->get_food_button_status();
  arena_->set_food_button_status(status);
  if (status) {
    food_button_->setCaption("Food On");
  } else {
    food_button_->setCaption("Food Off");
  }
}
  // Check Game status and communicate to user


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

  nvgRestore(ctx);
  nvgRestore(ctx);

  // draw left sensors
  nvgBeginPath(ctx);
  nvgCircle(ctx, static_cast<float>(robot->UpdateSensorPosition('L').x),
  static_cast<float>(robot->UpdateSensorPosition('L').y),
  static_cast<float>(SENSOR_GRAPHICS_RADIUS));

  nvgFillColor(ctx, nvgRGBA(100, 100, 100, 255));
  nvgFill(ctx);

  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);

  // draw right sensors
  nvgBeginPath(ctx);
  nvgCircle(ctx, static_cast<float>(robot->UpdateSensorPosition('R').x),
  static_cast<float>(robot->UpdateSensorPosition('R').y),
  static_cast<float>(SENSOR_GRAPHICS_RADIUS));

  nvgFillColor(ctx, nvgRGBA(100, 100, 100, 255));
  nvgFill(ctx);

  nvgStrokeColor(ctx, nvgRGBA(0, 0, 0, 255));
  nvgStroke(ctx);
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
  // light's circle
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

  // light id text label
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
  std::vector<Robot *> robots = arena_->robot();
  for (auto &robot : robots) {
  DrawRobot(ctx, robot);
}
}

NAMESPACE_END(csci3081);
